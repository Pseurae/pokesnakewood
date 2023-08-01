#include "global.h"
#include "malloc.h"
#include "bg.h"
#include "decompress.h"
#include "gpu_regs.h"
#include "palette.h"
#include "text.h"
#include "berry_pouch.h"
#include "item_menu.h"
#include "menu.h"
#include "menu_helpers.h"
#include "scanline_effect.h"
#include "item_menu_icons.h"
#include "list_menu.h"
#include "graphics.h"
#include "berry.h"
#include "item.h"
#include "item_icon.h"
#include "item_use.h"
#include "party_menu.h"
#include "text_window.h"
#include "strings.h"
#include "string_util.h"
#include "sound.h"
#include "link.h"
#include "money.h"
#include "shop.h"
#include "menu.h"
#include "pokemon_storage_system.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "constants/rgb.h"

enum
{
    ACTION_USE = 0,
    ACTION_TOSS,
    ACTION_GIVE,
    ACTION_CANCEL,
    ACTION_DUMMY
};

static EWRAM_DATA struct BerryPouch *sBerryPouch = NULL;
static EWRAM_DATA struct BerryPouchPosition sBerryPouchPosition = { 0 };
static EWRAM_DATA struct ListMenuItem *sListMenuItems = NULL;
static EWRAM_DATA u8 * sListMenuStrbuf = NULL;
static EWRAM_DATA const u8 * sContextMenuOptions = NULL;
static EWRAM_DATA u8 sContextMenuNumOptions = 0;
static ALIGNED(4) EWRAM_DATA u8 sVariableWindowIds[14] = {};
static ALIGNED(4) EWRAM_DATA u8 sBerryPouchSpriteId = 0;

const u32 gBerryPouchSpriteTiles[] = INCBIN_U32("graphics/berry_pouch/berry_pouch.4bpp.lz");
const u8 gBerryPouchBgGfx[] = INCBIN_U8("graphics/berry_pouch/background.4bpp.lz");
const u32 gBerryPouchBgPals[] = INCBIN_U32("graphics/berry_pouch/background.gbapal.lz");
const u32 gBerryPouchBgPal0FemaleOverride[] = INCBIN_U32("graphics/berry_pouch/background_female.gbapal.lz");
const u32 gBerryPouchSpritePalette[] = INCBIN_U32("graphics/berry_pouch/berry_pouch.gbapal.lz");
const u32 gBerryPouchBg1Tilemap[] = INCBIN_U32("graphics/berry_pouch/background.bin.lz");

static void CB2_BerryPouch(void);
static bool8 SetupBerryPouch(void);
static void AbortBerryPouchLoading(void);
static void Task_AbortBerryPouchLoading_WaitFade(u8 taskId);
static void BerryPouchInitBgs(void);
static bool8 BerryPouchLoadGfx(void);
static bool8 AllocateBerryPouchListBuffers(void);
static void SetUpListMenuTemplate(void);
static void GetBerryNameAndIndexForMenu(u8 * dest, u16 itemId);
static void BerryPouchMoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void BerryPouchItemPrintFunc(u8 windowId, u32 itemId, u8 y);
static void BerryPouch_PrintCursorAtPos(u8 y, u8 colorIdx);
static void PrintSelectedBerryDescription(s32 itemIndex);
static void CreateBerryPouchScrollArrowPair(void);
static void PrintBerryPouchHeaderCentered(void);
static void InitBerryPouchListPositions(void);
static void InitBerryPouchScrollPositions(void);
static void FreeBerryPouchBuffers(void);
static void Task_FadeAndCloseBerryPouch(u8 taskId);
static void Task_CloseBerryPouch(u8 taskId);
static void UpdateBerryPouchItemList(void);
static void Task_BerryPouch_HandleInput(u8 taskId);
static void Task_ContextMenu_Normal(u8 taskId);
static void Task_ContextMenu_Normal_HandleInput(u8 taskId);
static void Task_BerryPouch_Use(u8 taskId);
static void Task_BerryPouch_Toss(u8 taskId);
static void AskTossItems(u8 taskId);
static void CancelToss(u8 taskId);
static void Task_ChooseHowManyToToss(u8 taskId);
static void ConfirmToss(u8 taskId);
static void Task_RemoveBerryFromBag(u8 taskId);
static void Task_BerryPouch_Give(u8 taskId);
static void PrintThereIsNoPokemon(u8 taskId);
static void HandleErrorMessage(u8 taskId);
static void CloseBerryPouchMessage(u8 taskId);
static void Task_BerryPouch_Cancel(u8 taskId);
static void Task_ContextMenu_GiveToParty(u8 taskId);
static void Task_ContextMenu_Deposit(u8 taskId);
static void Task_ContextMenu_Sell(u8 taskId);
static void DisplaySellItemPriceAndConfirm(u8 taskId);
static void AskSellItems(u8 taskId);
static void CancelSell(u8 taskId);
static void InitSellHowManyInput(u8 taskId);
static void Task_ChooseHowManyToSell(u8 taskId);
static void ConfirmSell(u8 taskId);
static void Task_SellBerries_PlaySfxAndRemoveBerries(u8 taskId);
static void Task_SellBerries_WaitButton(u8 taskId);
static void BerryPouchInitWindows(void);
static void BerryPouch_Print(u8 windowId, u8 fontId, const u8 * str, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx);
static u8 GetOrCreateVariableWindow(u8 winIdx);
static void DestroyVariableWindow(u8 winIdx);
static void TryDestroyVariableWindow(u8 winIdx);
static u8 GetVariableWindowId(u8 winIdx);
static void DisplayItemMessageInBerryPouch(u8 taskId, u8 fontId, const u8 * str, TaskFunc followUpFunc);
static void CreateYesNoMenuWin3(u8 taskId, const struct YesNoFuncTable *ptrs);
static void CreateYesNoMenuWin4(u8 taskId, const struct YesNoFuncTable *ptrs);
static void PrintMoneyInWin2(void);
static void CreateBerryPouchSprite(void);
static void StartBerryPouchSpriteWobbleAnim(void);
static void SpriteCB_BerryPouchWaitWobbleAnim(struct Sprite *sprite);

static const struct BgTemplate sBgTemplates[] = 
{
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000
    }, 
    {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0x000
    }, 
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }
};

static const TaskFunc sContextMenuFuncs[] = 
{
    [BERRYPOUCH_FROM_FIELD] = Task_ContextMenu_Normal,
    [BERRYPOUCH_FROM_PARTY] = Task_ContextMenu_GiveToParty,
    [BERRYPOUCH_FROM_SHOP] = Task_ContextMenu_Sell,
    [BERRYPOUCH_FROM_ITEMPC] = Task_ContextMenu_Deposit,
    [BERRYPOUCH_FROM_BATTLE] = Task_ContextMenu_Normal
};

static const struct YesNoFuncTable sYesNoFuncs_Toss = 
{
    .yesFunc = ConfirmToss,
    .noFunc  = CancelToss
};

static const struct YesNoFuncTable sYesNoFuncs_Sell = 
{
    .yesFunc = ConfirmSell,
    .noFunc  = CancelSell
};

