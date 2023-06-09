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
#include "money.h"
#include "shop.h"
#include "menu.h"
#include "pokemon_storage_system.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "constants/rgb.h"

struct BerryPouch
{
    void (*exitCallback)(void);
    u16 indicatorOffset;
    u8 indicatorTaskId;
    u8 listMenuNumItems;
    u8 listMenuMaxShowed;
    u8 iconSpriteId;
    u8 ALIGNED(4) bg1TilemapBuffer[BG_SCREEN_SIZE];
    s16 data[4];
};

struct BerryPouchPosition
{
    void (*savedCallback)(void);
    u8 type;
    u8 allowSelect;
    u16 listMenuSelectedRow;
    u16 listMenuScrollOffset;
};

enum
{
    BP_ACTION_USE = 0,
    BP_ACTION_TOSS,
    BP_ACTION_GIVE,
    BP_ACTION_EXIT,
    BP_ACTION_DUMMY
};

static EWRAM_DATA struct BerryPouch *sBerryPouch = NULL;
static EWRAM_DATA struct BerryPouchPosition sBerryPouchPosition = {};
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
static bool8 AllocateListMenuBuffers(void);
static void SetUpListMenuTemplate(void);
static void GetBerryNameAndIndexForMenu(u8 * dest, u16 itemId);
static void BerryPouchMoveCursorFunc(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void BerryPouchItemPrintFunc(u8 windowId, u32 itemId, u8 y);
static void BerryPouchSetArrowCursorAt(u8 y, u8 colorIdx);
static void PrintSelectedBerryDescription(s32 itemIndex);
static void CreateScrollIndicatorArrows_BerryPouchList(void);
static void PrintBerryPouchHeaderCentered(void);
static void SanitizeListMenuSelectionParams(void);
static void UpdateListMenuScrollOffset(void);
static void BerryPouch_DestroyResources(void);
static void Task_BerryPouchFadeToExitCallback(u8 taskId);
static void SortAndCountBerries(void);
static void Task_BerryPouchMain(u8 taskId);
static void Task_NormalContextMenu(u8 taskId);
static void Task_NormalContextMenu_HandleInput(u8 taskId);
static void Task_BerryPouch_Use(u8 taskId);
static void Task_BerryPouch_Toss(u8 taskId);
static void Task_AskTossMultiple(u8 taskId);
static void Task_TossNo(u8 taskId);
static void Task_Toss_SelectMultiple(u8 taskId);
static void Task_TossYes(u8 taskId);
static void Task_WaitButtonThenTossBerries(u8 taskId);
static void Task_BerryPouch_Give(u8 taskId);
static void Task_Give_PrintThereIsNoPokemon(u8 taskId);
static void Task_WaitButtonBeforeDialogueWindowDestruction(u8 taskId);
static void Task_BerryPouch_Exit(u8 taskId);
static void Task_ContextMenu_FromPartyGiveMenu(u8 taskId);
static void Task_ContextMenu_FromPokemonPC(u8 taskId);
static void Task_ContextMenu_Sell(u8 taskId);
static void Task_AskSellMultiple(u8 taskId);
static void Task_SellMultiple_CreateYesNoMenu(u8 taskId);
static void Task_SellNo(u8 taskId);
static void Task_Sell_PrintSelectMultipleUI(u8 taskId);
static void SellMultiple_UpdateSellPriceDisplay(s32 price);
static void Task_Sell_SelectMultiple(u8 taskId);
static void Task_SellYes(u8 taskId);
static void Task_SellBerries_PlaySfxAndRemoveBerries(u8 taskId);
static void Task_SellBerries_WaitButton(u8 taskId);
static void BerryPouchInitWindows(void);
static void BerryPouchPrint(u8 windowId, u8 fontId, const u8 * str, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx);
static u8 GetOrCreateVariableWindow(u8 winIdx);
static void DestroyVariableWindow(u8 winIdx);
static void TryDestroyVariableWindow(u8 winIdx);
static u8 GetVariableWindowId(u8 winIdx);
static void CreateYesNoMenuWin3(u8 taskId, const struct YesNoFuncTable *ptrs);
static void CreateYesNoMenuWin4(u8 taskId, const struct YesNoFuncTable *ptrs);
static void PrintMoneyInWin2(void);
static void CreateBerryPouchSprite(void);
static void StartBerryPouchSpriteWobbleAnim(void);
static void SpriteCB_BerryPouchWaitWobbleAnim(struct Sprite *sprite);

static const struct BgTemplate sBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0x000
    }, {
        .bg = 1,
        .charBaseIndex = 3,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0x000
    }, {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }
};

static const TaskFunc sBerryPouchContextMenuTasks[] = {
    Task_NormalContextMenu,
    Task_ContextMenu_FromPartyGiveMenu,
    Task_ContextMenu_Sell,
    Task_ContextMenu_FromPokemonPC,
    Task_NormalContextMenu
};

static const struct YesNoFuncTable sYesNoFuncs_Toss = {
    .yesFunc = Task_TossYes,
    .noFunc  = Task_TossNo
};

static const struct YesNoFuncTable sYesNoFuncs_Sell = {
    .yesFunc = Task_SellYes,
    .noFunc  = Task_SellNo
};

static const struct MenuAction sContextMenuActions[] = {
    {gMenuText_Use,  Task_BerryPouch_Use},
    {gMenuText_Toss, Task_BerryPouch_Toss},
    {gMenuText_Give, Task_BerryPouch_Give},
    {gText_MenuExit, Task_BerryPouch_Exit}
};

static const u8 sOptions_UseGiveTossExit[] = {
    BP_ACTION_USE,
    BP_ACTION_GIVE,
    BP_ACTION_TOSS,
    BP_ACTION_EXIT
};

static const u8 sOptions_GiveExit[] = {
    BP_ACTION_GIVE,
    BP_ACTION_EXIT,
    BP_ACTION_DUMMY,
    BP_ACTION_DUMMY
};

