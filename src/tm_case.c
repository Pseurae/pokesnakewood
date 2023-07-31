#include "global.h"
#include "tm_case.h"
#include "bg.h"
#include "palette.h"
#include "gpu_regs.h"
#include "dma3.h"
#include "malloc.h"
#include "sound.h"
#include "text.h"
#include "sprite.h"
#include "window.h"
#include "blit.h"
#include "string_util.h"
#include "decompress.h"
#include "graphics.h"
#include "task.h"
#include "text_window.h"
#include "menu.h"
#include "menu_helpers.h"
#include "list_menu.h"
#include "item.h"
#include "item_menu.h"
#include "menu_helpers.h"
#include "link.h"
#include "money.h"
#include "shop.h"
#include "pokemon_storage_system.h"
#include "party_menu.h"
#include "data.h"
#include "scanline_effect.h"
#include "strings.h"
#include "list_menu.h"
#include "constants/items.h"
#include "constants/songs.h"
#include "constants/rgb.h"

static const u32 sTMCaseTiles[] = INCBIN_U32("graphics/tm_case/tiles.4bpp.lz");
static const u32 sTMCaseMenu_Tilemap[] = INCBIN_U32("graphics/tm_case/menu.bin.lz");
static const u32 sTMCase_Tilemap[] = INCBIN_U32("graphics/tm_case/case.bin.lz");
static const u32 sTMCaseMale_Pal[] = INCBIN_U32("graphics/tm_case/male.gbapal.lz");
static const u32 sTMCaseFemale_Pal[] = INCBIN_U32("graphics/tm_case/female.gbapal.lz");
static const u32 sTMDisc_Gfx[] = INCBIN_U32("graphics/tm_case/disk.4bpp.lz");
static const u32 sTMCaseTypes1_Pal[] = INCBIN_U32("graphics/tm_case/types1.gbapal.lz");
static const u32 sTMCaseTypes2_Pal[] = INCBIN_U32("graphics/tm_case/types2.gbapal.lz");
static const u8 sTMCaseHMIcon_Gfx[] = INCBIN_U8("graphics/tm_case/hm.4bpp");
static const u16 sTMCaseMainWindowPalette[] = INCBIN_U16("graphics/tm_case/window.gbapal");

static const u8 sText_ClearTo18[] = _("{CLEAR_TO 18}");
static const u8 sText_TMCaseWillBePutAway[] = _("The TM Case will be\nput away.");

static EWRAM_DATA struct TMCaseControl sTMCaseStaticResources = {};
static EWRAM_DATA struct TMCaseData * sTMCaseDynamicResources = NULL;
static EWRAM_DATA void *sTilemapBuffer = NULL; // tilemap buffer
static EWRAM_DATA struct ListMenuItem * sListMenuItemsBuffer = NULL;
static EWRAM_DATA u8 (* sListMenuStringsBuffer)[29] = NULL;
static EWRAM_DATA u16 * sTMSpritePaletteBuffer = NULL;

static void CB2_InitTMCase(void);
static bool8 InitTMCase(void);
static void LoadBgTemplates(void);
static bool8 HandleLoadTMCaseGraphicsAndPalettes(void);
static void CreateTMCaseListMenuBuffers(void);
static void InitTMCaseListMenuItems(void);
static void GetTMNumberAndMoveString(u8 * dest, u16 itemId);
static void TMCase_MoveCursorCallback(s32 itemIndex, bool8 onInit, struct ListMenu *list);
static void TMCase_ItemPrintFunc(u8 windowId, u32 itemId, u8 y);
static void PrintDescription(s32 itemIndex);
static void TMCase_PrintCursorAtPos(u8 y, u8 colorIdx);
static void CreateTMCaseScrollIndicatorArrows(void);
static void TMCaseSetup_GetTMCount(void);
static void TMCaseSetup_InitListMenuPositions(void);
static void TMCaseSetup_UpdateVisualMenuOffset(void);
static void DestroyTMCaseBuffers(void);
static void Task_CloseTMCase(u8 taskId);
static void Task_TMCaseHandleInput(u8 taskId);
static void Task_ContextMenu_FromFieldBag(u8 taskId);
static void Task_TMContext_HandleInput(u8 taskId);
static void TMCaseAction_Use(u8 taskId);
static void PrintThereIsNoPokemon(u8 taskId);
static void PrintItemCantBeHeld(u8 taskId);
static void HandleErrorMessage(u8 taskId);
static void CloseTMCaseMessage(u8 taskId);
static void TMCaseAction_Cancel(u8 taskId);
static void Task_ContextMenu_FromPartyGiveMenu(u8 taskId);
static void Task_ContextMenu_FromPokemonPC(u8 taskId);
static void Task_ContextMenu_FromSellMenu(u8 taskId);
static void InitTMCaseWindows(void);
static void TMCase_Print(u8 windowId, u8 fontId, const u8 * str, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx);
static void TMCase_SetWindowBorder(u8 windowId);
static void TMCase_DisplayMessageWithCallback(u8 taskId, u8 fontId, const u8 * str, TaskFunc func);
static void PrintTMCaseLabel(void);
static void DrawMoveInfoUIMarkers(void);
static void PrintTMInfo(u16 itemId);
static void PlaceHMTileInWindow(u8 windowId, u8 x, u8 y);
static u8 AddContextWindow(u8 *windowId);
static void RemoveContextWindow(u8 *windowId);
static u8 CreateDiscSprite(u16 itemId);
static void SetTMSpriteAnim(struct Sprite *sprite, u8 var);
static void TintDiscSpriteByType(u8 type);
static void UpdateDiscSpritePosition(struct Sprite *sprite, u8 tm);
static void InitSelectedTMSpriteData(u8 a0, u16 itemId);
static void SpriteCB_MoveTMSpriteInCase(struct Sprite *sprite);
static void LoadTMTypePalettes(void);