static const struct MenuAction sBerryPouchMenuActions[] = 
{
    [ACTION_USE]  = {gMenuText_Use,  Task_BerryPouch_Use},
    [ACTION_TOSS] = {gMenuText_Toss, Task_BerryPouch_Toss},
    [ACTION_GIVE] = {gMenuText_Give, Task_BerryPouch_Give},
    [ACTION_CANCEL] = {gText_Cancel2, Task_BerryPouch_Cancel}
};

static const u8 sOptions_UseGiveTossExit[] = {
    ACTION_USE,
    ACTION_GIVE,
    ACTION_TOSS,
    ACTION_CANCEL
};

static const u8 sOptions_GiveExit[] = {
    ACTION_GIVE,
    ACTION_CANCEL,
    ACTION_DUMMY,
    ACTION_DUMMY
};

static const u8 sOptions_Exit[] = {
    ACTION_CANCEL,
    ACTION_DUMMY,
    ACTION_DUMMY,
    ACTION_DUMMY
};

static const u8 sOptions_UseToss_Exit[] = {
    ACTION_USE,
    ACTION_TOSS,
    ACTION_CANCEL,
    ACTION_DUMMY
};

enum {
    WIN_LIST,
    WIN_DESCRIPTION,
    WIN_LABEL
};

static const struct WindowTemplate sWindowTemplates_Main[] = 
{
    [WIN_LIST] = {
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 1,
        .width = 18,
        .height = 14,
        .paletteNum = 3,
        .baseBlock = 0x027
    }, 
    [WIN_DESCRIPTION] = {
        .bg = 0,
        .tilemapLeft = 5,
        .tilemapTop = 16,
        .width = 25,
        .height = 4,
        .paletteNum = 3,
        .baseBlock = 0x123
    }, 
    [WIN_LABEL] = {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 9,
        .height = 2,
        .paletteNum = 3,
        .baseBlock = 0x187
    }, 
    DUMMY_WIN_TEMPLATE
};

enum
{
    BP_WIN_QUANTITY,
    BP_WIN_QUANTITY_WIDE,
    BP_WIN_MONEY,
    BP_WIN_3,
    BP_WIN_4,
    BP_WIN_5,
    BP_WIN_SELECTED,
    BP_WIN_TOSS_CONFIRM,
    BP_WIN_TOSS_HOW_MANY,
    BP_WIN_TOSS_THREW_AWAY,
    BP_WIN_CONTEXT1,
    BP_WIN_CONTEXT2,
    BP_WIN_CONTEXT3,
    BP_WIN_CONTEXT4,
    BP_WIN_COUNT,
};

static const struct WindowTemplate sWindowTemplates_Variable[BP_WIN_COUNT] = 
{
    [BP_WIN_QUANTITY] = {
        .bg = 0x02,
        .tilemapLeft = 0x18,
        .tilemapTop = 0x0f,
        .width = 5,
        .height = 4,
        .paletteNum = 3,
        .baseBlock = 0x1d1
    }, 
    [BP_WIN_QUANTITY_WIDE] = {
        .bg = 0x02,
        .tilemapLeft = 17,
        .tilemapTop = 9,
        .width = 10,
        .height = 4,
        .paletteNum = 3,
        .baseBlock = 0x1d1
    }, 
    [BP_WIN_MONEY] = {
        .bg = 0x02,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 8,
        .height = 2,
        .paletteNum = 3,
        .baseBlock = 0x201
    }, 
    [BP_WIN_3] = {
        .bg = 0x02,
        .tilemapLeft = 0x17,
        .tilemapTop = 0x0f,
        .width = 0x06,
        .height = 0x04,
        .paletteNum = 3,
        .baseBlock = 0x219
    }, 
    [BP_WIN_4] = {
        .bg = 0x02,
        .tilemapLeft = 0x15,
        .tilemapTop = 0x09,
        .width = 0x06,
        .height = 0x04,
        .paletteNum = 3,
        .baseBlock = 0x219
    }, 
    [BP_WIN_5] = {
        .bg = 0x02,
        .tilemapLeft = 0x02,
        .tilemapTop = 0x0f,
        .width = 0x1a,
        .height = 0x04,
        .paletteNum = 3,
        .baseBlock = 0x231
    }, 
    [BP_WIN_SELECTED] = {
        .bg = 0x02,
        .tilemapLeft = 0x06,
        .tilemapTop = 0x0f,
        .width = 0x0f,
        .height = 0x04,
        .paletteNum = 3,
        .baseBlock = 0x231
    }, 
    [BP_WIN_TOSS_CONFIRM] = {
        .bg = 0x02,
        .tilemapLeft = 0x06,
        .tilemapTop = 0x0f,
        .width = 0x0f,
        .height = 0x04,
        .paletteNum = 3,
        .baseBlock = 0x269
    }, 
    [BP_WIN_TOSS_HOW_MANY] = {
        .bg = 0x02,
        .tilemapLeft = 0x06,
        .tilemapTop = 0x0f,
        .width = 0x10,
        .height = 0x04,
        .paletteNum = 3,
        .baseBlock = 0x2a5
    }, 
    [BP_WIN_TOSS_THREW_AWAY] = {
        .bg = 0x02,
        .tilemapLeft = 0x06,
        .tilemapTop = 0x0f,
        .width = 0x17,
        .height = 0x04,
        .paletteNum = 3,
        .baseBlock = 0x2e5
    }, 
    [BP_WIN_CONTEXT1] = {
        .bg = 0x02,
        .tilemapLeft = 0x17,
        .tilemapTop = 0x11,
        .width = 0x06,
        .height = 0x02,
        .paletteNum = 3,
        .baseBlock = 0x199
    }, 
    [BP_WIN_CONTEXT2] = {
        .bg = 0x02,
        .tilemapLeft = 0x17,
        .tilemapTop = 0x0f,
        .width = 0x06,
        .height = 0x04,
        .paletteNum = 3,
        .baseBlock = 0x199
    }, 
    [BP_WIN_CONTEXT3] = {
        .bg = 0x02,
        .tilemapLeft = 0x17,
        .tilemapTop = 0x0d,
        .width = 0x06,
        .height = 0x06,
        .paletteNum = 3,
        .baseBlock = 0x199
    }, 
    [BP_WIN_CONTEXT4] = {
        .bg = 0x02,
        .tilemapLeft = 0x17,
        .tilemapTop = 0x0b,
        .width = 0x06,
        .height = 0x08,
        .paletteNum = 3,
        .baseBlock = 0x199
    }
};

enum
{
    COLORID_LIGHT,
    COLORID_DARK,
    COLORID_CURSOR_SELECTED,
    COLORID_NONE = 0xFF
};

static const u8 sFontColorTable[][3] = {
    [COLORID_LIGHT] = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY },
    [COLORID_DARK] = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY },
    [COLORID_CURSOR_SELECTED] = { TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GRAY, TEXT_COLOR_DARK_GRAY }
};

static const struct OamData sOamData = {
    .affineMode = ST_OAM_AFFINE_NORMAL,
    .shape = ST_OAM_SQUARE,
    .size = ST_OAM_SIZE_3,
    .priority = 1
};