static const u8 sOptions_Exit[] = {
    BP_ACTION_EXIT,
    BP_ACTION_DUMMY,
    BP_ACTION_DUMMY,
    BP_ACTION_DUMMY
};

static const u8 sOptions_UseToss_Exit[] = {
    BP_ACTION_USE,
    BP_ACTION_TOSS,
    BP_ACTION_EXIT,
    BP_ACTION_DUMMY
};

static const u8 sText_Space[] = _(" ");

enum {
    WIN_BERRY_LIST,
    WIN_DESCRIPTION,
    WIN_POCKET_NAME
};

static const struct WindowTemplate sWindowTemplates_Main[] = {
    [WIN_BERRY_LIST] = {
        .bg = 0,
        .tilemapLeft = 11,
        .tilemapTop = 1,
        .width = 18,
        .height = 14,
        .paletteNum = 15,
        .baseBlock = 0x027
    }, 
    [WIN_DESCRIPTION] = {
        .bg = 0,
        .tilemapLeft = 5,
        .tilemapTop = 16,
        .width = 25,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x123
    }, 
    [WIN_POCKET_NAME] = {
        .bg = 2,
        .tilemapLeft = 1,
        .tilemapTop = 1,
        .width = 9,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 0x187
    }, 
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sWindowTemplates_Variable[] = {
    {
        .bg = 0x02,
        .tilemapLeft = 0x18,
        .tilemapTop = 0x0f,
        .width = 0x05,
        .height = 0x04,
        .paletteNum = 15,
        .baseBlock = 0x1d1
    }, 
    {
        .bg = 0x02,
        .tilemapLeft = 0x11,
        .tilemapTop = 0x09,
        .width = 0x0c,
        .height = 0x04,
        .paletteNum = 15,
        .baseBlock = 0x1d1
    }, 
    {
        .bg = 0x02,
        .tilemapLeft = 0x01,
        .tilemapTop = 0x01,
        .width = 0x08,
        .height = 0x03,
        .paletteNum = 15,
        .baseBlock = 0x201
    }, 
    {
        .bg = 0x02,
        .tilemapLeft = 0x17,
        .tilemapTop = 0x0f,
        .width = 0x06,
        .height = 0x04,
        .paletteNum = 15,
        .baseBlock = 0x219
    }, 
    {
        .bg = 0x02,
        .tilemapLeft = 0x15,
        .tilemapTop = 0x09,
        .width = 0x06,
        .height = 0x04,
        .paletteNum = 0x0f,
        .baseBlock = 0x219
    }, 
    {
        .bg = 0x02,
        .tilemapLeft = 0x02,
        .tilemapTop = 0x0f,
        .width = 0x1a,
        .height = 0x04,
        .paletteNum = 0x0f,
        .baseBlock = 0x231
    }, 
    {
        .bg = 0x02,
        .tilemapLeft = 0x06,
        .tilemapTop = 0x0f,
        .width = 0x0e,
        .height = 0x04,
        .paletteNum = 15,
        .baseBlock = 0x231
    }, {
        .bg = 0x02,
        .tilemapLeft = 0x06,
        .tilemapTop = 0x0f,
        .width = 0x0f,
        .height = 0x04,
        .paletteNum = 0x0c,
        .baseBlock = 0x269
    }, {
        .bg = 0x02,
        .tilemapLeft = 0x06,
        .tilemapTop = 0x0f,
        .width = 0x10,
        .height = 0x04,
        .paletteNum = 0x0c,
        .baseBlock = 0x2a5
    }, {
        .bg = 0x02,
        .tilemapLeft = 0x06,
        .tilemapTop = 0x0f,
        .width = 0x17,
        .height = 0x04,
        .paletteNum = 0x0c,
        .baseBlock = 0x2e5
    }, {
        .bg = 0x02,
        .tilemapLeft = 0x16,
        .tilemapTop = 0x11,
        .width = 0x07,
        .height = 0x02,
        .paletteNum = 0x0f,
        .baseBlock = 0x199
    }, {
        .bg = 0x02,
        .tilemapLeft = 0x16,
        .tilemapTop = 0x0f,
        .width = 0x07,
        .height = 0x04,
        .paletteNum = 0x0f,
        .baseBlock = 0x199
    }, {
        .bg = 0x02,
        .tilemapLeft = 0x16,
        .tilemapTop = 0x0d,
        .width = 0x07,
        .height = 0x06,
        .paletteNum = 0x0f,
        .baseBlock = 0x199
    }, {
        .bg = 0x02,
        .tilemapLeft = 0x16,
        .tilemapTop = 0x0b,
        .width = 0x07,
        .height = 0x08,
        .paletteNum = 0x0f,
        .baseBlock = 0x199
    }
};

static const u8 sTextColors[][3] = {
    { 0, 1, 2 },
    { 0, 2, 3 },
    { 0, 3, 2 }
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

void InitBerryPouch(u8 type, void (*savedCallback)(void), u8 allowSelect)
{
    u8 i;

    sBerryPouch = Alloc(sizeof(struct BerryPouch));
    if (sBerryPouch == NULL)
    {
        SetMainCallback2(savedCallback);
    }
    else
    {
        if (type != BERRYPOUCH_NA)
            sBerryPouchPosition.type = type;
        if (allowSelect != 0xFF)
            sBerryPouchPosition.allowSelect = allowSelect;
        if (savedCallback != NULL)
            sBerryPouchPosition.savedCallback = savedCallback;
        sBerryPouch->exitCallback = NULL;
        sBerryPouch->iconSpriteId = 0;
        sBerryPouch->indicatorTaskId = 0xFF;
        for (i = 0; i < 4; i++)
            sBerryPouch->data[i] = 0;
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
        sBerryPouch->data[0] = 0;
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
        SortAndCountBerries();
        SanitizeListMenuSelectionParams();
        UpdateListMenuScrollOffset();
        gMain.state++;
        break;
    case 11:
        if (!AllocateListMenuBuffers())
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
        taskId = CreateTask(Task_BerryPouchMain, 0);
        gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sBerryPouchPosition.listMenuScrollOffset, sBerryPouchPosition.listMenuSelectedRow);
        gTasks[taskId].data[8] = 0;
        gMain.state++;
        break;
    case 15:
        CreateBerryPouchSprite();
        gMain.state++;
        break;
    case 16:
        CreateScrollIndicatorArrows_BerryPouchList();
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
        SetMainCallback2(sBerryPouchPosition.savedCallback);
        BerryPouch_DestroyResources();
        DestroyTask(taskId);
    }
}

static void BerryPouchInitBgs(void)
{
    ResetVramOamAndBgCntRegs();
    ResetAllBgsCoordinates();
    memset(sBerryPouch->bg1TilemapBuffer, 0, BG_SCREEN_SIZE);
    ResetBgsAndClearDma3BusyFlags(FALSE);
    InitBgsFromTemplates(0, sBgTemplates, NELEMS(sBgTemplates));
    SetBgTilemapBuffer(1, sBerryPouch->bg1TilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_MODE_0 | DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
}

static bool8 BerryPouchLoadGfx(void)
{
    switch (sBerryPouch->data[0])
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, gBerryPouchBgGfx, 0, 0, 0);
        sBerryPouch->data[0]++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(gBerryPouchBg1Tilemap, sBerryPouch->bg1TilemapBuffer);
            sBerryPouch->data[0]++;
        }
        break;
    case 2:
        LoadCompressedPalette(gBerryPouchBgPals, 0, 0x60);
        if (gSaveBlock2Ptr->playerGender != MALE)
            LoadCompressedPalette(gBerryPouchBgPal0FemaleOverride, 0, 0x20);
        sBerryPouch->data[0]++;
        break;
    case 3:
        LoadCompressedSpriteSheet(&sBerryPouchSpriteSheet);
        sBerryPouch->data[0]++;
        break;
    default:
        LoadCompressedSpritePalette(&sBerryPouchSpritePal);
        sBerryPouch->data[0] = 0;
        return TRUE;
    }

    return FALSE;
}