static const struct BgTemplate sTMCaseBgTemplates[] = 
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
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0x000
    }, 
    {
        .bg = 2,
        .charBaseIndex = 0,
        .mapBaseIndex = 29,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 2,
        .baseTile = 0x000
    }
};

static const TaskFunc sSelectTMActionTasks[] = 
{
    [TMCASE_FROMFIELD] = Task_ContextMenu_FromFieldBag,
    [TMCASE_FROMPARTYGIVE] = Task_ContextMenu_FromPartyGiveMenu,
    [TMCASE_FROMMARTSELL] = Task_ContextMenu_FromSellMenu,
    [TMCASE_FROMPOKEMONSTORAGEPC] = Task_ContextMenu_FromPokemonPC
};

enum
{
    ACTION_USE,
    ACTION_EXIT
};

static const struct MenuAction sTMCaseMenuActions[] = 
{
    [ACTION_USE] =  { gMenuText_Use,  TMCaseAction_Use },
    [ACTION_EXIT] = { gText_MenuExit, TMCaseAction_Cancel },
};

static const u8 sTMCaseMenuItems_Normal[] = {
    ACTION_USE, 
    ACTION_EXIT
};

static const u8 sTMCaseMenuItems_UnionRoom[] = {
    ACTION_EXIT
};

enum 
{
    COLORID_LIGHT,
    COLORID_DARK,
    COLORID_CURSOR_SELECTED,
    COLORID_MOVE_INFO,
    COLORID_NONE = 0xFF
};

static const u8 sFontColorTable[][3] = {
    [COLORID_LIGHT] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_WHITE, TEXT_COLOR_DARK_GRAY},
    [COLORID_DARK] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY},
    [COLORID_CURSOR_SELECTED] = {TEXT_COLOR_TRANSPARENT, TEXT_COLOR_LIGHT_GRAY, TEXT_COLOR_DARK_GRAY},
    [COLORID_MOVE_INFO] = {TEXT_COLOR_TRANSPARENT, TEXT_DYNAMIC_COLOR_5, TEXT_DYNAMIC_COLOR_1}
};

enum {
    WIN_LIST,
    WIN_INFO,
    WIN_MSG,
    WIN_LABEL,
    WIN_UI_MARKERS,
    WIN_MOVE_INFO,
    WIN_CASHIER_MSG,
    WIN_QUANTITY,
    WIN_MONEYBOX,
    WIN_DUMMY
};

