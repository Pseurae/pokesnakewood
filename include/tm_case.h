#ifndef GUARD_TM_CASE_H
#define GUARD_TM_CASE_H

enum TMCaseType
{
    TMCASE_FROM_FIELD,
    TMCASE_FROM_PARTY,
    TMCASE_FROM_SHOP,
    TMCASE_FROM_ITEMPC,
    TMCASE_REOPENING
};

struct TMCase
{
    void (*newScreenCallback)(void);
    u8 tilemapBuffer[BG_SCREEN_SIZE];
    u8 tmSpriteId;
    u8 numShownTMs;
    u8 numTMs;
    u8 contextMenuWindowId;
    u8 scrollArrowsTask;
    const u8 *contextMenuItemsPtr;
    u8 contextMenuNumItems;
    s16 graphicsLoadState;
};

struct TMCasePosition
{
    void (*exitCallback)(void);
    u8 type;
    u16 selectedRow;
    u16 scrollOffset;
};

void OpenTMCase(u8 type, void (* exitCallback)(void), u8 a2);
void ResetTMCaseCursorPos(void);

#endif //GUARD_TM_CASE_H