static bool8 AllocateListMenuBuffers(void)
{
    sListMenuItems = Alloc(LAST_BERRY_INDEX * sizeof(struct ListMenuItem));
    if (sListMenuItems == NULL)
        return FALSE;
    sListMenuStrbuf = Alloc(sBerryPouch->listMenuNumItems * 27);
    if (sListMenuStrbuf == NULL)
        return FALSE;
    return TRUE;
}

static void SetUpListMenuTemplate(void)
{
    u16 i;
    struct BagPocket *pocket = &gBagPockets[POCKET_BERRIES - 1];
    for (i = 0; i < sBerryPouch->listMenuNumItems; i++)
    {
        GetBerryNameAndIndexForMenu(&sListMenuStrbuf[i * 27], pocket->itemSlots[i].itemId);
        sListMenuItems[i].name = &sListMenuStrbuf[i * 27];
        sListMenuItems[i].id = i;
    }
    sListMenuItems[i].name = gText_Close;
    sListMenuItems[i].id = i;
    gMultiuseListMenuTemplate.items = sListMenuItems;
    if (sBerryPouchPosition.type != BERRYPOUCH_FROMBERRYCRUSH)
        gMultiuseListMenuTemplate.totalItems = sBerryPouch->listMenuNumItems + 1;
    else
        gMultiuseListMenuTemplate.totalItems = sBerryPouch->listMenuNumItems;
    gMultiuseListMenuTemplate.windowId = 0;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 9;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 2;
    gMultiuseListMenuTemplate.upText_Y = 2;
    gMultiuseListMenuTemplate.maxShowed = sBerryPouch->listMenuMaxShowed;
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
    StringAppend(gStringVar4, sText_Space);
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

    if (sBerryPouch->listMenuNumItems != itemIndex)
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
    if (itemId != -2 && sBerryPouch->listMenuNumItems != itemId)
    {
        unused = BagGetItemIdByPocketPosition(POCKET_BERRIES, itemId);
        itemQuantity = BagGetQuantityByPocketPosition(POCKET_BERRIES, itemId);
        ConvertIntToDecimalStringN(gStringVar1, itemQuantity, STR_CONV_MODE_RIGHT_ALIGN, 3);
        StringExpandPlaceholders(gStringVar4, gText_xVar1);
        BerryPouchPrint(windowId, FONT_SMALL, gStringVar4, 110, y, 0, 0, 0xFF, 1);
    }
}

static void BerryPouchSetArrowCursorFromListMenu(u8 taskId, u8 colorIdx)
{
    BerryPouchSetArrowCursorAt(ListMenuGetYCoordForPrintingArrowCursor(taskId), colorIdx);
}

static void BerryPouchSetArrowCursorAt(u8 y, u8 colorIdx)
{
    u8 width;
    u8 height;
    if (colorIdx == 0xFF)
    {
        width = GetMenuCursorDimensionByFont(FONT_NORMAL, 0);
        height = GetMenuCursorDimensionByFont(FONT_NORMAL, 1);
        FillWindowPixelRect(0, 0, 1, y, width, height);
        CopyWindowToVram(0, COPYWIN_GFX);
    }
    else
    {
        BerryPouchPrint(0, FONT_NORMAL, gText_SelectorArrow2, 1, y, 0, 0, 0, colorIdx);
    }
}

const u8 gText_TheBerryPouchWillBePutAway[] = _("The Berry Pouch will be\nput away.");