static const struct WindowTemplate sWindowTemplates[] = {
    [WIN_LIST] = {
        .bg = 0, 
        .tilemapLeft = 10, 
        .tilemapTop = 1, 
        .width = 19, 
        .height = 10, 
        .paletteNum = 4, 
        .baseBlock = 129
    },
    [WIN_INFO] = {
        .bg = 0, 
        .tilemapLeft = 12, 
        .tilemapTop = 12, 
        .width = 18, 
        .height = 8, 
        .paletteNum = 6, 
        .baseBlock = 319
    },
    [WIN_MSG] = {
        .bg = 1, 
        .tilemapLeft = 5, 
        .tilemapTop = 15, 
        .width = 15, 
        .height = 4, 
        .paletteNum = 4, 
        .baseBlock = 505
    },
    [WIN_LABEL] = {
        .bg = 0, 
        .tilemapLeft = 0, 
        .tilemapTop = 1, 
        .width = 10, 
        .height = 2, 
        .paletteNum = 4, 
        .baseBlock = 565
    },
    [WIN_UI_MARKERS] = {
        .bg = 0, 
        .tilemapLeft = 1, 
        .tilemapTop = 13, 
        .width = 5, 
        .height = 6, 
        .paletteNum = 7, 
        .baseBlock = 585
    },
    [WIN_MOVE_INFO] = {
        .bg = 0, 
        .tilemapLeft = 7, 
        .tilemapTop = 13, 
        .width = 5, 
        .height = 6, 
        .paletteNum = 7, 
        .baseBlock = 615
    },
    [WIN_CASHIER_MSG] = {
        .bg = 1, 
        .tilemapLeft = 2, 
        .tilemapTop = 15, 
        .width = 26, 
        .height = 4, 
        .paletteNum = 4, 
        .baseBlock = 645
    },
    [WIN_QUANTITY] = {
        .bg = 1, 
        .tilemapLeft = 17, 
        .tilemapTop = 9, 
        .width = 12, 
        .height = 4, 
        .paletteNum = 15, 
        .baseBlock = 749
    },
    [WIN_MONEYBOX] = {
        .bg = 1, 
        .tilemapLeft = 1, 
        .tilemapTop = 1, 
        .width = 10, 
        .height = 2, 
        .paletteNum = 15, 
        .baseBlock = 797
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sContextWindowTemplate = {
    .bg = 1, 
    .tilemapLeft = 24, 
    .tilemapTop = 15, 
    .width = 5, 
    .height = 4, 
    .paletteNum = 4, 
    .baseBlock = 463
};

enum {
    ANIM_TM,
    ANIM_HM
};

#define TAG_DISC 400 

static const struct OamData sOamData_TMDisc = {
    .size = 2,
    .priority = 2
};

static const union AnimCmd sAnims_TMSpriteTM[] = {
    ANIMCMD_FRAME(0, 0),
    ANIMCMD_END
};

static const union AnimCmd sAnims_TMSpriteHM[] = {
    ANIMCMD_FRAME(16, 0),
    ANIMCMD_END
};

static const union AnimCmd *const sAnims_TMDisc[] = 
{
    [ANIM_TM] = sAnims_TMSpriteTM,
    [ANIM_HM] = sAnims_TMSpriteHM
};

static const struct CompressedSpriteSheet sSpriteSheet_TMDisc = 
{
    .data = sTMDisc_Gfx,
    .size = 0x400,
    .tag = TAG_DISC
};

static const struct SpriteTemplate sSpriteTemplate_TMDisc = 
{
    .tileTag = TAG_DISC,
    .paletteTag = TAG_DISC,
    .oam = &sOamData_TMDisc,
    .anims = sAnims_TMDisc,
    .images = NULL,
    .affineAnims = gDummySpriteAffineAnimTable,
    .callback = SpriteCallbackDummy
};

static const u8 sTMSpritePaletteOffsetByType[] = 
{
    [TYPE_NORMAL]   = 0x00,
    [TYPE_FIRE]     = 0x01,
    [TYPE_WATER]    = 0x02,
    [TYPE_GRASS]    = 0x03,
    [TYPE_ELECTRIC] = 0x04,
    [TYPE_ROCK]     = 0x05,
    [TYPE_GROUND]   = 0x06,
    [TYPE_ICE]      = 0x07,
    [TYPE_FLYING]   = 0x08,
    [TYPE_FIGHTING] = 0x09,
    [TYPE_GHOST]    = 0x0a,
    [TYPE_BUG]      = 0x0b,
    [TYPE_POISON]   = 0x0c,
    [TYPE_PSYCHIC]  = 0x0d,
    [TYPE_STEEL]    = 0x0e,
    [TYPE_DARK]     = 0x0f,
    [TYPE_DRAGON]   = 0x10
};

void OpenTMCase(u8 type, void (* callback)(void), u8 a2)
{
    DestroyTMCaseBuffers();
    sTMCaseDynamicResources = Alloc(sizeof(struct TMCaseData));
    sTMCaseDynamicResources->savedCallback = NULL;
    sTMCaseDynamicResources->scrollIndicatorArrowPairId = TASK_NONE;
    sTMCaseDynamicResources->contextMenuWindowId = WINDOW_NONE;
    if (type != TMCASE_REOPENING)
        sTMCaseStaticResources.tmCaseMenuType = type;
    if (callback != NULL)
        sTMCaseStaticResources.savedCallback = callback;
    gTextFlags.autoScroll = FALSE;
    SetMainCallback2(CB2_InitTMCase);
}

static void MainCB2(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

static void CB2_InitTMCase(void)
{
    while (1)
    {
        if (MenuHelpers_ShouldWaitForLinkRecv() == TRUE)
            break;
        if (InitTMCase() == TRUE)
            break;
        if (MenuHelpers_IsLinkActive() == TRUE)
            break;
    }
}

static bool8 InitTMCase(void)
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
        ResetTasks();
        gMain.state++;
        break;
    case 6:
        LoadBgTemplates();
        sTMCaseDynamicResources->seqId = 0;
        gMain.state++;
        break;
    case 7:
        InitTMCaseWindows();
        gMain.state++;
        break;
    case 8:
        if (HandleLoadTMCaseGraphicsAndPalettes())
            gMain.state++;
        break;
    case 9:
        SortBerriesOrTMHMs(&gBagPockets[POCKET_TM_HM - 1]);
        gMain.state++;
        break;
    case 10:
        TMCaseSetup_GetTMCount();
        TMCaseSetup_InitListMenuPositions();
        TMCaseSetup_UpdateVisualMenuOffset();
        gMain.state++;
        break;
    case 11:
        DrawMoveInfoUIMarkers();
        gMain.state++;
        break;
    case 12:
        CreateTMCaseListMenuBuffers();
        InitTMCaseListMenuItems();
        gMain.state++;
        break;
    case 13:
        PrintTMCaseLabel();
        gMain.state++;
        break;
    case 14:
        taskId = CreateTask(Task_TMCaseHandleInput, 0);
        gTasks[taskId].data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sTMCaseStaticResources.scrollOffset, sTMCaseStaticResources.selectedRow);
        gMain.state++;
        break;
    case 15:
        CreateTMCaseScrollIndicatorArrows();
        gMain.state++;
        break;
    case 16:
        sTMCaseDynamicResources->tmSpriteId = CreateDiscSprite(BagGetItemIdByPocketPosition(POCKET_TM_HM, sTMCaseStaticResources.scrollOffset + sTMCaseStaticResources.selectedRow));
        gMain.state++;
        break;
    case 17:
        BlendPalettes(PALETTES_ALL, 16, 0);
        gMain.state++;
        break;
    case 18:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gMain.state++;
        break;
    default:
        SetVBlankCallback(VBlankCB);
        SetMainCallback2(MainCB2);
        return TRUE;
    }

    return FALSE;
}

static void LoadBgTemplates(void)
{
    ResetVramOamAndBgCntRegs();
    ResetAllBgsCoordinates();
    sTilemapBuffer = AllocZeroed(0x800);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sTMCaseBgTemplates, NELEMS(sTMCaseBgTemplates));
    SetBgTilemapBuffer(2, sTilemapBuffer);
    ScheduleBgCopyTilemapToVram(1);
    ScheduleBgCopyTilemapToVram(2);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_1D_MAP | DISPCNT_OBJ_ON);
    // SetGpuReg(REG_OFFSET_BLDCNT, 0);
    ShowBg(0);
    ShowBg(1);
    ShowBg(2);
}

static bool8 HandleLoadTMCaseGraphicsAndPalettes(void)
{
    switch (sTMCaseDynamicResources->seqId)
    {
    case 0:
        ResetTempTileDataBuffers();
        DecompressAndCopyTileDataToVram(1, sTMCaseTiles, 0, 0, 0);
        sTMCaseDynamicResources->seqId++;
        break;
    case 1:
        if (FreeTempTileDataBuffersIfPossible() != TRUE)
        {
            LZDecompressWram(sTMCaseMenu_Tilemap, sTilemapBuffer);
            sTMCaseDynamicResources->seqId++;
        }
        break;
    case 2:
        LZDecompressWram(sTMCase_Tilemap, GetBgTilemapBuffer(1));
        sTMCaseDynamicResources->seqId++;
        break;
    case 3:
        if (gSaveBlock2Ptr->playerGender == MALE)
            LoadCompressedPalette(sTMCaseMale_Pal, BG_PLTT_ID(0), PLTT_SIZE_4BPP * 4);
        else
            LoadCompressedPalette(sTMCaseFemale_Pal, BG_PLTT_ID(0), PLTT_SIZE_4BPP * 4);
        sTMCaseDynamicResources->seqId++;
        break;
    case 4:
        LoadCompressedSpriteSheet(&sSpriteSheet_TMDisc);
        sTMCaseDynamicResources->seqId++;
        break;
    default:
        LoadTMTypePalettes();
        sTMCaseDynamicResources->seqId = 0;
        return TRUE;
    }

    return FALSE;
}

