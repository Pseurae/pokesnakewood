#ifndef GUARD_BERRY_POUCH_H
#define GUARD_BERRY_POUCH_H

#include "task.h"

enum BerryPouchType
{
    BERRYPOUCH_FROM_FIELD,
    BERRYPOUCH_FROM_PARTY,
    BERRYPOUCH_FROM_SHOP,
    BERRYPOUCH_FROM_ITEMPC,
    BERRYPOUCH_FROM_BATTLE,
    BERRYPOUCH_FROM_BERRYCRUSH,
    BERRYPOUCH_REOPENING
};

struct BerryPouch
{
    void (*newScreenCallback)(void);
    u16 indicatorOffset;
    u8 indicatorTaskId;
    u8 numBerries;
    u8 numShownBerries;
    u8 iconSpriteId;
    u8 tilemapBuffer[BG_SCREEN_SIZE];
    s16 graphicsLoadState;
    s16 data[3];
};

struct BerryPouchPosition
{
    void (*exitCallback)(void);
    u8 type;
    u8 allowSelect;
    u16 selectedRow;
    u16 scrollOffset;
};

void OpenBerryPouch(u8 type, void (*savedCallback)(void), u8 allowSelect);

#endif //GUARD_BERRY_POUCH_H