static void PrintSelectedBerryDescription(s32 itemIdx)
{
    const u8 * str;
    if (itemIdx != sBerryPouch->listMenuNumItems)
        str = ItemId_GetDescription(BagGetItemIdByPocketPosition(POCKET_BERRIES, itemIdx));
    else
        str = gText_TheBerryPouchWillBePutAway;
    FillWindowPixelBuffer(1, PIXEL_FILL(0));
    BerryPouchPrint(1, FONT_SHORT, str, 0, 2, 2, 0, 0, 0);
}

static void SetDescriptionWindowBorderPalette(s32 pal)
{
    SetBgTilemapPalette(1, 0, 16, 30, 4, pal + 1);
    ScheduleBgCopyTilemapToVram(1);
}

static void CreateScrollIndicatorArrows_BerryPouchList(void)
{
    if (sBerryPouchPosition.type != BERRYPOUCH_FROMBERRYCRUSH)
        sBerryPouch->indicatorTaskId = AddScrollIndicatorArrowPairParameterized(2, 160, 8, 120, sBerryPouch->listMenuNumItems - sBerryPouch->listMenuMaxShowed + 1, 110, 110, &sBerryPouchPosition.listMenuScrollOffset);
    else
        sBerryPouch->indicatorTaskId = AddScrollIndicatorArrowPairParameterized(2, 160, 8, 120, sBerryPouch->listMenuNumItems - sBerryPouch->listMenuMaxShowed, 110, 110, &sBerryPouchPosition.listMenuScrollOffset);
}

static void CreateScrollIndicatorArrows_TossQuantity(void)
{
    sBerryPouch->indicatorOffset = 1;
    sBerryPouch->indicatorTaskId = AddScrollIndicatorArrowPairParameterized(2, 212, 120, 152, 2, 110, 110, &sBerryPouch->indicatorOffset);
}

static void CreateScrollIndicatorArrows_SellQuantity(void)
{
    sBerryPouch->indicatorOffset = 1;
    sBerryPouch->indicatorTaskId = AddScrollIndicatorArrowPairParameterized(2, 152, 72, 104, 2, 110, 110, &sBerryPouch->indicatorOffset);
}

static void DestroyScrollIndicatorArrows(void)
{
    if (sBerryPouch->indicatorTaskId != 0xFF)
    {
        RemoveScrollIndicatorArrowPair(sBerryPouch->indicatorTaskId);
        sBerryPouch->indicatorTaskId = 0xFF;
    }
}

static const u8 gText_BerryPouch[] = _("Berry Pouch");

static void PrintBerryPouchHeaderCentered(void)
{
    u32 slack = 72 - GetStringWidth(FONT_NORMAL, gText_BerryPouch, 0);
    BerryPouchPrint(2, FONT_NORMAL, gText_BerryPouch, slack / 2, 1, 0, 0, 0, 0);
}

void BerryPouch_CursorResetToTop(void)
{
    sBerryPouchPosition.listMenuSelectedRow = 0;
    sBerryPouchPosition.listMenuScrollOffset = 0;
}

static void SanitizeListMenuSelectionParams(void)
{
    s32 r2;
    if (sBerryPouchPosition.type != BERRYPOUCH_FROMBERRYCRUSH)
        r2 = sBerryPouch->listMenuNumItems + 1;
    else
        r2 = sBerryPouch->listMenuNumItems;
    if (sBerryPouchPosition.listMenuScrollOffset != 0 && sBerryPouchPosition.listMenuScrollOffset + sBerryPouch->listMenuMaxShowed > r2)
        sBerryPouchPosition.listMenuScrollOffset = r2 - sBerryPouch->listMenuMaxShowed;
    if (sBerryPouchPosition.listMenuScrollOffset + sBerryPouchPosition.listMenuSelectedRow >= r2)
    {
        if (r2 == 0 || r2 == 1)
            sBerryPouchPosition.listMenuSelectedRow = 0;
        else
            sBerryPouchPosition.listMenuSelectedRow = r2 - 1;
    }
}

static void UpdateListMenuScrollOffset(void)
{
    u8 lim;
    u8 i;
    if (sBerryPouchPosition.type != BERRYPOUCH_FROMBERRYCRUSH)
        lim = sBerryPouch->listMenuNumItems + 1;
    else
        lim = sBerryPouch->listMenuNumItems;
    if (sBerryPouchPosition.listMenuSelectedRow > 4)
    {
        for (i = 0; i <= sBerryPouchPosition.listMenuSelectedRow - 4; sBerryPouchPosition.listMenuSelectedRow--, sBerryPouchPosition.listMenuScrollOffset++, i++)
        {
            if (sBerryPouchPosition.listMenuScrollOffset + sBerryPouch->listMenuMaxShowed == lim)
                break;
        }
    }
}

static void BerryPouch_DestroyResources(void)
{
    if (sBerryPouch != NULL)
        Free(sBerryPouch);
    if (sListMenuItems != NULL)
        Free(sListMenuItems);
    if (sListMenuStrbuf != NULL)
        Free(sListMenuStrbuf);
    FreeAllWindowBuffers();
}

void BerryPouch_StartFadeToExitCallback(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_BerryPouchFadeToExitCallback;
}

static void Task_BerryPouchFadeToExitCallback(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    if (!gPaletteFade.active)
    {
        DestroyListMenuTask(data[0], &sBerryPouchPosition.listMenuScrollOffset, &sBerryPouchPosition.listMenuSelectedRow);
        if (sBerryPouch->exitCallback != NULL)
            SetMainCallback2(sBerryPouch->exitCallback);
        else
            SetMainCallback2(sBerryPouchPosition.savedCallback);
        DestroyScrollIndicatorArrows();
        BerryPouch_DestroyResources();
        DestroyTask(taskId);
    }
}