static const union AnimCmd sSpriteAnim_Dummy[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sSpriteAnimTable[] = {
    sSpriteAnim_Dummy
};

static const union AffineAnimCmd sSpriteAffineAnim_Static[] = {
    AFFINEANIMCMD_FRAME(0x100, 0x100, 0, 0),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd sSpriteAffineAnim_Wobble[] = {
    AFFINEANIMCMD_FRAME(0, 0, -2, 2),
    AFFINEANIMCMD_FRAME(0, 0,  2, 4),
    AFFINEANIMCMD_FRAME(0, 0, -2, 4),
    AFFINEANIMCMD_FRAME(0, 0,  2, 2),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sSpriteAffineAnimTable[] = {
    sSpriteAffineAnim_Static,
    sSpriteAffineAnim_Wobble
};

static const struct CompressedSpriteSheet sBerryPouchSpriteSheet = {
    gBerryPouchSpriteTiles, 0x800, 100
};

static const struct CompressedSpritePalette sBerryPouchSpritePal = {
    gBerryPouchSpritePalette, 100
};

static const struct SpriteTemplate sSpriteTemplate_BerryPouch = {
    100, 100, &sOamData, sSpriteAnimTable, NULL, sSpriteAffineAnimTable, SpriteCallbackDummy
};

void OpenBerryPouch(u8 type, void (*exitCallback)(void), u8 allowSelect)
{
    sBerryPouch = AllocZeroed(sizeof(*sBerryPouch));
    if (sBerryPouch == NULL)
    {
        SetMainCallback2(exitCallback);
    }
    else
    {
        if (type != BERRYPOUCH_REOPENING)
            sBerryPouchPosition.type = type;
        if (allowSelect != 0xFF)
            sBerryPouchPosition.allowSelect = allowSelect;
        if (exitCallback != NULL)
            sBerryPouchPosition.exitCallback = exitCallback;
        sBerryPouch->newScreenCallback = NULL;
        sBerryPouch->iconSpriteId = MAX_SPRITES;
        sBerryPouch->indicatorTaskId = TASK_NONE;
        gTextFlags.autoScroll = FALSE;
        gSpecialVar_ItemId = ITEM_NONE;
        SetMainCallback2(CB2_BerryPouch);
    }
}

static void CB2_BerryPouchRun(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_BerryPouchRun(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_BerryPouch(void)
{
    while (MenuHelpers_ShouldWaitForLinkRecv() != TRUE && SetupBerryPouch() != TRUE && MenuHelpers_IsLinkActive() != TRUE)
        { };
}

#define tListTaskId data[0]
#define tListPosition data[1]
#define tQuantity data[2]
#define tItemCount data[8]

static bool8 SetupBerryPouch(void)
{
    u8 taskId;

    switch (gMain.state)
    {
    case 0:
        SetVBlankHBlankCallbacksToNull();
        ClearScheduledBgCopiesToVram();
        gMain.state++;
        break;
    case 1:
        ScanlineEffect_Stop();
        gMain.state++;
        break;
    case 2:
        FreeAllSpritePalettes();
        gMain.state++;
        break;
    case 3:
        ResetPaletteFade();
        gMain.state++;
        break;
    case 4:
        ResetSpriteData();
        gMain.state++;
        break;
    case 5:
        memset(gBagMenu->spriteIds, SPRITE_NONE, sizeof(gBagMenu->spriteIds));
        gMain.state++;
        break;
    case 6:
        if (!MenuHelpers_IsLinkActive())
            ResetTasks();
        gMain.state++;
        break;
    case 7:
        BerryPouchInitBgs();
        sBerryPouch->graphicsLoadState = 0;
        gMain.state++;
        break;
    case 8:
        if (BerryPouchLoadGfx())
            gMain.state++;
        break;
    case 9:
        BerryPouchInitWindows();
        gMain.state++;
        break;
    case 10:
        UpdateBerryPouchItemList();
        InitBerryPouchListPositions();
        InitBerryPouchScrollPositions();
        gMain.state++;
        break;
    case 11:
        if (!AllocateBerryPouchListBuffers())
        {
            AbortBerryPouchLoading();
            return TRUE;
        }
        gMain.state++;
        break;
    case 12:
        SetUpListMenuTemplate();
        gMain.state++;
        break;
    case 13:
        PrintBerryPouchHeaderCentered();
        gMain.state++;
        break;
    case 14:
        taskId = CreateTask(Task_BerryPouch_HandleInput, 0);
        gTasks[taskId].tListTaskId = ListMenuInit(&gMultiuseListMenuTemplate, sBerryPouchPosition.scrollOffset, sBerryPouchPosition.selectedRow);
        gTasks[taskId].tItemCount = 0;
        gMain.state++;
        break;
    case 15:
        CreateBerryPouchSprite();
        gMain.state++;
        break;
    case 16:
        CreateBerryPouchScrollArrowPair();
        gMain.state++;
        break;
    case 17:
        BlendPalettes(PALETTES_ALL, 16, RGB_BLACK);
        gMain.state++;
        break;
    case 18:
        BeginNormalPaletteFade(PALETTES_ALL, -2, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(VBlankCB_BerryPouchRun);
        SetMainCallback2(CB2_BerryPouchRun);
        return TRUE;
    }

    return FALSE;
}

static void AbortBerryPouchLoading(void)
{
    BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_BLACK);
    CreateTask(Task_AbortBerryPouchLoading_WaitFade, 0);
    SetVBlankCallback(VBlankCB_BerryPouchRun);
    SetMainCallback2(CB2_BerryPouchRun);
}

static void Task_AbortBerryPouchLoading_WaitFade(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetMainCallback2(sBerryPouchPosition.exitCallback);
        FreeBerryPouchBuffers();
        DestroyTask(taskId);
    }
}

static void BerryPouchInitBgs(void)
{
    ResetVramOamAndBgCntRegs();
    ResetAllBgsCoordinates();
    memset(sBerryPouch->tilemapBuffer, 0, BG_SCREEN_SIZE);
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sBgTemplates, NELEMS(sBgTemplates));
    SetBgTilemapBuffer(1, sBerryPouch->tilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
}

static bool8 BerryPouchLoadGfx(void)
{
    switch (sBerryPouch->graphicsLoadState)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, gBerryPouchBgGfx, 0, 0, 0);
        sBerryPouch->graphicsLoadState++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(gBerryPouchBg1Tilemap, sBerryPouch->tilemapBuffer);
            sBerryPouch->graphicsLoadState++;
        }
        break;
    case 2:
        FillPalette(0, BG_PLTT_ID(0), PLTT_SIZE_4BPP * 16);
        LoadCompressedPalette(gBerryPouchBgPals, 0, 0x60);
        if (gSaveBlock2Ptr->playerGender != MALE)
            LoadCompressedPalette(gBerryPouchBgPal0FemaleOverride, 0, 0x20);
        sBerryPouch->graphicsLoadState++;
        break;
    case 3:
        LoadCompressedSpriteSheet(&sBerryPouchSpriteSheet);
        sBerryPouch->graphicsLoadState++;
        break;
    default:
        LoadCompressedSpritePalette(&sBerryPouchSpritePal);
        sBerryPouch->graphicsLoadState = 0;
        return TRUE;
    }

    return FALSE;
}

static bool8 AllocateBerryPouchListBuffers(void)
{
    sListMenuItems = Alloc(MAX_BERRY_INDEX * sizeof(struct ListMenuItem));
    if (sListMenuItems == NULL)
        return FALSE;
    sListMenuStrbuf = Alloc(sBerryPouch->numBerries * 27);
    if (sListMenuStrbuf == NULL)
        return FALSE;
    return TRUE;
}

static void SetUpListMenuTemplate(void)
{
    u16 i;
    struct BagPocket *pocket = &gBagPockets[POCKET_BERRIES - 1];
    for (i = 0; i < sBerryPouch->numBerries; i++)
    {
        GetBerryNameAndIndexForMenu(&sListMenuStrbuf[i * 27], pocket->itemSlots[i].itemId);
        sListMenuItems[i].name = &sListMenuStrbuf[i * 27];
        sListMenuItems[i].id = i;
    }
    sListMenuItems[i].name = gText_Close;
    sListMenuItems[i].id = i;
    gMultiuseListMenuTemplate.items = sListMenuItems;
    if (sBerryPouchPosition.type != BERRYPOUCH_FROM_BERRYCRUSH)
        gMultiuseListMenuTemplate.totalItems = sBerryPouch->numBerries + 1;
    else
        gMultiuseListMenuTemplate.totalItems = sBerryPouch->numBerries;
    gMultiuseListMenuTemplate.windowId = 0;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 9;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 2;
    gMultiuseListMenuTemplate.upText_Y = 2;
    gMultiuseListMenuTemplate.maxShowed = sBerryPouch->numShownBerries;
    gMultiuseListMenuTemplate.fontId = FONT_SHORT;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 0;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = BerryPouchMoveCursorFunc;
    gMultiuseListMenuTemplate.itemPrintFunc = BerryPouchItemPrintFunc;
    gMultiuseListMenuTemplate.cursorKind = 0;
    gMultiuseListMenuTemplate.scrollMultiple = 0;
}

static void GetBerryNameAndIndexForMenu(u8 * dest, u16 itemId)
{
    StringCopy(gStringVar4, gText_FontSize0);
    StringAppend(gStringVar4, gText_NumberClear01);
    ConvertIntToDecimalStringN(gStringVar1, itemId - FIRST_BERRY_INDEX + 1, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringAppend(gStringVar4, gStringVar1);
    CopyItemName(itemId, gStringVar1);
    StringAppend(gStringVar4, gText_Space);
    StringAppend(gStringVar4, gText_FontSize2);
    StringAppend(gStringVar4, gStringVar1);
    StringCopy(dest, gStringVar4);
}

static void CopySelectedListMenuItemName(s16 itemIdx, u8 * dest)
{
    StringCopy(dest, &sListMenuStrbuf[itemIdx * 27]);
}

#define TAG_ITEM_ICON 5500

static void BerryPouchMoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    u8 spriteId;
    u16 itemId = BagGetItemIdByPocketPosition(POCKET_BERRIES, itemIndex);
    if (onInit != TRUE)
    {
        PlaySE(SE_SELECT);
        StartBerryPouchSpriteWobbleAnim();
    }

    FreeSpriteTilesByTag(TAG_ITEM_ICON);
    FreeSpritePaletteByTag(TAG_ITEM_ICON);

    if (sBerryPouch->iconSpriteId != MAX_SPRITES)
        DestroySprite(&gSprites[sBerryPouch->iconSpriteId]);

    if (sBerryPouch->numBerries != itemIndex)
        spriteId = AddItemIconSprite(TAG_ITEM_ICON, TAG_ITEM_ICON, itemId);
    else
        spriteId = AddItemIconSprite(TAG_ITEM_ICON, TAG_ITEM_ICON, ITEM_LIST_END);
    
    if (spriteId != MAX_SPRITES)
    {
        sBerryPouch->iconSpriteId = spriteId;
        gSprites[spriteId].x2 = 24;
        gSprites[spriteId].y2 = 147;
    }

    PrintSelectedBerryDescription(itemIndex);
}

static void BerryPouchItemPrintFunc(u8 windowId, u32 itemId, u8 y)
{
    u16 unused;
    u16 itemQuantity;
    if (itemId != LIST_CANCEL && sBerryPouch->numBerries != itemId)
    {
        unused = BagGetItemIdByPocketPosition(POCKET_BERRIES, itemId);
        itemQuantity = BagGetQuantityByPocketPosition(POCKET_BERRIES, itemId);
        ConvertIntToDecimalStringN(gStringVar1, itemQuantity, STR_CONV_MODE_RIGHT_ALIGN, 3);
        StringExpandPlaceholders(gStringVar4, gText_xVar1);
        BerryPouch_Print(windowId, FONT_SMALL, gStringVar4, 110, y, 0, 0, TEXT_SKIP_DRAW, COLORID_DARK);
    }
}

static void BerryPouch_PrintCursor(u8 taskId, u8 colorIdx)
{
    BerryPouch_PrintCursorAtPos(ListMenuGetYCoordForPrintingArrowCursor(taskId), colorIdx);
}

static void BerryPouch_PrintCursorAtPos(u8 y, u8 colorIdx)
{
    u8 width;
    u8 height;
    if (colorIdx == COLORID_NONE)
    {
        FillWindowPixelRect(WIN_LIST, 0, 1, y, GetMenuCursorDimensionByFont(FONT_SHORT, 0), GetMenuCursorDimensionByFont(FONT_SHORT, 1));
        CopyWindowToVram(WIN_LIST, COPYWIN_GFX);
    }
    else
    {
        BerryPouch_Print(WIN_LIST, FONT_SHORT, gText_SelectorArrow2, 0, y, 0, 0, 0, colorIdx);
    }
}

const u8 gText_TheBerryPouchWillBePutAway[] = _("The Berry Pouch will be\nput away.");

static void PrintSelectedBerryDescription(s32 itemIdx)
{
    const u8 * str;
    if (itemIdx != sBerryPouch->numBerries)
        str = ItemId_GetDescription(BagGetItemIdByPocketPosition(POCKET_BERRIES, itemIdx));
    else
        str = gText_TheBerryPouchWillBePutAway;
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    BerryPouch_Print(WIN_DESCRIPTION, FONT_SHORT, str, 0, 2, 2, 0, 0, COLORID_LIGHT);
}

static void ShadeDescriptionWindow(bool8 darken)
{
    SetBgTilemapPalette(1, 0, 16, 30, 4, darken + 1);
    ScheduleBgCopyTilemapToVram(1);
}

static void CreateBerryPouchScrollArrowPair(void)
{
    if (sBerryPouchPosition.type != BERRYPOUCH_FROM_BERRYCRUSH)
        sBerryPouch->indicatorTaskId = AddScrollIndicatorArrowPairParameterized(2, 160, 8, 120, sBerryPouch->numBerries - sBerryPouch->numShownBerries + 1, 110, 110, &sBerryPouchPosition.scrollOffset);
    else
        sBerryPouch->indicatorTaskId = AddScrollIndicatorArrowPairParameterized(2, 160, 8, 120, sBerryPouch->numBerries - sBerryPouch->numShownBerries, 110, 110, &sBerryPouchPosition.scrollOffset);
}

static void CreateBerryPouchTossQuantityArrowPair(void)
{
    sBerryPouch->indicatorOffset = 1;
    sBerryPouch->indicatorTaskId = AddScrollIndicatorArrowPairParameterized(2, 212, 120, 152, 2, 110, 110, &sBerryPouch->indicatorOffset);
}

static void CreateBerryPouchSellQuantityArrowPair(void)
{
    sBerryPouch->indicatorOffset = 1;
    sBerryPouch->indicatorTaskId = AddScrollIndicatorArrowPairParameterized(2, 152, 72, 104, 2, 110, 110, &sBerryPouch->indicatorOffset);
}

static void DestroyBerryPouchArrowPair(void)
{
    if (sBerryPouch->indicatorTaskId != TASK_NONE)
    {
        RemoveScrollIndicatorArrowPair(sBerryPouch->indicatorTaskId);
        sBerryPouch->indicatorTaskId = TASK_NONE;
    }
}

static void PrintBerryPouchHeaderCentered(void)
{
    const u8 *name = ItemId_GetName(ITEM_BERRY_POUCH);
    u32 slack = 72 - GetStringWidth(FONT_SHORT, name, 0);
    BerryPouch_Print(WIN_LABEL, FONT_SHORT, name, slack / 2, 1, 0, 0, 0, COLORID_LIGHT);
}

static void InitBerryPouchListPositions(void)
{
    s32 r2;
    if (sBerryPouchPosition.type != BERRYPOUCH_FROM_BERRYCRUSH)
        r2 = sBerryPouch->numBerries + 1;
    else
        r2 = sBerryPouch->numBerries;
    if (sBerryPouchPosition.scrollOffset != 0 && sBerryPouchPosition.scrollOffset + sBerryPouch->numShownBerries > r2)
        sBerryPouchPosition.scrollOffset = r2 - sBerryPouch->numShownBerries;
    if (sBerryPouchPosition.scrollOffset + sBerryPouchPosition.selectedRow >= r2)
    {
        if (r2 == 0 || r2 == 1)
            sBerryPouchPosition.selectedRow = 0;
        else
            sBerryPouchPosition.selectedRow = r2 - 1;
    }
}

static void InitBerryPouchScrollPositions(void)
{
    u8 lim;
    u8 i;
    if (sBerryPouchPosition.type != BERRYPOUCH_FROM_BERRYCRUSH)
        lim = sBerryPouch->numBerries + 1;
    else
        lim = sBerryPouch->numBerries;
    if (sBerryPouchPosition.selectedRow > 4)
    {
        for (i = 0; i <= sBerryPouchPosition.selectedRow - 4; sBerryPouchPosition.selectedRow--, sBerryPouchPosition.scrollOffset++, i++)
        {
            if (sBerryPouchPosition.scrollOffset + sBerryPouch->numShownBerries == lim)
                break;
        }
    }
}

static void FreeBerryPouchBuffers(void)
{
    if (sBerryPouch != NULL)
        Free(sBerryPouch);
    if (sListMenuItems != NULL)
        Free(sListMenuItems);
    if (sListMenuStrbuf != NULL)
        Free(sListMenuStrbuf);
    FreeAllWindowBuffers();
}

static void Task_FadeAndCloseBerryPouch(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_CloseBerryPouch;
}

static void Task_CloseBerryPouch(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (!gPaletteFade.active)
    {
        DestroyListMenuTask(tListTaskId, &sBerryPouchPosition.scrollOffset, &sBerryPouchPosition.selectedRow);
        if (sBerryPouch->newScreenCallback != NULL)
            SetMainCallback2(sBerryPouch->newScreenCallback);
        else
            SetMainCallback2(sBerryPouchPosition.exitCallback);
        DestroyBerryPouchArrowPair();
        FreeBerryPouchBuffers();
        DestroyTask(taskId);
    }
}

static void UpdateBerryPouchItemList(void)
{
    u16 i;
    u32 r2;
    struct BagPocket *pocket = &gBagPockets[POCKET_BERRIES - 1];
    SortBerriesOrTMHMs(pocket);
    sBerryPouch->numBerries = 0;
    for (i = 0; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i].itemId == ITEM_NONE)
            break;
        sBerryPouch->numBerries++;
    }
    if (sBerryPouchPosition.type != BERRYPOUCH_FROM_BERRYCRUSH)
        r2 = sBerryPouch->numBerries + 1;
    else
        r2 = sBerryPouch->numBerries;
    if (r2 > 7)
        sBerryPouch->numShownBerries = 7;
    else
        sBerryPouch->numShownBerries = r2;
}

