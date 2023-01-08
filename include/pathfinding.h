#ifndef GUARD_PATHFINDING_H
#define GUARD_PATHFINDING_H

#define HEAP_NODES 200
#define MAP_NODES (HEAP_NODES * 4)

struct Node
{
    s16 x, y;
    u16 g, f;
    u8 dir;
    struct Node *parent;
};

struct NodeMap
{
    u16 count;
    u32 *keys;
    struct Node *nodes;
};

struct NodeHeap
{
    u16 count;
    struct Node **nodes;
};

bool8 ScriptMovement_StartPath(u8 localId, u8 mapNum, u8 mapGroup, s32 x, s32 y, u8 mode);

#endif // GUARD_PATHFINDING_H