static void SortAndCountBerries(void)
{
    u16 i;
    u32 r2;
    struct BagPocket *pocket = &gBagPockets[POCKET_BERRIES - 1];
    SortBerriesOrTMHMs(pocket);
    sBerryPouch->listMenuNumItems = 0;
    for (i = 0; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i].itemId == ITEM_NONE)
            break;
        sBerryPouch->listMenuNumItems++;
    }
    if (sBerryPouchPosition.type != BERRYPOUCH_FROMBERRYCRUSH)
        r2 = sBerryPouch->listMenuNumItems + 1;
    else
        r2 = sBerryPouch->listMenuNumItems;
    if (r2 > 7)
        sBerryPouch->listMenuMaxShowed = 7;
    else
        sBerryPouch->listMenuMaxShowed = r2;
}

void BerryPouch_SetExitCallback(void (*callback)(void))
{
    sBerryPouch->exitCallback = callback;
}

void InitTossQuantitySelectUI(u8 taskId, const u8 * str)
{
    s16 * data = gTasks[taskId].data;
    u8 windowId = GetOrCreateVariableWindow(8);
    u8 windowId2;
    CopySelectedListMenuItemName(data[1], gStringVar1);
    StringExpandPlaceholders(gStringVar4, str);
    BerryPouchPrint(windowId, FONT_NORMAL, gStringVar4, 0, 2, 1, 2, 0, 1);
    windowId2 = GetOrCreateVariableWindow(0);
    ConvertIntToDecimalStringN(gStringVar1, 1, STR_CONV_MODE_LEADING_ZEROS, 3);
    StringExpandPlaceholders(gStringVar4, gText_xVar1);
    BerryPouchPrint(windowId2, FONT_SMALL, gStringVar4, 4, 10, 1, 0, 0, 1);
}

static void PrintxQuantityOnWindow(u8 whichWindow, s16 quantity, u8 ndigits)
{
    u8 windowId = GetVariableWindowId(whichWindow);
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    ConvertIntToDecimalStringN(gStringVar1, quantity, STR_CONV_MODE_LEADING_ZEROS, ndigits);
    StringExpandPlaceholders(gStringVar4, gText_xVar1);
    BerryPouchPrint(windowId, FONT_SMALL, gStringVar4, 4, 10, 1, 0, 0, 1);
}

static void Task_BerryPouchMain(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    s32 menuInput;
    if (!gPaletteFade.active && MenuHelpers_ShouldWaitForLinkRecv() != TRUE)
    {
        menuInput = ListMenu_ProcessInput(data[0]);
        ListMenuGetScrollAndRow(data[0], &sBerryPouchPosition.listMenuScrollOffset, &sBerryPouchPosition.listMenuSelectedRow);
        if (JOY_NEW(SELECT_BUTTON) && sBerryPouchPosition.allowSelect == 1)
        {
            PlaySE(SE_SELECT);
            gSpecialVar_ItemId = 0;
            BerryPouch_StartFadeToExitCallback(taskId);
        }
        else
        {
            switch (menuInput)
            {
            case -1:
                return;
            case -2:
                if (sBerryPouchPosition.type != BERRYPOUCH_FROMBERRYCRUSH)
                {
                    PlaySE(SE_SELECT);
                    gSpecialVar_ItemId = 0;
                    BerryPouch_StartFadeToExitCallback(taskId);
                }
                break;
            default:
                PlaySE(SE_SELECT);
                if (sBerryPouchPosition.type == BERRYPOUCH_FROMBERRYCRUSH)
                {
                    gSpecialVar_ItemId = BagGetItemIdByPocketPosition(POCKET_BERRIES, menuInput);
                    BerryPouch_StartFadeToExitCallback(taskId);
                }
                else if (menuInput == sBerryPouch->listMenuNumItems)
                {
                    gSpecialVar_ItemId = 0;
                    BerryPouch_StartFadeToExitCallback(taskId);
                }
                else
                {
                    DestroyScrollIndicatorArrows();
                    SetDescriptionWindowBorderPalette(1);
                    BerryPouchSetArrowCursorFromListMenu(data[0], 2);
                    data[1] = menuInput;
                    data[2] = BagGetQuantityByPocketPosition(POCKET_BERRIES, menuInput);
                    gSpecialVar_ItemId = BagGetItemIdByPocketPosition(POCKET_BERRIES, menuInput);
                    gTasks[taskId].func = sBerryPouchContextMenuTasks[sBerryPouchPosition.type];
                }
                break;
            }
        }
    }
}

static void Task_CleanUpAndReturnToMain(u8 taskId)
{
    SetDescriptionWindowBorderPalette(0);
    CreateScrollIndicatorArrows_BerryPouchList();
    gTasks[taskId].func = Task_BerryPouchMain;
}

static void CreateNormalContextMenu(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    u8 windowId;
    u8 windowId2;

    if (sBerryPouchPosition.type == BERRYPOUCH_FROMBATTLE)
    {
        sContextMenuOptions = sOptions_UseToss_Exit;
        sContextMenuNumOptions = 3;
    }
    else if (MenuHelpers_IsLinkActive() == TRUE)
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
    windowId = GetOrCreateVariableWindow(sContextMenuNumOptions + 9);
    PrintMenuActionTexts(windowId, FONT_SHORT, GetMenuCursorDimensionByFont(FONT_SHORT, 0), 2, GetFontAttribute(FONT_SHORT, FONTATTR_LETTER_SPACING), GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_HEIGHT) + 2, sContextMenuNumOptions, sContextMenuActions, sContextMenuOptions);
    InitMenuNormal(windowId, FONT_SHORT, 0, 2, GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_HEIGHT) + 2, sContextMenuNumOptions, 0);
    windowId2 = GetOrCreateVariableWindow(6);
    CopySelectedListMenuItemName(data[1], gStringVar1);
    StringExpandPlaceholders(gStringVar4, gText_Var1IsSelected);
    BerryPouchPrint(windowId2, FONT_SHORT, gStringVar4, 0, 2, 1, 1, 0, 1);
}

static void Task_NormalContextMenu(u8 taskId)
{
    CreateNormalContextMenu(taskId);
    gTasks[taskId].func = Task_NormalContextMenu_HandleInput;
}