static void CreateTMCaseListMenuBuffers(void)
{
    struct BagPocket *pocket = &gBagPockets[POCKET_TM_HM - 1];
    sListMenuItemsBuffer = Alloc((pocket->capacity + 1) * sizeof(struct ListMenuItem));
    sListMenuStringsBuffer = Alloc(sTMCaseDynamicResources->numTMs * 29);
}

static void InitTMCaseListMenuItems(void)
{
    struct BagPocket * pocket = &gBagPockets[POCKET_TM_HM - 1];
    u16 i;

    for (i = 0; i < sTMCaseDynamicResources->numTMs; i++)
    {
        GetTMNumberAndMoveString(sListMenuStringsBuffer[i], pocket->itemSlots[i].itemId);
        sListMenuItemsBuffer[i].name = sListMenuStringsBuffer[i];
        sListMenuItemsBuffer[i].id = i;
    }
    sListMenuItemsBuffer[i].name = gText_Close;
    sListMenuItemsBuffer[i].id = -2;
    gMultiuseListMenuTemplate.items = sListMenuItemsBuffer;
    gMultiuseListMenuTemplate.totalItems = sTMCaseDynamicResources->numTMs + 1;
    gMultiuseListMenuTemplate.windowId = 0;
    gMultiuseListMenuTemplate.header_X = 0;
    gMultiuseListMenuTemplate.item_X = 8;
    gMultiuseListMenuTemplate.cursor_X = 0;
    gMultiuseListMenuTemplate.lettersSpacing = 0;
    gMultiuseListMenuTemplate.itemVerticalPadding = 2;
    gMultiuseListMenuTemplate.upText_Y = 2;
    gMultiuseListMenuTemplate.maxShowed = sTMCaseDynamicResources->maxTMsShown;
    gMultiuseListMenuTemplate.fontId = FONT_SHORT;
    gMultiuseListMenuTemplate.cursorPal = 2;
    gMultiuseListMenuTemplate.fillValue = 0;
    gMultiuseListMenuTemplate.cursorShadowPal = 3;
    gMultiuseListMenuTemplate.moveCursorFunc = TMCase_MoveCursorCallback;
    gMultiuseListMenuTemplate.itemPrintFunc = TMCase_ItemPrintFunc;
    gMultiuseListMenuTemplate.cursorKind = 0;
    gMultiuseListMenuTemplate.scrollMultiple = 0;
}

static void GetTMNumberAndMoveString(u8 *dest, u16 itemId)
{
    StringCopy(gStringVar4, gText_FontSize0);
    if (itemId >= ITEM_HM01)
    {
        StringAppend(gStringVar4, sText_ClearTo18);
        StringAppend(gStringVar4, gText_NumberClear01);
        ConvertIntToDecimalStringN(gStringVar1, itemId - ITEM_HM01 + 1, STR_CONV_MODE_LEADING_ZEROS, 1);
        StringAppend(gStringVar4, gStringVar1);
    }
    else
    {
        StringAppend(gStringVar4, gText_NumberClear01);
        ConvertIntToDecimalStringN(gStringVar1, itemId - ITEM_TM01 + 1, STR_CONV_MODE_LEADING_ZEROS, 2);
        StringAppend(gStringVar4, gStringVar1);
    }
    StringAppend(gStringVar4, gText_Space);
    StringAppend(gStringVar4, gText_FontSize2);
    StringAppend(gStringVar4, gMoveNames[ItemIdToBattleMoveId(itemId)]);
    StringCopy(dest, gStringVar4);
}

static void TMCase_MoveCursorCallback(s32 itemIndex, bool8 onInit, struct ListMenu *list)
{
    u16 itemId;

    if (itemIndex == LIST_CANCEL)
        itemId = 0;
    else
        itemId = BagGetItemIdByPocketPosition(POCKET_TM_HM, itemIndex);

    if (onInit != TRUE)
    {
        PlaySE(SE_SELECT);
        InitSelectedTMSpriteData(sTMCaseDynamicResources->tmSpriteId, itemId);
    }
    PrintDescription(itemId);
    PrintTMInfo(itemId);
    sTMCaseDynamicResources->itemId = itemId;
}

static void TMCase_ItemPrintFunc(u8 windowId, u32 itemId, u8 y)
{
    if (itemId != LIST_CANCEL)
    {
        if (BagGetItemIdByPocketPosition(POCKET_TM_HM, itemId) >= ITEM_HM01)
        {
            PlaceHMTileInWindow(windowId, 8, y);
        }
    }
}

static void PrintDescription(s32 itemId)
{
    const u8 *str = itemId != ITEM_NONE ? ItemId_GetDescription(itemId) : sText_TMCaseWillBePutAway;
    FillWindowPixelBuffer(WIN_INFO, 0);
    TMCase_Print(WIN_INFO, FONT_SHORT, str, 2, 3, 1, 0, 0, COLORID_LIGHT);
}

static void ShadeDescriptionWindow(bool8 darken)
{
    SetBgTilemapPalette(2, 0, 12, 30, 8, 2 * darken + 1);
    ScheduleBgCopyTilemapToVram(2);
}