static void PrintItemQuantity(u8 whichWindow, s16 quantity)
{
    u8 windowId = GetVariableWindowId(whichWindow);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    ConvertIntToDecimalStringN(gStringVar1, quantity, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringExpandPlaceholders(gStringVar4, gText_xVar1);
    BerryPouch_Print(windowId, FONT_SMALL, gStringVar4, 4, 10, 1, 0, 0, COLORID_DARK);
}

static void PrintSellItemAmount(u8 whichWindow, s16 quantity, u8 price)
{
    u8 windowId = GetVariableWindowId(whichWindow);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    ConvertIntToDecimalStringN(gStringVar1, quantity, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringExpandPlaceholders(gStringVar4, gText_xVar1);
    BerryPouch_Print(windowId, FONT_SMALL, gStringVar4, 4, 10, 1, 0, 0, COLORID_DARK);
    PrintMoneyAmount(GetVariableWindowId(BP_WIN_QUANTITY_WIDE), 38, 10, price, 0);
}

static void AddItemQuantityWindow(u8 taskId, const u8 *str)
{
    s16 *data = gTasks[taskId].data;
    u8 windowId = GetOrCreateVariableWindow(BP_WIN_TOSS_HOW_MANY);

    StringCopy(gStringVar1, ItemId_GetName(BagGetItemIdByPocketPosition(POCKET_BERRIES, tListPosition)));
    StringExpandPlaceholders(gStringVar4, str);
    BerryPouch_Print(windowId, FONT_SHORT, gStringVar4, 0, 2, 1, 2, 0, COLORID_DARK);

    PrintItemQuantity(BP_WIN_QUANTITY, 1);
}

static void Task_BerryPouch_HandleInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s32 menuInput;
    if (!gPaletteFade.active && MenuHelpers_ShouldWaitForLinkRecv() != TRUE)
    {
        menuInput = ListMenu_ProcessInput(tListTaskId);
        ListMenuGetScrollAndRow(tListTaskId, &sBerryPouchPosition.scrollOffset, &sBerryPouchPosition.selectedRow);
        if (JOY_NEW(SELECT_BUTTON) && sBerryPouchPosition.allowSelect == 1)
        {
            PlaySE(SE_SELECT);
            gSpecialVar_ItemId = 0;
            Task_FadeAndCloseBerryPouch(taskId);
        }
        else
        {
            switch (menuInput)
            {
            case LIST_NOTHING_CHOSEN:
                return;
            case LIST_CANCEL:
                if (sBerryPouchPosition.type != BERRYPOUCH_FROM_BERRYCRUSH)
                {
                    PlaySE(SE_SELECT);
                    gSpecialVar_ItemId = 0;
                    Task_FadeAndCloseBerryPouch(taskId);
                }
                break;
            default:
                PlaySE(SE_SELECT);
                if (sBerryPouchPosition.type == BERRYPOUCH_FROM_BERRYCRUSH)
                {
                    gSpecialVar_ItemId = BagGetItemIdByPocketPosition(POCKET_BERRIES, menuInput);
                    Task_FadeAndCloseBerryPouch(taskId);
                }
                else if (menuInput == sBerryPouch->numBerries)
                {
                    gSpecialVar_ItemId = 0;
                    Task_FadeAndCloseBerryPouch(taskId);
                }
                else
                {
                    DestroyBerryPouchArrowPair();
                    ShadeDescriptionWindow(TRUE);
                    BerryPouch_PrintCursor(tListTaskId, 2);
                    tListPosition = menuInput;
                    tQuantity = BagGetQuantityByPocketPosition(POCKET_BERRIES, menuInput);
                    gSpecialVar_ItemId = BagGetItemIdByPocketPosition(POCKET_BERRIES, menuInput);
                    gTasks[taskId].func = sContextMenuFuncs[sBerryPouchPosition.type];
                }
                break;
            }
        }
    }
}

static void ReturnToBerryList(u8 taskId)
{
    ShadeDescriptionWindow(FALSE);
    CreateBerryPouchScrollArrowPair();
    gTasks[taskId].func = Task_BerryPouch_HandleInput;
}

static void CreateNormalContextMenu(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 windowId;
    u8 windowId2;

    if (sBerryPouchPosition.type == BERRYPOUCH_FROM_BATTLE)
    {
        sContextMenuOptions = sOptions_UseToss_Exit;
        sContextMenuNumOptions = 3;
    }
    else if (MenuHelpers_IsLinkActive() == TRUE || InUnionRoom() == TRUE)
    {
        if (!IsHoldingItemAllowed(gSpecialVar_ItemId))
        {
            sContextMenuOptions = sOptions_Exit;
            sContextMenuNumOptions = 1;
        }
        else
        {
            sContextMenuOptions = sOptions_GiveExit;
            sContextMenuNumOptions = 2;
        }
    }
    else
    {
        sContextMenuOptions = sOptions_UseGiveTossExit;
        sContextMenuNumOptions = 4;
    }
    windowId = GetOrCreateVariableWindow(sContextMenuNumOptions + BP_WIN_TOSS_THREW_AWAY);
    PrintMenuActionTexts(windowId, FONT_SHORT, GetMenuCursorDimensionByFont(FONT_SHORT, 0), 2, GetFontAttribute(FONT_SHORT, FONTATTR_LETTER_SPACING), GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_HEIGHT) + 2, sContextMenuNumOptions, sBerryPouchMenuActions, sContextMenuOptions);
    InitMenuNormal(windowId, FONT_SHORT, 0, 2, GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_HEIGHT) + 2, sContextMenuNumOptions, 0);

    windowId2 = GetOrCreateVariableWindow(BP_WIN_SELECTED);
    CopySelectedListMenuItemName(tListPosition, gStringVar1);
    StringExpandPlaceholders(gStringVar4, gText_Var1IsSelected);
    BerryPouch_Print(windowId2, FONT_SHORT, gStringVar4, 0, 2, 1, 1, 0, COLORID_DARK);
}