static void Task_NormalContextMenu_HandleInput(u8 taskId)
{
    s8 input;
    if (MenuHelpers_ShouldWaitForLinkRecv() != TRUE)
    {
        input = Menu_ProcessInputNoWrap();
        switch (input)
        {
        case -2:
            break;
        case -1:
            PlaySE(SE_SELECT);
            sContextMenuActions[BP_ACTION_EXIT].func.void_u8(taskId);
            break;
        default:
            PlaySE(SE_SELECT);
            sContextMenuActions[sContextMenuOptions[input]].func.void_u8(taskId);
            break;
        }
    }
}

static void Task_BerryPouch_Use(u8 taskId)
{
    DestroyVariableWindow(sContextMenuNumOptions + 9);
    DestroyVariableWindow(6);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(2);
    if (sBerryPouchPosition.type == BERRYPOUCH_FROMBATTLE)
    {
        if (ItemId_GetBattleFunc(gSpecialVar_ItemId) == NULL)
            ItemUseOutOfBattle_CannotUse(taskId);
        else
            ItemId_GetBattleFunc(gSpecialVar_ItemId)(taskId);
    }
    else if (CalculatePlayerPartyCount() == 0 && ItemId_GetType(gSpecialVar_ItemId) == ITEM_USE_PARTY_MENU)
        Task_Give_PrintThereIsNoPokemon(taskId);
    else
        ItemId_GetFieldFunc(gSpecialVar_ItemId)(taskId);
}

static void Task_BerryPouch_Toss(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    ClearWindowTilemap(GetVariableWindowId(sContextMenuNumOptions + 9));
    ClearWindowTilemap(GetVariableWindowId(6));
    DestroyVariableWindow(sContextMenuNumOptions + 9);
    DestroyVariableWindow(6);
    PutWindowTilemap(0);
    data[8] = 1;
    if (data[2] == 1)
        Task_AskTossMultiple(taskId);
    else
    {
        InitTossQuantitySelectUI(taskId, gText_TossHowManyVar1s);
        CreateScrollIndicatorArrows_TossQuantity();
        gTasks[taskId].func = Task_Toss_SelectMultiple;
    }
}

static void Task_AskTossMultiple(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    ConvertIntToDecimalStringN(gStringVar2, data[8], STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, gText_ThrowAwayVar1);
    BerryPouchPrint(GetOrCreateVariableWindow(7), FONT_NORMAL, gStringVar4, 0, 2, 1, 2, 0, 1);
    CreateYesNoMenuWin3(taskId, &sYesNoFuncs_Toss);
}

static void Task_TossNo(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    DestroyVariableWindow(7);
    PutWindowTilemap(1);
    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(2);
    BerryPouchSetArrowCursorFromListMenu(data[0], 1);
    Task_CleanUpAndReturnToMain(taskId);
}

static void Task_Toss_SelectMultiple(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    if (AdjustQuantityAccordingToDPadInput(&data[8], data[2]) == TRUE)
        PrintxQuantityOnWindow(0, data[8], 3);
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        ClearWindowTilemap(GetVariableWindowId(8));
        DestroyVariableWindow(8);
        DestroyVariableWindow(0);
        ScheduleBgCopyTilemapToVram(0);
        ScheduleBgCopyTilemapToVram(2);
        DestroyScrollIndicatorArrows();
        Task_AskTossMultiple(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyVariableWindow(8);
        DestroyVariableWindow(0);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        ScheduleBgCopyTilemapToVram(2);
        BerryPouchSetArrowCursorFromListMenu(data[0], 1);
        DestroyScrollIndicatorArrows();
        Task_CleanUpAndReturnToMain(taskId);
    }
}

static void Task_TossYes(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    DestroyVariableWindow(7);
    CopySelectedListMenuItemName(data[1], gStringVar1);
    ConvertIntToDecimalStringN(gStringVar2, data[8], STR_CONV_MODE_LEFT_ALIGN, 3);
    StringExpandPlaceholders(gStringVar4, gText_ThrewAwayVar2Var1s);
    BerryPouchPrint(GetOrCreateVariableWindow(9), FONT_NORMAL, gStringVar4, 0, 2, 1, 2, 0, 1);
    gTasks[taskId].func = Task_WaitButtonThenTossBerries;
}

static void Task_WaitButtonThenTossBerries(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        RemoveBagItem(gSpecialVar_ItemId, data[8]);
        DestroyVariableWindow(9);
        DestroyListMenuTask(data[0], &sBerryPouchPosition.listMenuScrollOffset, &sBerryPouchPosition.listMenuSelectedRow);
        SortAndCountBerries();
        SanitizeListMenuSelectionParams();
        SetUpListMenuTemplate();
        data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sBerryPouchPosition.listMenuScrollOffset, sBerryPouchPosition.listMenuSelectedRow);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        BerryPouchSetArrowCursorFromListMenu(data[0], 1);
        Task_CleanUpAndReturnToMain(taskId);
    }
}

static void Task_BerryPouch_Give(u8 taskId)
{
    DestroyVariableWindow(sContextMenuNumOptions + 9);
    DestroyVariableWindow(6);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(2);
    if (CalculatePlayerPartyCount() == 0)
        Task_Give_PrintThereIsNoPokemon(taskId);
    else
    {
        sBerryPouch->exitCallback = CB2_ChooseMonToGiveItem;
        gTasks[taskId].func = BerryPouch_StartFadeToExitCallback;
    }
}

static void Task_Give_PrintThereIsNoPokemon(u8 taskId)
{
    DisplayItemMessageInBerryPouch(taskId, FONT_NORMAL, gText_NoPokemon, Task_WaitButtonBeforeDialogueWindowDestruction);
}

static void Task_WaitButtonBeforeDialogueWindowDestruction(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        Task_BerryPouch_DestroyDialogueWindowAndRefreshListMenu(taskId);
    }
}