static void TMCase_PrintCursor(u8 listTaskId, u8 colorIdx)
{
    TMCase_PrintCursorAtPos(ListMenuGetYCoordForPrintingArrowCursor(listTaskId), colorIdx);
}

static void TMCase_PrintCursorAtPos(u8 y, u8 colorIdx)
{
    if (colorIdx == COLORID_NONE)
    {
        FillWindowPixelRect(WIN_LIST, 0, 0, y, GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_WIDTH), GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_HEIGHT));
        CopyWindowToVram(WIN_LIST, COPYWIN_GFX);
    }
    else
    {
        TMCase_Print(WIN_LIST, FONT_SHORT, gText_SelectorArrow2, 0, y, 0, 0, 0, colorIdx);
    }
}

static void CreateTMCaseScrollIndicatorArrows(void)
{
    sTMCaseDynamicResources->scrollIndicatorArrowPairId = AddScrollIndicatorArrowPairParameterized(WIN_MSG, 0xA0, 0x08, 0x58, sTMCaseDynamicResources->numTMs - sTMCaseDynamicResources->maxTMsShown + 1, 0x6E, 0x6E, &sTMCaseStaticResources.scrollOffset);
}

static void RemoveTMCaseScrollIndicatorArrows(void)
{
    if (sTMCaseDynamicResources->scrollIndicatorArrowPairId != TASK_NONE)
    {
        RemoveScrollIndicatorArrowPair(sTMCaseDynamicResources->scrollIndicatorArrowPairId);
        sTMCaseDynamicResources->scrollIndicatorArrowPairId = TASK_NONE;
    }
}

void ResetTMCaseCursorPos(void)
{
    sTMCaseStaticResources.selectedRow = 0;
    sTMCaseStaticResources.scrollOffset = 0;
}

static void TMCaseSetup_GetTMCount(void)
{
    struct BagPocket * pocket = &gBagPockets[POCKET_TM_HM - 1];
    u16 i;

    CompactItemsInBagPocket(pocket);
    sTMCaseDynamicResources->numTMs = 0;
    for (i = 0; i < pocket->capacity; i++)
    {
        if (pocket->itemSlots[i].itemId == ITEM_NONE)
            break;
        sTMCaseDynamicResources->numTMs++;
    }
    sTMCaseDynamicResources->maxTMsShown = min(sTMCaseDynamicResources->numTMs + 1, 5);
}

static void TMCaseSetup_InitListMenuPositions(void)
{
    if (sTMCaseStaticResources.scrollOffset != 0)
    {
        if (sTMCaseStaticResources.scrollOffset + sTMCaseDynamicResources->maxTMsShown > sTMCaseDynamicResources->numTMs + 1)
            sTMCaseStaticResources.scrollOffset = sTMCaseDynamicResources->numTMs + 1 - sTMCaseDynamicResources->maxTMsShown;
    }
    if (sTMCaseStaticResources.scrollOffset + sTMCaseStaticResources.selectedRow >= sTMCaseDynamicResources->numTMs + 1)
    {
        if (sTMCaseDynamicResources->numTMs + 1 < 2)
            sTMCaseStaticResources.selectedRow = 0;
        else
            sTMCaseStaticResources.selectedRow = sTMCaseDynamicResources->numTMs;
    }
}

static void TMCaseSetup_UpdateVisualMenuOffset(void)
{
    u8 i;
    if (sTMCaseStaticResources.selectedRow > 3)
    {
        for (i = 0; i <= sTMCaseStaticResources.selectedRow - 3 && sTMCaseStaticResources.scrollOffset + sTMCaseDynamicResources->maxTMsShown != sTMCaseDynamicResources->numTMs + 1; i++)
        {
            do {} while (0);
            sTMCaseStaticResources.selectedRow--;
            sTMCaseStaticResources.scrollOffset++;
        }
    }
}

static void DestroyTMCaseBuffers(void)
{
    if (sTMCaseDynamicResources != NULL)
        Free(sTMCaseDynamicResources);
    if (sTilemapBuffer != NULL)
        Free(sTilemapBuffer);
    if (sListMenuItemsBuffer != NULL)
        Free(sListMenuItemsBuffer);
    if (sListMenuStringsBuffer != NULL)
        Free(sListMenuStringsBuffer);
    if (sTMSpritePaletteBuffer != NULL)
        Free(sTMSpritePaletteBuffer);
    FreeAllWindowBuffers();
}

static void Task_FadeAndCloseTMCase(u8 taskId)
{
    BeginNormalPaletteFade(PALETTES_ALL, -2, 0, 16, RGB_BLACK);
    gTasks[taskId].func = Task_CloseTMCase;
}

static void Task_CloseTMCase(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        DestroyListMenuTask(data[0], &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
        if (sTMCaseDynamicResources->savedCallback != NULL)
            SetMainCallback2(sTMCaseDynamicResources->savedCallback);
        else
            SetMainCallback2(sTMCaseStaticResources.savedCallback);
        RemoveTMCaseScrollIndicatorArrows();
        DestroyTMCaseBuffers();
        DestroyTask(taskId);
    }
}


static void Task_TMCaseHandleInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    s32 input;

    if (!gPaletteFade.active)
    {
        if (MenuHelpers_ShouldWaitForLinkRecv() != TRUE)
        {
            input = ListMenu_ProcessInput(data[0]);
            ListMenuGetScrollAndRow(data[0], &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
            switch (input)
            {
            case LIST_NOTHING_CHOSEN:
                break;
            case LIST_CANCEL:
                PlaySE(SE_SELECT);
                gSpecialVar_ItemId = 0;
                Task_FadeAndCloseTMCase(taskId);
                break;
            default:
                PlaySE(SE_SELECT);
                ShadeDescriptionWindow(TRUE);
                RemoveTMCaseScrollIndicatorArrows();
                TMCase_PrintCursor(data[0], COLORID_CURSOR_SELECTED);
                data[1] = input;
                data[2] = BagGetQuantityByPocketPosition(POCKET_TM_HM, input);
                gSpecialVar_ItemId = BagGetItemIdByPocketPosition(POCKET_TM_HM, input);
                gTasks[taskId].func = sSelectTMActionTasks[sTMCaseStaticResources.tmCaseMenuType];
                break;
            }
        }
    }
}

