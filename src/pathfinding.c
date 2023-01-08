#include "global.h"
#include "pathfinding.h"
#include "event_object_movement.h"
#include "fieldmap.h"
#include "overworld.h"
#include "script_movement.h"
#include "malloc.h"

// TODO: Implement a SMA* routine.

EWRAM_DATA u8 *sMovementScript = NULL;

static u8 CheckCollisionAtCoords(struct ObjectEvent *objectEvent, s16 x, s16 y, u32 dir, u8 currentElevation);

// Heap functions

static int heap_parent(int i)
{
    return (i - 1) / 2;
}

static int heap_left_child(int i)
{
    return 2 * i + 1;
}

static int heap_right_child(int i)
{
    return 2 * i + 2;
}

static void node_swap(struct Node **node1, struct Node **node2)
{
    struct Node *temp = *node1;
    *node1 = *node2;
    *node2 = temp;
}

static void heap_push(struct NodeHeap *heap, struct Node *node)
{
    int32_t i;
    heap->nodes[heap->count] = node;
    heap->count += 1;

    i = heap->count - 1;
    while (i != 0 && heap->nodes[heap_parent(i)]->f > heap->nodes[i]->f)
    {
        node_swap(&heap->nodes[heap_parent(i)], &heap->nodes[i]);
        i = heap_parent(i);
    }
}

void heap_heapify(struct NodeHeap *heap, int i)
{
    int left = heap_left_child(i);
    int right = heap_right_child(i);

    int largest = i;

    if (left <= heap->count && heap->nodes[left]->f < heap->nodes[largest]->f)
        largest = left;

    if (right <= heap->count && heap->nodes[right]->f < heap->nodes[largest]->f)
        largest = right;

    if (largest != i)
    {
        struct Node *temp = heap->nodes[i];
        heap->nodes[i] = heap->nodes[largest];
        heap->nodes[largest] = temp;
        heap_heapify(heap, largest);
    }
}

static struct Node *heap_pop(struct NodeHeap *heap)
{
    struct Node *max_item = heap->nodes[0];

    heap->nodes[0] = heap->nodes[heap->count - 1];
    heap->count -= 1;

    heap_heapify(heap, 0);
    return max_item;
}

// End of Heap functions

// Map functions

static struct Node *map_insert(struct NodeMap *map, u32 key, struct Node node)
{
    map->keys[map->count] = key;
    map->nodes[map->count] = node;
    return &map->nodes[map->count++];
}

static bool8 map_exists(struct NodeMap *map, u32 key)
{
    u16 i;

    for (i = 0; i < map->count; i++)
    {
        if (map->keys[i] == key)
            return TRUE;
    }

    return FALSE;
}

static struct Node *map_get(struct NodeMap *map, u32 key)
{
    u16 i;

    for (i = 0; i < map->count; i++)
    {
        if (map->keys[i] == key)
            return &map->nodes[i];
    }

    return NULL;
}

// End of Map functions

static const int8_t child_offsets[4][2] =
{
    {0, 1},
    {0, -1},
    {-1, 0},
    {1, 0},
};

static struct Node node_create_new(u16 x, u16 y, u16 g, u16 f)
{
    struct Node node = {x, y, g, f, 0, NULL};
    return node;
}

static struct Node node_create_child(struct Node *parent, struct Node *end, u8 i)
{
    s32 dx, dy;
    struct Node node;

    node.x = parent->x + child_offsets[i][0];
    node.y = parent->y + child_offsets[i][1];
    node.g = parent->g + 1;

    dx = node.x - end->x;
    dy = node.y - end->y;
    node.f = node.g + 2 * (abs(dx) + abs(dy));

    return node;
}