void Task_BerryPouch_DestroyDialogueWindowAndRefreshListMenu(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    TryDestroyVariableWindow(5);
    DestroyListMenuTask(data[0], &sBerryPouchPosition.listMenuScrollOffset, &sBerryPouchPosition.listMenuSelectedRow);
    SortAndCountBerries();
    SanitizeListMenuSelectionParams();
    SetUpListMenuTemplate();
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sBerryPouchPosition.listMenuScrollOffset, sBerryPouchPosition.listMenuSelectedRow);
    ScheduleBgCopyTilemapToVram(0);
    BerryPouchSetArrowCursorFromListMenu(data[0], 1);
    Task_CleanUpAndReturnToMain(taskId);
}

static void Task_BerryPouch_Exit(u8 taskId)
{
    DestroyVariableWindow(sContextMenuNumOptions + 9);
    DestroyVariableWindow(6);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(2);
    BerryPouchSetArrowCursorFromListMenu(gTasks[taskId].data[0], 1);
    Task_CleanUpAndReturnToMain(taskId);
}

static void Task_ContextMenu_FromPartyGiveMenu(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    u16 itemId = BagGetItemIdByPocketPosition(POCKET_BERRIES, data[1]);
    if (!IsHoldingItemAllowed(itemId))
    {
        CopyItemName(itemId, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_Var1CantBeHeldHere);
        DisplayItemMessageInBerryPouch(taskId, FONT_NORMAL, gStringVar4, Task_WaitButtonBeforeDialogueWindowDestruction);
    }
    else
    {
        sBerryPouch->exitCallback = CB2_GiveHoldItem;
        gTasks[taskId].func = BerryPouch_StartFadeToExitCallback;
    }
}

static void Task_ContextMenu_FromPokemonPC(u8 taskId)
{
    sBerryPouch->exitCallback = CB2_ReturnToPokeStorage;
    gTasks[taskId].func = BerryPouch_StartFadeToExitCallback;
}

static void Task_ContextMenu_Sell(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    if (ItemId_GetPrice(gSpecialVar_ItemId) == 0)
    {
        CopyItemName(gSpecialVar_ItemId, gStringVar1);
        StringExpandPlaceholders(gStringVar4, gText_CantBuyKeyItem);
        DisplayItemMessageInBerryPouch(taskId, FONT_NORMAL, gStringVar4, Task_BerryPouch_DestroyDialogueWindowAndRefreshListMenu);
    }
    else
    {
        data[8] = 1;
        if (data[2] == 1)
        {
            PrintMoneyInWin2();
            Task_AskSellMultiple(taskId);
        }
        else
        {
            if (data[2] > 99)
                data[2] = 99;
            CopyItemName(gSpecialVar_ItemId, gStringVar1);
            StringExpandPlaceholders(gStringVar4, gText_HowManyToSell);
            DisplayItemMessageInBerryPouch(taskId, FONT_NORMAL, gStringVar4, Task_Sell_PrintSelectMultipleUI);
        }
    }
}

static void Task_AskSellMultiple(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    ConvertIntToDecimalStringN(gStringVar3, ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_BERRIES, data[1])) / 2 * data[8], STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, gText_ICanPayVar1);
    DisplayItemMessageInBerryPouch(taskId, FONT_NORMAL, gStringVar4, Task_SellMultiple_CreateYesNoMenu);
}

static void Task_SellMultiple_CreateYesNoMenu(u8 taskId)
{
    CreateYesNoMenuWin4(taskId, &sYesNoFuncs_Sell);
}

static void Task_SellNo(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    DestroyVariableWindow(2);
    TryDestroyVariableWindow(5);
    PutWindowTilemap(2);
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    ScheduleBgCopyTilemapToVram(0);
    BerryPouchSetArrowCursorFromListMenu(data[0], 1);
    Task_CleanUpAndReturnToMain(taskId);
}

static void Task_Sell_PrintSelectMultipleUI(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    u8 windowId = GetOrCreateVariableWindow(1);
    ConvertIntToDecimalStringN(gStringVar1, 1, STR_CONV_MODE_LEADING_ZEROS, 2);
    StringExpandPlaceholders(gStringVar4, gText_xVar1);
    BerryPouchPrint(windowId, FONT_SMALL, gStringVar4, 4, 10, 1, 0, 0xFF, 1);
    SellMultiple_UpdateSellPriceDisplay(ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_BERRIES, data[1])) / 2 * data[8]);
    PrintMoneyInWin2();
    CreateScrollIndicatorArrows_SellQuantity();
    gTasks[taskId].func = Task_Sell_SelectMultiple;
}

static void SellMultiple_UpdateSellPriceDisplay(s32 price)
{
    PrintMoneyAmount(GetVariableWindowId(1), 56, 10, price, 0);
}

static void Task_Sell_SelectMultiple(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    if (AdjustQuantityAccordingToDPadInput(&data[8], data[2]) == TRUE)
    {
        PrintxQuantityOnWindow(1, data[8], 2);
        SellMultiple_UpdateSellPriceDisplay(ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_BERRIES, data[1])) / 2 * data[8]);
    }
    else if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyVariableWindow(1);
        PutWindowTilemap(0);
        ScheduleBgCopyTilemapToVram(0);
        DestroyScrollIndicatorArrows();
        Task_AskSellMultiple(taskId);
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyVariableWindow(1);
        DestroyVariableWindow(2);
        TryDestroyVariableWindow(5);
        PutWindowTilemap(2);
        PutWindowTilemap(0);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        DestroyScrollIndicatorArrows();
        BerryPouchSetArrowCursorFromListMenu(data[0], 1);
        Task_CleanUpAndReturnToMain(taskId);
    }
}