static void ReturnToTMCaseList(u8 taskId)
{
    ShadeDescriptionWindow(FALSE);
    CreateTMCaseScrollIndicatorArrows();
    gTasks[taskId].func = Task_TMCaseHandleInput;
}

static void Task_ContextMenu_FromFieldBag(u8 taskId)
{
    TMCase_SetWindowBorder(WIN_MSG);
    AddContextWindow(&sTMCaseDynamicResources->contextMenuWindowId);

    if (!MenuHelpers_IsLinkActive() && InUnionRoom() != TRUE)
    {
        sTMCaseDynamicResources->menuActionIndices = sTMCaseMenuItems_Normal;
        sTMCaseDynamicResources->numMenuActions = NELEMS(sTMCaseMenuItems_Normal);
    }
    else
    {
        sTMCaseDynamicResources->menuActionIndices = sTMCaseMenuItems_UnionRoom;
        sTMCaseDynamicResources->numMenuActions = NELEMS(sTMCaseMenuItems_UnionRoom);
    }
    PrintMenuActionTexts(sTMCaseDynamicResources->contextMenuWindowId, FONT_SHORT, GetMenuCursorDimensionByFont(FONT_SHORT, 0), 2, 0, GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_HEIGHT) + 2, sTMCaseDynamicResources->numMenuActions, sTMCaseMenuActions, sTMCaseDynamicResources->menuActionIndices);
    InitMenuNormal(sTMCaseDynamicResources->contextMenuWindowId, FONT_SHORT, 0, 2, GetFontAttribute(FONT_SHORT, FONTATTR_MAX_LETTER_HEIGHT) + 2, sTMCaseDynamicResources->numMenuActions, 0);
    GetTMNumberAndMoveString(gStringVar4, gSpecialVar_ItemId);
    StringAppend(gStringVar4, gText_Var1IsSelected + 2); // +2 skips over the stringvar
    TMCase_Print(WIN_MSG, FONT_SHORT, gStringVar4, 0, 2, 1, 0, 0, COLORID_DARK);
    if (gSpecialVar_ItemId >= ITEM_HM01)
    {
        PlaceHMTileInWindow(WIN_MSG, 0, 2);
        CopyWindowToVram(WIN_MSG, COPYWIN_GFX);
    }
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    gTasks[taskId].func = Task_TMContext_HandleInput;
}

static void Task_TMContext_HandleInput(u8 taskId)
{
    s8 input;

    if (MenuHelpers_ShouldWaitForLinkRecv() != TRUE)
    {
        input = Menu_ProcessInputNoWrap();
        switch (input)
        {
        case MENU_B_PRESSED:
            PlaySE(SE_SELECT);
            sTMCaseMenuActions[sTMCaseDynamicResources->menuActionIndices[sTMCaseDynamicResources->numMenuActions - 1]].func.void_u8(taskId);
            break;
        case MENU_NOTHING_CHOSEN:
            break;
        default:
            PlaySE(SE_SELECT);
            sTMCaseMenuActions[sTMCaseDynamicResources->menuActionIndices[input]].func.void_u8(taskId);
            break;
        }
    }
}

static void TMCaseAction_Use(u8 taskId)
{
    RemoveContextWindow(&sTMCaseDynamicResources->contextMenuWindowId);
    ClearStdWindowAndFrameToTransparent(WIN_MSG, FALSE);
    ClearWindowTilemap(WIN_MSG);
    PutWindowTilemap(WIN_LIST);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    if (CalculatePlayerPartyCount() == 0)
    {
        PrintThereIsNoPokemon(taskId);
    }
    else
    {
        gItemUseCB = ItemUseCB_TMHM;
        sTMCaseDynamicResources->savedCallback = CB2_ShowPartyMenuForItemUse;
        Task_FadeAndCloseTMCase(taskId);
    }
}

static void PrintThereIsNoPokemon(u8 taskId)
{
    TMCase_DisplayMessageWithCallback(taskId, FONT_NORMAL, gText_NoPokemon, HandleErrorMessage);
}

static void PrintItemCantBeHeld(u8 taskId)
{
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    StringExpandPlaceholders(gStringVar4, gText_Var1CantBeHeld);
    TMCase_DisplayMessageWithCallback(taskId, FONT_NORMAL, gStringVar4, HandleErrorMessage);
}

static void HandleErrorMessage(u8 taskId)
{
    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        CloseTMCaseMessage(taskId);
    }
}

static void CloseTMCaseMessage(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    DestroyListMenuTask(data[0], &sTMCaseStaticResources.scrollOffset, &sTMCaseStaticResources.selectedRow);
    data[0] = ListMenuInit(&gMultiuseListMenuTemplate, sTMCaseStaticResources.scrollOffset, sTMCaseStaticResources.selectedRow);
    TMCase_PrintCursor(data[0], COLORID_DARK);
    ClearDialogWindowAndFrameToTransparent(WIN_CASHIER_MSG, FALSE);
    ClearWindowTilemap(WIN_CASHIER_MSG);
    PutWindowTilemap(WIN_INFO);
    PutWindowTilemap(WIN_UI_MARKERS);
    PutWindowTilemap(WIN_MOVE_INFO);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    ReturnToTMCaseList(taskId);
}