static void Task_ContextMenu_Normal(u8 taskId)
{
    CreateNormalContextMenu(taskId);
    gTasks[taskId].func = Task_ContextMenu_Normal_HandleInput;
}

static void Task_ContextMenu_Normal_HandleInput(u8 taskId)
{
    s8 input;
    if (MenuHelpers_ShouldWaitForLinkRecv() != TRUE)
    {
        input = Menu_ProcessInputNoWrap();
        switch (input)
        {
        case LIST_CANCEL:
            break;
        case LIST_NOTHING_CHOSEN:
            PlaySE(SE_SELECT);
            sBerryPouchMenuActions[ACTION_CANCEL].func.void_u8(taskId);
            break;
        default:
            PlaySE(SE_SELECT);
            sBerryPouchMenuActions[sContextMenuOptions[input]].func.void_u8(taskId);
            break;
        }
    }
}

static void Task_BerryPouch_Use(u8 taskId)
{
    DestroyVariableWindow(sContextMenuNumOptions + BP_WIN_TOSS_THREW_AWAY);
    DestroyVariableWindow(BP_WIN_SELECTED);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(2);
    if (sBerryPouchPosition.type == BERRYPOUCH_FROM_BATTLE)
    {
        if (ItemId_GetBattleFunc(gSpecialVar_ItemId) == NULL)
            ItemUseOutOfBattle_CannotUse(taskId);
        else
            ItemId_GetBattleFunc(gSpecialVar_ItemId)(taskId);
    }
    else if (CalculatePlayerPartyCount() == 0)
        PrintThereIsNoPokemon(taskId);
    else
        ItemId_GetFieldFunc(gSpecialVar_ItemId)(taskId);
}