static void Task_SellYes(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    PutWindowTilemap(0);
    ScheduleBgCopyTilemapToVram(0);
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    ConvertIntToDecimalStringN(gStringVar3, ItemId_GetPrice(BagGetItemIdByPocketPosition(POCKET_BERRIES, data[1])) / 2 * data[8], STR_CONV_MODE_LEFT_ALIGN, 6);
    StringExpandPlaceholders(gStringVar4, gText_TurnedOverVar1ForVar2);
    DisplayItemMessageInBerryPouch(taskId, FONT_NORMAL, gStringVar4, Task_SellBerries_PlaySfxAndRemoveBerries);
}

static void Task_SellBerries_PlaySfxAndRemoveBerries(u8 taskId)
{
    s16 * data = gTasks[taskId].data;
    PlaySE(SE_SHOP);
    RemoveBagItem(gSpecialVar_ItemId, data[8]);
    AddMoney(&gSaveBlock1Ptr->money, ItemId_GetPrice(gSpecialVar_ItemId) / 2 * data[8]);
    // RecordItemPurchase(gSpecialVar_ItemId, data[8], 2);
    DestroyListMenuTask(data[0], &sBerryPouchPosition.listMenuScrollOffset, &sBerryPouchPosition.listMenuSelectedRow);
    SortAndCountBerries();
    SanitizeListMenuSelectionParams();
    SetUpListMenuTemplate();
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sBerryPouchPosition.listMenuScrollOffset, sBerryPouchPosition.listMenuSelectedRow);
    BerryPouchSetArrowCursorFromListMenu(data[0], 2);
    PrintMoneyAmountInMoneyBox(GetVariableWindowId(2), GetMoney(&gSaveBlock1Ptr->money), 0);
    gTasks[taskId].func = Task_SellBerries_WaitButton;
}

static void Task_SellBerries_WaitButton(u8 taskId)
{
    if (JOY_NEW(A_BUTTON) || JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        DestroyVariableWindow(2);
        PutWindowTilemap(2);
        Task_BerryPouch_DestroyDialogueWindowAndRefreshListMenu(taskId);
    }
}

static void BerryPouchInitWindows(void)
{
    u8 i;
    InitWindows(sWindowTemplates_Main);
    DeactivateAllTextPrinters();
    LoadUserWindowBorderGfx(0, 1, 0xE0);
    LoadMessageBoxGfx(0, 0x013, 0xD0);
    // LoadWindowGfx(0, 0x00A, 0xC0);
    LoadPalette(gStandardMenuPalette, 0xF0, 0x20);
    for (i = 0; i < 3; i++)
        FillWindowPixelBuffer(i, PIXEL_FILL(0));
    PutWindowTilemap(0);
    PutWindowTilemap(1);
    PutWindowTilemap(2);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(2);
    for (i = 0; i < 14; i++)
        sVariableWindowIds[i] = 0xFF;
}

static void BerryPouchPrint(u8 windowId, u8 fontId, const u8 * str, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx)
{
    AddTextPrinterParameterized4(windowId, fontId, x, y, letterSpacing, lineSpacing, sTextColors[colorIdx], speed, str);
}

static u8 GetOrCreateVariableWindow(u8 winIdx)
{
    u8 retval = sVariableWindowIds[winIdx];
    if (retval == 0xFF)
    {
        sVariableWindowIds[winIdx] = AddWindow(&sWindowTemplates_Variable[winIdx]);
        DrawStdFrameWithCustomTileAndPalette(sVariableWindowIds[winIdx], FALSE, 1, 14);
        ScheduleBgCopyTilemapToVram(2);
        retval = sVariableWindowIds[winIdx];
    }
    return retval;
}

static void DestroyVariableWindow(u8 winIdx)
{
    ClearStdWindowAndFrameToTransparent(sVariableWindowIds[winIdx], FALSE);
    ClearWindowTilemap(sVariableWindowIds[winIdx]);
    RemoveWindow(sVariableWindowIds[winIdx]);
    ScheduleBgCopyTilemapToVram(2);
    sVariableWindowIds[winIdx] = 0xFF;
}

static void TryDestroyVariableWindow(u8 winIdx)
{
    if (sVariableWindowIds[winIdx] != 0xFF)
    {
        ClearDialogWindowAndFrameToTransparent(sVariableWindowIds[winIdx], FALSE);
        ClearWindowTilemap(sVariableWindowIds[winIdx]);
        RemoveWindow(sVariableWindowIds[winIdx]);
        PutWindowTilemap(1);
        ScheduleBgCopyTilemapToVram(0);
        ScheduleBgCopyTilemapToVram(2);
        sVariableWindowIds[winIdx] = 0xFF;
    }
}

static u8 GetVariableWindowId(u8 winIdx)
{
    return sVariableWindowIds[winIdx];
}

void DisplayItemMessageInBerryPouch(u8 taskId, u8 fontId, const u8 * str, TaskFunc followUpFunc)
{
    if (sVariableWindowIds[5] == 0xFF)
        sVariableWindowIds[5] = AddWindow(&sWindowTemplates_Variable[5]);
    DisplayMessageAndContinueTask(taskId, sVariableWindowIds[5], 0x1, 0xE, fontId, GetPlayerTextSpeedDelay(), str, followUpFunc);
    ScheduleBgCopyTilemapToVram(2);
}

static void CreateYesNoMenuWin3(u8 taskId, const struct YesNoFuncTable *ptrs)
{
    CreateYesNoMenuWithCallbacks(taskId, &sWindowTemplates_Variable[3], FONT_NORMAL, 0, 2, 0x001, 0xE, ptrs);
}

static void CreateYesNoMenuWin4(u8 taskId, const struct YesNoFuncTable *ptrs)
{
    CreateYesNoMenuWithCallbacks(taskId, &sWindowTemplates_Variable[4], FONT_NORMAL, 0, 2, 0x001, 0xE, ptrs);
}

static void PrintMoneyInWin2(void)
{
    PrintMoneyAmountInMoneyBoxWithBorder(GetOrCreateVariableWindow(2), 1, 14, GetMoney(&gSaveBlock1Ptr->money));
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