static void TMCaseAction_Cancel(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    RemoveContextWindow(&sTMCaseDynamicResources->contextMenuWindowId);
    ClearStdWindowAndFrameToTransparent(WIN_MSG, FALSE);
    ClearWindowTilemap(WIN_MSG);
    PutWindowTilemap(WIN_LIST);
    TMCase_PrintCursor(data[0], COLORID_DARK);
    PutWindowTilemap(WIN_INFO);
    PutWindowTilemap(WIN_UI_MARKERS);
    PutWindowTilemap(WIN_MOVE_INFO);
    ScheduleBgCopyTilemapToVram(0);
    ScheduleBgCopyTilemapToVram(1);
    ReturnToTMCaseList(taskId);
}

static void Task_ContextMenu_FromPartyGiveMenu(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    if (!ItemId_GetImportance(BagGetItemIdByPocketPosition(POCKET_TM_HM, data[1])))
    {
        sTMCaseDynamicResources->savedCallback = CB2_GiveHoldItem;
        Task_FadeAndCloseTMCase(taskId);
    }
    else
    {
        PrintItemCantBeHeld(taskId);
    }
}

static void Task_ContextMenu_FromPokemonPC(u8 taskId)
{
    PrintItemCantBeHeld(taskId);
}

static void Task_ContextMenu_FromSellMenu(u8 taskId)
{
    CopyItemName(gSpecialVar_ItemId, gStringVar1);
    StringExpandPlaceholders(gStringVar4, gText_CantBuyKeyItem);
    TMCase_DisplayMessageWithCallback(taskId, FONT_NORMAL, gStringVar4, CloseTMCaseMessage);
}

static void InitTMCaseWindows(void)
{
    u8 i;

    InitWindows(sWindowTemplates);
    DeactivateAllTextPrinters();
    FillPalette(0, BG_PLTT_ID(0), PLTT_SIZE_4BPP * 16);

    LoadPalette(GetOverworldTextboxPalettePtr(), BG_PLTT_ID(4), PLTT_SIZE_4BPP);
    LoadUserWindowBorderGfx(WIN_LIST, 120, BG_PLTT_ID(5));
    LoadPalette(sTMCaseMainWindowPalette, BG_PLTT_ID(6), PLTT_SIZE_4BPP);
    ListMenuLoadStdPalAt(BG_PLTT_ID(7), 1);

    for (i = 0; i < ARRAY_COUNT(sWindowTemplates); i++)
        FillWindowPixelBuffer(i, PIXEL_FILL(0));

    PutWindowTilemap(WIN_LIST);
    PutWindowTilemap(WIN_INFO);
    PutWindowTilemap(WIN_LABEL);
    PutWindowTilemap(WIN_UI_MARKERS);
    PutWindowTilemap(WIN_MOVE_INFO);
    ScheduleBgCopyTilemapToVram(0);
    SetGpuRegBits(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(50, 255));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(100, 255));
    SetGpuRegBits(REG_OFFSET_WININ, WININ_WIN0_CLR);
    SetGpuRegBits(REG_OFFSET_WINOUT, WINOUT_WIN01_BG_ALL | WINOUT_WIN01_OBJ);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_BG0 | BLDCNT_TGT2_BG2);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
}

static void TMCase_Print(u8 windowId, u8 fontId, const u8 * str, u8 x, u8 y, u8 letterSpacing, u8 lineSpacing, u8 speed, u8 colorIdx)
{
    AddTextPrinterParameterized4(windowId, fontId, x, y, letterSpacing, lineSpacing, sFontColorTable[colorIdx], speed, str);
}

static void TMCase_SetWindowBorder(u8 windowId)
{
    DrawStdFrameWithCustomTileAndPalette(windowId, FALSE, 120, 5);
}

static void Task_TMCase_WaitForMessage(u8 taskId)
{
    if (!RunTextPrintersRetIsActive(WIN_CASHIER_MSG))
        SwitchTaskToFollowupFunc(taskId);
}

static void TMCase_DisplayMessageWithCallback(u8 taskId, u8 fontId, const u8 *str, TaskFunc callback)
{
    DrawStdFrameWithCustomTileAndPalette(WIN_CASHIER_MSG, FALSE, 120, 5);
    TMCase_Print(WIN_CASHIER_MSG, FONT_SHORT, str, 0, 1, 1, 1, GetPlayerTextSpeedDelay(), COLORID_DARK);
    SetTaskFuncWithFollowupFunc(taskId, Task_TMCase_WaitForMessage, callback);
    ScheduleBgCopyTilemapToVram(1);
}

static void PrintTMCaseLabel(void)
{
    const u8 *name = ItemId_GetName(ITEM_TM_CASE);
    u32 distance = 72 - GetStringWidth(FONT_NORMAL, name, 0);
    AddTextPrinterParameterized3(WIN_LABEL, FONT_NORMAL, distance / 2, 1, sFontColorTable[0], 0, name);
}

static void DrawMoveInfoUIMarkers(void)
{
    BlitMenuInfoIcon(WIN_UI_MARKERS, 19, 0, 0);
    BlitMenuInfoIcon(WIN_UI_MARKERS, 20, 0, 12);
    BlitMenuInfoIcon(WIN_UI_MARKERS, 21, 0, 24);
    BlitMenuInfoIcon(WIN_UI_MARKERS, 22, 0, 36);
    CopyWindowToVram(WIN_UI_MARKERS, COPYWIN_GFX);
}