static void Task_BerryPouch_Toss(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    ClearWindowTilemap(GetVariableWindowId(sContextMenuNumOptions + BP_WIN_TOSS_THREW_AWAY));
    ClearWindowTilemap(GetVariableWindowId(BP_WIN_SELECTED));
    DestroyVariableWindow(sContextMenuNumOptions + BP_WIN_TOSS_THREW_AWAY);
    DestroyVariableWindow(BP_WIN_SELECTED);
    PutWindowTilemap(0);
    tItemCount = 1;
    if (tQuantity == 1)
        AskTossItems(taskId);
    else
    {
        AddItemQuantityWindow(taskId, gText_TossHowManyVar1s);
        CreateBerryPouchTossQuantityArrowPair();
        gTasks[taskId].func = Task_ChooseHowManyToToss;
    }
}

static void AskTossItems(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    ConvertIntToDecimalStringN(gStringVar2, tItemCount, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, gText_ConfirmTossItems);
    BerryPouch_Print(GetOrCreateVariableWindow(BP_WIN_TOSS_CONFIRM), FONT_SHORT, gStringVar4, 0, 2, 1, 2, 0, COLORID_DARK);
    CreateYesNoMenuWin3(taskId, &sYesNoFuncs_Toss);
}

static void CancelToss(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    DestroyVariableWindow(BP_WIN_TOSS_CONFIRM);
    PutWindowTilemap(1);
    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(2);
    BerryPouch_PrintCursor(tListTaskId, 1);
    ReturnToBerryList(taskId);
}