static bool8 find_path(struct ObjectEvent *objEvent, u16 endx, u16 endy)
{
    uint32_t index, startx, starty;
    int i, steps;
    struct Node *node, child, *inserted, startnode, endnode;
    struct NodeMap map = { 0 };
    struct NodeHeap heap = { 0 };
    int32_t width, height;

    map.keys = AllocZeroed(sizeof(uint32_t) * MAP_NODES);
    map.nodes = AllocZeroed(sizeof(struct Node) * MAP_NODES);
    heap.nodes = AllocZeroed(sizeof(struct Node *) * HEAP_NODES);

    startx = objEvent->currentCoords.x - MAP_OFFSET;
    starty = objEvent->currentCoords.y - MAP_OFFSET;

    width = GetMapLayout()->width;
    height = GetMapLayout()->height;

    startnode = node_create_new(startx, starty, 0, 0);
    endnode = node_create_new(endx, endy, 0, 0);

    DebugPrintf("Starts: %d %d\n", startnode.x, startnode.y);
    DebugPrintf("Ends: %d %d\n", endnode.x, endnode.y);

    heap_push(&heap, &startnode);

    while (heap.count)
    {
        node = heap_pop(&heap);

        DebugPrintf("Popped %d, %d\n", node->x, node->y);

        if (node->x == endx && node->y == endy)
        {
            steps = node->g;
            i = steps - 1;
            sMovementScript = AllocZeroed(steps + 1);
            sMovementScript[steps] = MOVEMENT_ACTION_STEP_END;

            DebugPrintf("\nFinal path:\n");
            while (node && i >= 0)
            {
                sMovementScript[i--] = MOVEMENT_ACTION_WALK_NORMAL_DOWN + node->dir;
                DebugPrintf("%d %d\n", node->x, node->y);
                node = node->parent;
            }

            Free(map.nodes);
            Free(heap.nodes);
            return TRUE;
        }

        if (heap.count < HEAP_NODES && map.count < MAP_NODES)
        {
            for (i = 0; i < 4; i++)
            {
                child = node_create_child(node, &endnode, i);

                if (CheckCollisionAtCoords(objEvent, child.x + MAP_OFFSET, child.y + MAP_OFFSET, i + 1, objEvent->currentElevation))
                    continue;

                index = child.x + child.y * width;
                child.parent = node;
                child.dir = i;

                DebugPrintf("Checking neighbour: %d %d (%u %u)\n", child.x, child.y, index, child.f);

                if (!map_exists(&map, index) || child.f < node->f)
                {
                    if (map.count < MAP_NODES)
                    {
                        inserted = map_insert(&map, index, child);
                        if (heap.count < HEAP_NODES)
                        {
                            heap_push(&heap, inserted);
                            DebugPrintf("Inserted %d %d\n", inserted->x, inserted->y);
                        }
                    }
                }
            }
        }
    }

    DebugPrintf("No path.\n");
    Free(map.nodes);
    Free(heap.nodes);
    return FALSE;
}

static bool8 DoesObjectCollideWithObjectAtZ(struct ObjectEvent *objectEvent, s16 x, s16 y)
{
    u8 i;
    struct ObjectEvent *curObject;

    for (i = 0; i < OBJECT_EVENTS_COUNT; i++)
    {
        curObject = &gObjectEvents[i];
        if (curObject->active && curObject != objectEvent)
        {
            if ((curObject->currentCoords.x == x && curObject->currentCoords.y == y) || 
                (curObject->previousCoords.x == x && curObject->previousCoords.y == y))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}

static u8 CheckCollisionAtCoords(struct ObjectEvent *objectEvent, s16 x, s16 y, u32 dir, u8 currentElevation)
{
    u8 direction = dir;
    if (MapGridGetCollisionAt(x, y) || GetMapBorderIdAt(x, y) == CONNECTION_INVALID || IsMetatileDirectionallyImpassable(objectEvent, x, y, direction))
        return COLLISION_IMPASSABLE;
    else if (objectEvent->trackedByCamera && !CanCameraMoveInDirection(direction))
        return COLLISION_IMPASSABLE;
    else if (IsElevationMismatchAt(objectEvent->currentElevation, x, y))
        return COLLISION_ELEVATION_MISMATCH;
    else if (DoesObjectCollideWithObjectAt(objectEvent, x, y))
        return COLLISION_OBJECT_EVENT;
    return COLLISION_NONE;
}

// End of pathfinding functions

bool8 ScriptMovement_StartPath(u8 localId, u8 mapNum, u8 mapGroup, s32 x, s32 y, u8 mode)
{
    u8 objectEventId;

    if (!TryGetObjectEventIdByLocalIdAndMap(localId, mapNum, mapGroup, &objectEventId))
    {
        if (find_path(&gObjectEvents[objectEventId], x, y))
        {
            ScriptMovement_StartObjectMovementScript(localId, mapNum, mapGroup, sMovementScript);
            FREE_AND_SET_NULL(sMovementScript);
            return TRUE;
        }
    }

    return FALSE;
}