static void PrintTMInfo(u16 itemId)
{
    u8 i;
    u16 move;
    const u8 * str;

    FillWindowPixelRect(WIN_MOVE_INFO, 0, 0, 0, 40, 48);
    if (itemId == ITEM_NONE)
    {
        for (i = 0; i < 4; i++)
            TMCase_Print(WIN_MOVE_INFO, FONT_SHORT_COPY_1, gText_ThreeDashes, 7, 12 * i, 0, 0, TEXT_SKIP_DRAW, COLORID_MOVE_INFO);
    }
    else
    {
        move = ItemIdToBattleMoveId(itemId);
        BlitMenuInfoIcon(WIN_MOVE_INFO, gBattleMoves[move].type + 1, 0, 0);
        if (gBattleMoves[move].power < 2)
            str = gText_ThreeDashes;
        else
        {
            ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[move].power, STR_CONV_MODE_RIGHT_ALIGN, 3);
            str = gStringVar1;
        }
        TMCase_Print(WIN_MOVE_INFO, FONT_SHORT_COPY_1, str, 7, 12, 0, 0, TEXT_SKIP_DRAW, COLORID_MOVE_INFO);
        if (gBattleMoves[move].accuracy == 0)
            str = gText_ThreeDashes;
        else
        {
            ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[move].accuracy, STR_CONV_MODE_RIGHT_ALIGN, 3);
            str = gStringVar1;
        }
        TMCase_Print(WIN_MOVE_INFO, FONT_SHORT_COPY_1, str, 7, 24, 0, 0, TEXT_SKIP_DRAW, COLORID_MOVE_INFO);
        ConvertIntToDecimalStringN(gStringVar1, gBattleMoves[move].pp, STR_CONV_MODE_RIGHT_ALIGN, 3);
        TMCase_Print(WIN_MOVE_INFO, FONT_SHORT_COPY_1, gStringVar1, 7, 36, 0, 0, TEXT_SKIP_DRAW, COLORID_MOVE_INFO);
    }
    CopyWindowToVram(WIN_MOVE_INFO, COPYWIN_GFX);
}

static void PlaceHMTileInWindow(u8 windowId, u8 x, u8 y)
{
    BlitBitmapToWindow(windowId, sTMCaseHMIcon_Gfx, x, y, 16, 12);
}

static u8 AddContextWindow(u8 *windowId)
{
    if (*windowId == WINDOW_NONE)
    {
        *windowId = AddWindow(&sContextWindowTemplate);
        TMCase_SetWindowBorder(*windowId);
        ScheduleBgCopyTilemapToVram(0);
    }
    return *windowId;
}

static void RemoveContextWindow(u8 *windowId)
{
    ClearStdWindowAndFrameToTransparent(*windowId, FALSE);
    ClearWindowTilemap(*windowId);
    RemoveWindow(*windowId);
    ScheduleBgCopyTilemapToVram(0);
    *windowId = WINDOW_NONE;
}

static u8 CreateDiscSprite(u16 itemId)
{
    u8 spriteId = CreateSprite(&sSpriteTemplate_TMDisc, 0x29, 0x2E, 0);
    u8 tmIndex;
    if (itemId == ITEM_NONE)
    {
        UpdateDiscSpritePosition(&gSprites[spriteId], 0xFF);
        return spriteId;
    }
    else
    {
        tmIndex = itemId - ITEM_TM01;
        SetTMSpriteAnim(&gSprites[spriteId], tmIndex);
        TintDiscSpriteByType(gBattleMoves[ItemIdToBattleMoveId(itemId)].type);
        UpdateDiscSpritePosition(&gSprites[spriteId], tmIndex);
        return spriteId;
    }
}

static void SetTMSpriteAnim(struct Sprite *sprite, u8 idx)
{
    if (idx >= (ITEM_HM01 - ITEM_TM01))
        StartSpriteAnim(sprite, ANIM_HM);
    else
        StartSpriteAnim(sprite, ANIM_TM);
}

static void TintDiscSpriteByType(u8 type)
{
    u8 paletteIndex = IndexOfSpritePaletteTag(TAG_DISC);
    LoadPalette(sTMSpritePaletteBuffer + PLTT_ID(sTMSpritePaletteOffsetByType[type]), OBJ_PLTT_ID(paletteIndex), PLTT_SIZE_4BPP);
}

static void UpdateDiscSpritePosition(struct Sprite *sprite, u8 idx)
{
    s32 x, y;
    if (idx == 0xFF)
    {
        x = 0x1B;
        y = 0x36;
        sprite->y2 = 0x14;
    }
    else
    {
        if (idx >= 50)
            idx -= 50;
        else
            idx += 8;
        x = 0x29 - (((0xE00 * idx) / 58) >> 8);
        y = 0x2E + (((0x800 * idx) / 58) >> 8);
    }
    sprite->x = x;
    sprite->y = y;
}

static void InitSelectedTMSpriteData(u8 spriteId, u16 itemId)
{
    gSprites[spriteId].data[0] = itemId;
    gSprites[spriteId].data[1] = 0;
    gSprites[spriteId].callback = SpriteCB_MoveTMSpriteInCase;
}

static void SpriteCB_MoveTMSpriteInCase(struct Sprite *sprite)
{
    switch (sprite->data[1])
    {
    case 0:
        if (sprite->y2 >= 20)
        {
            if (sprite->data[0] != ITEM_NONE)
            {
                sprite->data[1]++;
                TintDiscSpriteByType(gBattleMoves[ItemIdToBattleMoveId(sprite->data[0])].type);
                sprite->data[0] -= ITEM_TM01;
                SetTMSpriteAnim(sprite, sprite->data[0]);
                UpdateDiscSpritePosition(sprite, sprite->data[0]);
            }
            else
                sprite->callback = SpriteCallbackDummy;
        }
        else
        {
            sprite->y2 += 10;
        }
        break;
    case 1:
        if (sprite->y2 <= 0)
            sprite->callback = SpriteCallbackDummy;
        else
            sprite->y2 -= 10;
    }
}

static void LoadTMTypePalettes(void)
{
    // struct SpritePalette spritePalette;

    sTMSpritePaletteBuffer = Alloc(0x110 * sizeof(u16));
    LZDecompressWram(sTMCaseTypes1_Pal, sTMSpritePaletteBuffer);
    LZDecompressWram(sTMCaseTypes2_Pal, sTMSpritePaletteBuffer + 0x100);
    // spritePalette.data = sTMSpritePaletteBuffer + 0x110;
    // spritePalette.tag = TAG_DISC;
    AllocSpritePalette(TAG_DISC);
    // LoadSpritePalette(&spritePalette);
}