static void Task_ChooseHowManyToToss(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (AdjustQuantityAccordingToDPadInput(&tItemCount, tQuantity) == TRUE)
    {
        PrintItemQuantity(BP_WIN_QUANTITY, tItemCount);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearWindowTilemap(GetVariableWindowId(BP_WIN_TOSS_HOW_MANY));
        DestroyVariableWindow(BP_WIN_TOSS_HOW_MANY);
        DestroyVariableWindow(BP_WIN_QUANTITY);
        ScheduleBgCopyTilemapToVram(0);
        ScheduleBgCopyTilemapToVram(2);
        DestroyBerryPouchArrowPair();
        AskTossItems(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyVariableWindow(BP_WIN_TOSS_HOW_MANY);
        DestroyVariableWindow(BP_WIN_QUANTITY);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        ScheduleBgCopyTilemapToVram(2);
        BerryPouch_PrintCursor(tListTaskId, 1);
        DestroyBerryPouchArrowPair();
        ReturnToBerryList(taskId);
    }
}

static void ConfirmToss(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    DestroyVariableWindow(BP_WIN_TOSS_CONFIRM);
    CopySelectedListMenuItemName(tListPosition, gStringVar1);
    ConvertIntToDecimalStringN(gStringVar2, tItemCount, STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, gText_ThrewAwayVar2Var1s);
    BerryPouch_Print(GetOrCreateVariableWindow(BP_WIN_TOSS_THREW_AWAY), FONT_SHORT, gStringVar4, 0, 2, 1, 2, 0, COLORID_DARK);
    gTasks[taskId].func = Task_RemoveBerryFromBag;
}

static void Task_RemoveBerryFromBag(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        RemoveBagItem(gSpecialVar_ItemId, tItemCount);
        DestroyVariableWindow(BP_WIN_TOSS_THREW_AWAY);
        DestroyListMenuTask(tListTaskId, &sBerryPouchPosition.scrollOffset, &sBerryPouchPosition.selectedRow);
        UpdateBerryPouchItemList();
        InitBerryPouchListPositions();
        SetUpListMenuTemplate();
        tListTaskId = ListMenuInit(&gMultiuseListMenuTemplate, sBerryPouchPosition.scrollOffset, sBerryPouchPosition.selectedRow);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        BerryPouch_PrintCursor(tListTaskId, 1);
        ReturnToBerryList(taskId);
    }
}

static void Task_BerryPouch_Give(u8 taskId)
{
    DestroyVariableWindow(sContextMenuNumOptions + BP_WIN_TOSS_THREW_AWAY);
    DestroyVariableWindow(BP_WIN_SELECTED);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(2);
    if (CalculatePlayerPartyCount() == 0)
        PrintThereIsNoPokemon(taskId);
    else
    {
        sBerryPouch->newScreenCallback = CB2_ChooseMonToGiveItem;
        gTasks[taskId].func = Task_FadeAndCloseBerryPouch;
    }
}

static void PrintThereIsNoPokemon(u8 taskId)
{
    DisplayItemMessageInBerryPouch(taskId, FONT_SHORT, gText_NoPokemon, HandleErrorMessage);
}

static void HandleErrorMessage(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        CloseBerryPouchMessage(taskId);
    }
}

static void CloseBerryPouchMessage(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    TryDestroyVariableWindow(BP_WIN_5);
    DestroyListMenuTask(tListTaskId, &sBerryPouchPosition.scrollOffset, &sBerryPouchPosition.selectedRow);
    UpdateBerryPouchItemList();
    InitBerryPouchListPositions();
    SetUpListMenuTemplate();
    tListTaskId = ListMenuInit(&gMultiuseListMenuTemplate, sBerryPouchPosition.scrollOffset, sBerryPouchPosition.selectedRow);
    ScheduleBgCopyTilemapToVram(0);
    BerryPouch_PrintCursor(tListTaskId, 1);
    ReturnToBerryList(taskId);
}

static void Task_BerryPouch_Cancel(u8 taskId)
{
    DestroyVariableWindow(sContextMenuNumOptions + BP_WIN_TOSS_THREW_AWAY);
    DestroyVariableWindow(BP_WIN_SELECTED);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(2);
    BerryPouch_PrintCursor(gTasks[taskId].tListTaskId, 1);
    ReturnToBerryList(taskId);
}

static void Task_ContextMenu_GiveToParty(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 itemId = BagGetItemIdByPocketPosition(POCKET_BERRIES, tListPosition);
    if (!IsHoldingItemAllowed(itemId))
    {
        CopyItemName(itemId, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_Var1CantBeHeldHere);
        DisplayItemMessageInBerryPouch(taskId, FONT_SHORT, gStringVar4, HandleErrorMessage);
    }
    else
    {
        sBerryPouch->newScreenCallback = CB2_GiveHoldItem;
        gTasks[taskId].func = Task_FadeAndCloseBerryPouch;
    }
}

static void Task_ContextMenu_Deposit(u8 taskId)
{
    sBerryPouch->newScreenCallback = CB2_ReturnToPokeStorage;
    gTasks[taskId].func = Task_FadeAndCloseBerryPouch;
}

static void Task_ContextMenu_Sell(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (ItemId_GetPrice(gSpecialVar_ItemId) == 0)
    {
        CopyItemName(gSpecialVar_ItemId, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_CantBuyKeyItem);
        DisplayItemMessageInBerryPouch(taskId, FONT_SHORT, gStringVar4, CloseBerryPouchMessage);
    }
    else
    {
        tItemCount = 1;
        if (tQuantity == 1)
        {
            PrintMoneyInWin2();
            DisplaySellItemPriceAndConfirm(taskId);
        }
        else
        {
            if (tQuantity > 99)
                tQuantity = 99;
            CopyItemName(gSpecialVar_ItemId, gStringVar2);
            StringExpandPlaceholders(gStringVar4, gText_HowManyToSell);
            DisplayItemMessageInBerryPouch(taskId, FONT_SHORT, gStringVar4, InitSellHowManyInput);
        }
    }
}

static void DisplaySellItemPriceAndConfirm(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    ConvertIntToDecimalStringN(gStringVar3, ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_BERRIES, tListPosition)) / 2 * tItemCount, STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, gText_ICanPayVar1);
    DisplayItemMessageInBerryPouch(taskId, FONT_SHORT, gStringVar4, AskSellItems);
}

static void AskSellItems(u8 taskId)
{
    CreateYesNoMenuWin4(taskId, &sYesNoFuncs_Sell);
}

static void CancelSell(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    DestroyVariableWindow(BP_WIN_MONEY);
    TryDestroyVariableWindow(BP_WIN_5);
    PutWindowTilemap(2);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    BerryPouch_PrintCursor(tListTaskId, 1);
    RemoveMoneyLabelObject();
    ReturnToBerryList(taskId);
}

static void InitSellHowManyInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u8 windowId = GetOrCreateVariableWindow(BP_WIN_QUANTITY_WIDE);
    PrintSellItemAmount(BP_WIN_QUANTITY_WIDE, tItemCount, ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_BERRIES, tListPosition)) / 2 * tItemCount);
    CreateBerryPouchSellQuantityArrowPair();
    gTasks[taskId].func = Task_ChooseHowManyToSell;
}

static void Task_ChooseHowManyToSell(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    if (AdjustQuantityAccordingToDPadInput(&tItemCount, tQuantity) == TRUE)
    {
        PrintSellItemAmount(BP_WIN_QUANTITY_WIDE, tItemCount, ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_BERRIES, tListPosition)) / 2 * tItemCount);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyVariableWindow(BP_WIN_QUANTITY_WIDE);
        PutWindowTilemap(0);
        ScheduleBgCopyTilemapToVram(0);
        DestroyBerryPouchArrowPair();
        DisplaySellItemPriceAndConfirm(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyVariableWindow(BP_WIN_QUANTITY_WIDE);
        DestroyVariableWindow(BP_WIN_MONEY);
        RemoveMoneyLabelObject();
        TryDestroyVariableWindow(BP_WIN_5);
        PutWindowTilemap(2);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        DestroyBerryPouchArrowPair();
        BerryPouch_PrintCursor(tListTaskId, 1);
        ReturnToBerryList(taskId);
    }
}

static void ConfirmSell(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    ConvertIntToDecimalStringN(gStringVar3, ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_BERRIES, tListPosition)) / 2 * tItemCount, STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, gText_TurnedOverVar1ForVar2);
    DisplayItemMessageInBerryPouch(taskId, FONT_SHORT, gStringVar4, Task_SellBerries_PlaySfxAndRemoveBerries);
}

static void Task_SellBerries_PlaySfxAndRemoveBerries(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    PlaySE(SE_SHOP);
    RemoveBagItem(gSpecialVar_ItemId, tItemCount);
    AddMoney(&gSaveBlock1Ptr->money, ItemId_GetPrice(gSpecialVar_ItemId) / 2 * tItemCount);
    DestroyListMenuTask(tListTaskId, &sBerryPouchPosition.scrollOffset, &sBerryPouchPosition.selectedRow);
    UpdateBerryPouchItemList();
    InitBerryPouchListPositions();
    SetUpListMenuTemplate();
    tListTaskId = ListMenuInit(&gMultiuseListMenuTemplate, sBerryPouchPosition.scrollOffset, sBerryPouchPosition.selectedRow);
    BerryPouch_PrintCursor(tListTaskId, 2);
    PrintMoneyAmountInMoneyBox(GetVariableWindowId(BP_WIN_MONEY), GetMoney(&gSaveBlock1Ptr->money), 0);
    gTasks[taskId].func = Task_SellBerries_WaitButton;
}

#undef tListTaskId
#undef tListPosition
#undef tQuantity
#undef tItemCount

static void Task_SellBerries_WaitButton(u8 taskId)
{
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyVariableWindow(BP_WIN_MONEY);
        RemoveMoneyLabelObject();
        PutWindowTilemap(2);
        CloseBerryPouchMessage(taskId);
    }
}

static void BerryPouchInitWindows(void)
{
    u8 i;
    InitWindows(sWindowTemplates_Main);
    DeactivateAllTextPrinters();

    LoadMessageBoxGfx(WIN_LIST, 1, BG_PLTT_ID(3));
    LoadUserWindowBorderGfx(WIN_LIST, 21, BG_PLTT_ID(4));
    LoadStdBoxGfx(WIN_LIST, 30, BG_PLTT_ID(5));

    for (i = 0; i < 3; i++)
        FillWindowPixelBuffer(i, PIXEL_FILL(0));
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(2);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(2);
    memset(sVariableWindowIds, WINDOW_NONE, sizeof(sVariableWindowIds));
}

static void BerryPouch_Print(u8 windowId, u8 fontId, const u8 * str, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx)
{
    AddTextPrinterParameterized4(windowId, fontId, x, y, letterSpacing, lineSpacing, sFontColorTable[colorIdx], speed, str);
}

static u8 GetOrCreateVariableWindow(u8 winIdx)
{
    u8 retval = sVariableWindowIds[winIdx];
    if (retval == WINDOW_NONE)
    {
        sVariableWindowIds[winIdx] = AddWindow(&sWindowTemplates_Variable[winIdx]);
        DrawStdFrameWithCustomTileAndPalette(sVariableWindowIds[winIdx], FALSE, 21, 4);
        ScheduleBgCopyTilemapToVram(2);
        retval = sVariableWindowIds[winIdx];
    }
    return retval;
}

static void VariableWindowSetAltFrameTileAndPalette(u8 winIdx)
{
    DrawStdFrameWithCustomTileAndPalette(sVariableWindowIds[winIdx], FALSE, 21, 4);
}

static void DestroyVariableWindow(u8 winIdx)
{
    ClearStdWindowAndFrameToTransparent(sVariableWindowIds[winIdx], FALSE);
    ClearWindowTilemap(sVariableWindowIds[winIdx]);
    RemoveWindow(sVariableWindowIds[winIdx]);
    ScheduleBgCopyTilemapToVram(2);
    sVariableWindowIds[winIdx] = WINDOW_NONE;
}

static void TryDestroyVariableWindow(u8 winIdx)
{
    if (sVariableWindowIds[winIdx] != WINDOW_NONE)
    {
        ClearDialogWindowAndFrameToTransparent(sVariableWindowIds[winIdx], FALSE);
        ClearWindowTilemap(sVariableWindowIds[winIdx]);
        RemoveWindow(sVariableWindowIds[winIdx]);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        ScheduleBgCopyTilemapToVram(2);
        sVariableWindowIds[winIdx] = WINDOW_NONE;
    }
}

static u8 GetVariableWindowId(u8 winIdx)
{
    return sVariableWindowIds[winIdx];
}

static void DisplayItemMessageInBerryPouch(u8 taskId, u8 fontId, const u8 * str, TaskFunc followUpFunc)
{
    if (sVariableWindowIds[5] == WINDOW_NONE)
        sVariableWindowIds[5] = AddWindow(&sWindowTemplates_Variable[5]);
    DisplayMessageAndContinueTask(taskId, sVariableWindowIds[5], 1, 3, fontId, GetPlayerTextSpeedDelay(), str, followUpFunc);
    ScheduleBgCopyTilemapToVram(2);
}

static void CreateYesNoMenuWin3(u8 taskId, const struct YesNoFuncTable *ptrs)
{
    CreateYesNoMenuWithCallbacks(taskId, &sWindowTemplates_Variable[3], FONT_SHORT, 0, 2, 21, 4, ptrs);
}

static void CreateYesNoMenuWin4(u8 taskId, const struct YesNoFuncTable *ptrs)
{
    CreateYesNoMenuWithCallbacks(taskId, &sWindowTemplates_Variable[4], FONT_SHORT, 0, 2, 21, 4, ptrs);
}

static void PrintMoneyInWin2(void)
{
    AddMoneyLabelObject(19, 11);
    PrintMoneyAmountInMoneyBoxWithBorder(GetOrCreateVariableWindow(BP_WIN_MONEY), 30, 5, GetMoney(&gSaveBlock1Ptr->money));
}

static void CreateBerryPouchSprite(void)
{
    sBerryPouchSpriteId = CreateSprite(&sSpriteTemplate_BerryPouch, 40, 76, 0);
}

static void StartBerryPouchSpriteWobbleAnim(void)
{
    struct Sprite *sprite = &gSprites[sBerryPouchSpriteId];
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, 1);
        sprite->callback = SpriteCB_BerryPouchWaitWobbleAnim;
    }
}

static void SpriteCB_BerryPouchWaitWobbleAnim(struct Sprite *sprite)
{
    if (sprite->affineAnimEnded)
    {
        StartSpriteAffineAnim(sprite, 0);
        sprite->callback = SpriteCallbackDummy;
    }
}
