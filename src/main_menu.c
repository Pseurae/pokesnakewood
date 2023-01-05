#include "global.h"
#include "trainer_pokemon_sprites.h"
#include "bg.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/trainers.h"
#include "decompress.h"
#include "event_data.h"
#include "event_object_movement.h"
#include "field_effect.h"
#include "gpu_regs.h"
#include "graphics.h"
#include "international_string_util.h"
#include "intro_speech.h"
#include "link.h"
#include "main.h"
#include "menu.h"
#include "list_menu.h"
#include "mystery_event_menu.h"
#include "naming_screen.h"
#include "option_menu.h"
#include "overworld.h"
#include "palette.h"
#include "pokeball.h"
#include "pokedex.h"
#include "pokemon.h"
#include "pokemon_icon.h"
#include "random.h"
#include "rtc.h"
#include "save.h"
#include "scanline_effect.h"
#include "sound.h"
#include "sprite.h"
#include "strings.h"
#include "string_util.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "title_screen.h"
#include "window.h"
#include "mystery_gift_menu.h"
#include "constants/event_objects.h"

/*
 * Main menu state machine
 * -----------------------
 *
 * Entry point: CB2_InitMainMenu
 *
 * Note: States advance sequentially unless otherwise stated.
 *
 * CB2_InitMainMenu / CB2_ReinitMainMenu
 *  - Both of these states call InitMainMenu, which does all the work.
 *  - In the Reinit case, the init code will check if the user came from
 *    the options screen. If they did, then the options menu item is
 *    pre-selected.
 *
 * Task_MainMenuCheckSaveFile
 *  - Determines how many menu options to show based on whether
 *    the save file is Ok, empty, corrupted, etc.
 *  - If there was an error loading the save file, advance to
 *    Task_WaitForSaveFileErrorWindow.
 *  - If there were no errors, advance to Task_MainMenuCheckBattery.
 *  - Note that the check to enable Mystery Events would normally happen
 *    here, but this version of Emerald has them disabled.
 *
 * Task_WaitForSaveFileErrorWindow
 *  - Wait for the text to finish printing and then for the A button
 *    to be pressed.
 *
 * Task_MainMenuCheckBattery
 *  - If the battery is OK, advance to Task_DisplayMainMenu.
 *  - If the battery is dry, advance to Task_WaitForBatteryDryErrorWindow.
 *
 * Task_WaitForBatteryDryErrorWindow
 *  - Wait for the text to finish printing and then for the A button
 *    to be pressed.
 *
 * Task_DisplayMainWindow
 *  - Display the buttons to the user. If the menu is in HAS_MYSTERY_EVENTS
 *    mode, there are too many buttons for one screen and a scrollbar is added,
 *    and the scrollbar task is spawned (Task_ScrollIndicatorArrowPairOnMainMenu).
 *
 * Task_HighlightSelectedMainMenuItem
 *  - Update the UI to match the currently selected item.
 *
 * Task_HandleMainMenuInput
 *  - If A is pressed, advance to Task_HandleMainMenuAPressed.
 *  - If B is pressed, return to the title screen via CB2_InitTitleScreen.
 *  - If Up or Down is pressed, handle scrolling if there is a scroll bar, change
 *    the selection, then go back to Task_HighlightSelectedMainMenuItem.
 *
 * Task_HandleMainMenuAPressed
 *  - If the user selected New Game, advance to Task_NewGameBirchSpeech_Init.
 *  - If the user selected Continue, advance to CB2_ContinueSavedGame.
 *  - If the user selected the Options menu, advance to CB2_InitOptionMenu.
 *  - If the user selected Mystery Gift, advance to CB2_InitMysteryGift. However,
 *    if the wireless adapter was removed, instead advance to
 *    Task_DisplayMainMenuInvalidActionError.
 *  - Code to start a Mystery Event is present here, but is unreachable in this
 *    version.
 *
 * Task_HandleMainMenuBPressed
 *  - Clean up the main menu and go back to CB2_InitTitleScreen.
 *
 * Task_DisplayMainMenuInvalidActionError
 *  - Print one of three different error messages, wait for the text to stop
 *    printing, and then wait for A or B to be pressed.
 * - Then advance to Task_HandleMainMenuBPressed.
 */

#define OPTION_MENU_FLAG (1 << 15)

// Static type declarations

// Static RAM declarations

static EWRAM_DATA bool8 sStartedPokeBallTask = 0;
static EWRAM_DATA u16 sCurrItemAndOptionMenuCheck = 0;

// Static ROM declarations

static u32 InitMainMenu(bool8);
static void Task_MainMenuCheckSaveFile(u8);
static void Task_MainMenuCheckBattery(u8);
static void Task_WaitForSaveFileErrorWindow(u8);
static void CreateMainMenuErrorWindow(const u8 *);
static void ClearMainMenuWindowTilemap(const struct WindowTemplate *);
static void Task_DisplayMainMenu(u8);
static void Task_WaitForBatteryDryErrorWindow(u8);
static void MainMenu_FormatSavegameText(u8);
static void HighlightSelectedMainMenuItem(u8, u8, s16);
static void Task_HandleMainMenuInput(u8);
static void Task_HandleMainMenuAPressed(u8);
static void Task_HandleMainMenuBPressed(u8);
static void Task_NewGameGenderSelection(u8);
static void Task_GenderSelectionMessage(u8);
static void Task_GenderSelectionStartSpriteFadeIn(u8);
static void Task_GenderSelection_WaitForPlayerFadeIn(u8);
static void Task_DisplayMainMenuInvalidActionError(u8);
static void AddBirchSpeechObjects(u8);
static void StartFadeInTarget1OutTarget2(u8, u8);
static void StartFadePlatformOut(u8, u8);
static void StartFadeOutTarget1InTarget2(u8, u8);
static void StartFadePlatformIn(u8, u8);
static void LoadMainMenuWindowFrameTiles(u8, u16);
static void DrawMainMenuWindowBorder(const struct WindowTemplate *, u16);
static void Task_HighlightSelectedMainMenuItem(u8);
void CreateYesNoMenuParameterized(u8, u8, u16, u16, u8, u8);
static void SpriteCB_MovePlayerDownWhileShrinking(struct Sprite *);
static void SpriteCB_Null();
static void MainMenu_FormatSavegamePlayer(u8);
static void MainMenu_FormatSavegamePokedex(u8);
static void MainMenu_FormatSavegameTime(u8);
static void MainMenu_FormatSavegameBadges(u8);
static void MainMenu_CreatePlayerSprite(u8);
static void MainMenu_CreatePokeIcons(u8);

// .rodata
static const u8 sMainMenuFrame_Gfx[] = INCBIN_U8("graphics/text_window/main_menu.4bpp");
static const u16 sMainMenuFrame_Pal[] = INCBIN_U16("graphics/text_window/main_menu.gbapal");

#define MENU_LEFT 2
#define MENU_TOP_WIN0 1
#define MENU_TOP_WIN1 5
#define MENU_TOP_WIN2 1
#define MENU_TOP_WIN3 13
#define MENU_TOP_WIN4 17
#define MENU_TOP_WIN5 1
#define MENU_TOP_WIN6 9
#define MENU_TOP_WIN7 13
#define MENU_WIDTH 26
#define MENU_HEIGHT_WIN0 2
#define MENU_HEIGHT_WIN1 2
#define MENU_HEIGHT_WIN2 10
#define MENU_HEIGHT_WIN3 2
#define MENU_HEIGHT_WIN4 2
#define MENU_HEIGHT_WIN5 6
#define MENU_HEIGHT_WIN6 2
#define MENU_HEIGHT_WIN7 2

#define MENU_LEFT_ERROR 2
#define MENU_TOP_ERROR 15
#define MENU_WIDTH_ERROR 26
#define MENU_HEIGHT_ERROR 4

#define MENU_SHADOW_PADDING 1

#define MENU_WIN_HCOORDS WIN_RANGE(((MENU_LEFT - 1) * 8) + MENU_SHADOW_PADDING, (MENU_LEFT + MENU_WIDTH + 1) * 8 - MENU_SHADOW_PADDING)
#define MENU_WIN_VCOORDS(n) WIN_RANGE(((MENU_TOP_WIN##n - 1) * 8) + MENU_SHADOW_PADDING, (MENU_TOP_WIN##n + MENU_HEIGHT_WIN##n + 1) * 8 - MENU_SHADOW_PADDING)
#define MENU_SCROLL_SHIFT WIN_RANGE(32, 32)

static const struct WindowTemplate sWindowTemplates_MainMenu[] =
{
    // No saved game
    // NEW GAME
    {
        .bg = 0,
        .tilemapLeft = MENU_LEFT,
        .tilemapTop = MENU_TOP_WIN0,
        .width = MENU_WIDTH,
        .height = MENU_HEIGHT_WIN0,
        .paletteNum = 15,
        .baseBlock = 1
    },
    // OPTIONS
    {
        .bg = 0,
        .tilemapLeft = MENU_LEFT,
        .tilemapTop = MENU_TOP_WIN1,
        .width = MENU_WIDTH,
        .height = MENU_HEIGHT_WIN1,
        .paletteNum = 15,
        .baseBlock = 0x35
    },
    // Has saved game with Pokemon
    // CONTINUE
    {
        .bg = 0,
        .tilemapLeft = MENU_LEFT,
        .tilemapTop = MENU_TOP_WIN2,
        .width = MENU_WIDTH,
        .height = MENU_HEIGHT_WIN2,
        .paletteNum = 15,
        .baseBlock = 1
    },
    // NEW GAME
    {
        .bg = 0,
        .tilemapLeft = MENU_LEFT,
        .tilemapTop = MENU_TOP_WIN3,
        .width = MENU_WIDTH,
        .height = MENU_HEIGHT_WIN3,
        .paletteNum = 15,
        .baseBlock = 0x105
    },
    // OPTION
    {
        .bg = 0,
        .tilemapLeft = MENU_LEFT,
        .tilemapTop = MENU_TOP_WIN4,
        .width = MENU_WIDTH,
        .height = MENU_HEIGHT_WIN4,
        .paletteNum = 15,
        .baseBlock = 0x139
    },
    // Has saved game with no Pokemon
    // CONTINUE
    {
        .bg = 0,
        .tilemapLeft = MENU_LEFT,
        .tilemapTop = MENU_TOP_WIN5,
        .width = MENU_WIDTH,
        .height = MENU_HEIGHT_WIN5,
        .paletteNum = 15,
        .baseBlock = 1
    },
    // NEW GAME
    {
        .bg = 0,
        .tilemapLeft = MENU_LEFT,
        .tilemapTop = MENU_TOP_WIN6,
        .width = MENU_WIDTH,
        .height = MENU_HEIGHT_WIN6,
        .paletteNum = 15,
        .baseBlock = 0x105
    },
    // OPTION
    {
        .bg = 0,
        .tilemapLeft = MENU_LEFT,
        .tilemapTop = MENU_TOP_WIN7,
        .width = MENU_WIDTH,
        .height = MENU_HEIGHT_WIN7,
        .paletteNum = 15,
        .baseBlock = 0x139
    },
    // Error message window
    {
        .bg = 0,
        .tilemapLeft = MENU_LEFT_ERROR,
        .tilemapTop = MENU_TOP_ERROR,
        .width = MENU_WIDTH_ERROR,
        .height = MENU_HEIGHT_ERROR,
        .paletteNum = 15,
        .baseBlock = 0x1D5
    },
    DUMMY_WIN_TEMPLATE
};

static const struct WindowTemplate sNewGameBirchSpeechTextWindows[] =
{
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 27,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 1
    },
    {
        .bg = 0,
        .tilemapLeft = 3,
        .tilemapTop = 5,
        .width = 6,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 0x6D
    },
    {
        .bg = 0,
        .tilemapLeft = 3,
        .tilemapTop = 2,
        .width = 9,
        .height = 10,
        .paletteNum = 15,
        .baseBlock = 0x85
    },
    DUMMY_WIN_TEMPLATE
};

static const u16 sMainMenuBgPal[] = INCBIN_U16("graphics/interface/main_menu_bg.gbapal");
static const u16 sMainMenuTextPal[] = INCBIN_U16("graphics/interface/main_menu_text.gbapal");

static const u8 sTextColor_Headers[] = {TEXT_DYNAMIC_COLOR_1, TEXT_COLOR_DARK_GRAY, TEXT_COLOR_LIGHT_GRAY};
static const u8 sTextColor_MenuInfo[] = {TEXT_DYNAMIC_COLOR_1, TEXT_COLOR_WHITE, TEXT_COLOR_LIGHT_GRAY};

static const struct BgTemplate sMainMenuBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 2,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 0,
        .mapBaseIndex = 7,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 3,
        .baseTile = 0
    }
};

static const struct ScrollArrowsTemplate sScrollArrowsTemplate_MainMenu = {2, 0x78, 8, 3, 0x78, 0x98, 3, 4, 1, 1, 0};

static const union AffineAnimCmd sSpriteAffineAnim_PlayerShrink[] = {
    AFFINEANIMCMD_FRAME(-2, -2, 0, 0x30),
    AFFINEANIMCMD_END
};

static const union AffineAnimCmd *const sSpriteAffineAnimTable_PlayerShrink[] =
{
    sSpriteAffineAnim_PlayerShrink
};

static const struct MenuAction sMenuActions_Gender[] = {
    {gText_BirchBoy, NULL},
    {gText_BirchGirl, NULL}
};

// .text

enum
{
    HAS_NO_SAVED_GAME,     //NEW GAME, OPTION
    HAS_SAVED_GAME,        //CONTINUE, NEW GAME, OPTION
    HAS_SAVED_GAME_NO_POKE //CONTINUE, NEW GAME, OPTION
};

enum
{
    ACTION_NEW_GAME,
    ACTION_CONTINUE,
    ACTION_OPTION,
    ACTION_MYSTERY_GIFT,
    ACTION_MYSTERY_EVENTS,
    ACTION_EREADER,
    ACTION_INVALID
};

EWRAM_DATA static u8 sContinueMonIconSpriteIds[PARTY_SIZE] = { MAX_SPRITES };

#define MAIN_MENU_BORDER_TILE   0x1D5

static void CB2_MainMenu(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    UpdatePaletteFade();
}

static void VBlankCB_MainMenu(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_InitMainMenu(void)
{
    InitMainMenu(FALSE);
}

void CB2_ReinitMainMenu(void)
{
    InitMainMenu(TRUE);
}

static u32 InitMainMenu(bool8 returningFromOptionsMenu)
{
    SetVBlankCallback(NULL);

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);

    DmaFill16(3, 0, (void *)VRAM, VRAM_SIZE);
    DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
    DmaFill16(3, 0, (void *)(PLTT + 2), PLTT_SIZE - 2);

    ResetPaletteFade();
    LoadPalette(sMainMenuBgPal, 0, 32);
    LoadPalette(sMainMenuTextPal, 0xF0, 32);
    ScanlineEffect_Stop();
    ResetTasks();
    ResetSpriteData();
    FreeAllSpritePalettes();
    if (returningFromOptionsMenu)
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_BLACK); // fade to black
    else
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0x10, 0, RGB_WHITEALPHA); // fade to white
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sMainMenuBgTemplates, ARRAY_COUNT(sMainMenuBgTemplates));
    ChangeBgX(0, 0, BG_COORD_SET);
    ChangeBgY(0, 0, BG_COORD_SET);
    ChangeBgX(1, 0, BG_COORD_SET);
    ChangeBgY(1, 0, BG_COORD_SET);
    InitWindows(sWindowTemplates_MainMenu);
    DeactivateAllTextPrinters();
    LoadMainMenuWindowFrameTiles(0, MAIN_MENU_BORDER_TILE);

    SetGpuReg(REG_OFFSET_WIN0H, 0);
    SetGpuReg(REG_OFFSET_WIN0V, 0);
    SetGpuReg(REG_OFFSET_WININ, 0);
    SetGpuReg(REG_OFFSET_WINOUT, 0);
    SetGpuReg(REG_OFFSET_BLDCNT, 0);
    SetGpuReg(REG_OFFSET_BLDALPHA, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);

    EnableInterrupts(1);
    SetVBlankCallback(VBlankCB_MainMenu);
    SetMainCallback2(CB2_MainMenu);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_WIN0_ON | DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);
    ShowBg(0);
    HideBg(1);
    CreateTask(Task_MainMenuCheckSaveFile, 0);

    return 0;
}

#define tMenuType data[0]
#define tCurrItem data[1]
#define tItemCount data[12]
#define tScrollArrowTaskId data[13]
#define tIsScrolled data[14]
#define tWirelessAdapterConnected data[15]

#define tArrowTaskIsScrolled data[15]   // For scroll indicator arrow task

static void Task_MainMenuCheckSaveFile(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0 | WININ_WIN0_OBJ);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_DARKEN | BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 7);

        if (IsWirelessAdapterConnected())
            tWirelessAdapterConnected = TRUE;
        switch (gSaveFileStatus)
        {
            case SAVE_STATUS_OK:
                if (gPlayerPartyCount > 0)
                    tMenuType = HAS_SAVED_GAME;
                else
                    tMenuType = HAS_SAVED_GAME_NO_POKE;
                gTasks[taskId].func = Task_MainMenuCheckBattery;
                break;
            case SAVE_STATUS_CORRUPT:
                CreateMainMenuErrorWindow(gText_SaveFileErased);
                tMenuType = HAS_NO_SAVED_GAME;
                gTasks[taskId].func = Task_WaitForSaveFileErrorWindow;
                break;
            case SAVE_STATUS_ERROR:
                CreateMainMenuErrorWindow(gText_SaveFileCorrupted);
                gTasks[taskId].func = Task_WaitForSaveFileErrorWindow;
                if (gPlayerPartyCount > 0)
                    tMenuType = HAS_SAVED_GAME;
                else
                    tMenuType = HAS_SAVED_GAME_NO_POKE;
                break;
            case SAVE_STATUS_EMPTY:
            default:
                tMenuType = HAS_NO_SAVED_GAME;
                gTasks[taskId].func = Task_MainMenuCheckBattery;
                break;
            case SAVE_STATUS_NO_FLASH:
                CreateMainMenuErrorWindow(gJPText_No1MSubCircuit);
                gTasks[taskId].tMenuType = HAS_NO_SAVED_GAME;
                gTasks[taskId].func = Task_WaitForSaveFileErrorWindow;
                break;
        }
        if (sCurrItemAndOptionMenuCheck & OPTION_MENU_FLAG)   // are we returning from the options menu?
            sCurrItemAndOptionMenuCheck = tMenuType + 1;

        sCurrItemAndOptionMenuCheck &= ~OPTION_MENU_FLAG;  // turn off the "returning from options menu" flag
        tCurrItem = sCurrItemAndOptionMenuCheck;
        tItemCount = tMenuType + 2;
    }
}

static void Task_WaitForSaveFileErrorWindow(u8 taskId)
{
    RunTextPrinters();
    if (!IsTextPrinterActive(7) && (JOY_NEW(A_BUTTON)))
    {
        ClearWindowTilemap(7);
        ClearMainMenuWindowTilemap(&sWindowTemplates_MainMenu[8]);
        gTasks[taskId].func = Task_MainMenuCheckBattery;
    }
}

static void Task_MainMenuCheckBattery(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0 | WININ_WIN0_OBJ);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_DARKEN | BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ);
        SetGpuReg(REG_OFFSET_BLDALPHA, 0);
        SetGpuReg(REG_OFFSET_BLDY, 7);

        if (!(RtcGetErrorStatus() & RTC_ERR_FLAG_MASK))
        {
            gTasks[taskId].func = Task_DisplayMainMenu;
        }
        else
        {
            CreateMainMenuErrorWindow(gText_BatteryRunDry);
            gTasks[taskId].func = Task_WaitForBatteryDryErrorWindow;
        }
    }
}

static void Task_WaitForBatteryDryErrorWindow(u8 taskId)
{
    RunTextPrinters();
    if (!IsTextPrinterActive(7) && (JOY_NEW(A_BUTTON)))
    {
        ClearWindowTilemap(7);
        ClearMainMenuWindowTilemap(&sWindowTemplates_MainMenu[8]);
        gTasks[taskId].func = Task_DisplayMainMenu;
    }
}

static const u8 sPokeBorderGfx[] = INCBIN_U8("graphics/text_window/poke_border.4bpp");

static void Task_DisplayMainMenu(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    u16 palette;

    if (!gPaletteFade.active)
    {
        SetGpuReg(REG_OFFSET_WIN0H, 0);
        SetGpuReg(REG_OFFSET_WIN0V, 0);
        SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0 | WININ_WIN0_OBJ);
        SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
        SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_EFFECT_DARKEN | BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ | BLDCNT_TGT2_BG0);
        SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(7, 11));
        SetGpuReg(REG_OFFSET_BLDY, 7);

        palette = RGB_WHITE;
        LoadPalette(&palette, 250, 2);

        palette = RGB_BLACK;
        LoadPalette(&palette, 251, 2);

        // palette = RGB(12, 12, 12);
        // LoadPalette(&palette, 251, 2);

        // palette = RGB(26, 26, 25);
        // LoadPalette(&palette, 252, 2);

        // Note: If there is no save file, the save block is zeroed out,
        // so the default gender is MALE.
        if (gSaveBlock2Ptr->playerGender == MALE)
        {
            palette = RGB(4, 16, 31);
            LoadPalette(&palette, 241, 2);
        }
        else
        {
            palette = RGB(31, 3, 21);
            LoadPalette(&palette, 241, 2);
        }

        switch (gTasks[taskId].tMenuType)
        {
            case HAS_NO_SAVED_GAME:
            default:
                FillWindowPixelBuffer(0, PIXEL_FILL(0xA));
                FillWindowPixelBuffer(1, PIXEL_FILL(0xA));
                AddTextPrinterParameterized3(0, FONT_SHORT, GetStringCenterAlignXOffset(FONT_SHORT, gText_MainMenuNewGame, 208), 1, sTextColor_Headers, TEXT_SKIP_DRAW, gText_MainMenuNewGame);
                AddTextPrinterParameterized3(1, FONT_SHORT, GetStringCenterAlignXOffset(FONT_SHORT, gText_MainMenuOption, 208), 1, sTextColor_Headers, TEXT_SKIP_DRAW, gText_MainMenuOption);
                PutWindowTilemap(0);
                PutWindowTilemap(1);
                CopyWindowToVram(0, COPYWIN_GFX);
                CopyWindowToVram(1, COPYWIN_GFX);
                DrawMainMenuWindowBorder(&sWindowTemplates_MainMenu[0], MAIN_MENU_BORDER_TILE);
                DrawMainMenuWindowBorder(&sWindowTemplates_MainMenu[1], MAIN_MENU_BORDER_TILE);
                break;
            case HAS_SAVED_GAME:
                FillWindowPixelBuffer(2, PIXEL_FILL(0xA));
                FillWindowPixelBuffer(3, PIXEL_FILL(0xA));
                FillWindowPixelBuffer(4, PIXEL_FILL(0xA));
                AddTextPrinterParameterized3(2, FONT_SHORT, GetStringCenterAlignXOffset(FONT_SHORT, gText_MainMenuContinue, 208), 1, sTextColor_Headers, TEXT_SKIP_DRAW, gText_MainMenuContinue);
                AddTextPrinterParameterized3(3, FONT_SHORT, GetStringCenterAlignXOffset(FONT_SHORT, gText_MainMenuNewGame, 208), 1, sTextColor_Headers, TEXT_SKIP_DRAW, gText_MainMenuNewGame);
                AddTextPrinterParameterized3(4, FONT_SHORT, GetStringCenterAlignXOffset(FONT_SHORT, gText_MainMenuOption, 208), 1, sTextColor_Headers, TEXT_SKIP_DRAW, gText_MainMenuOption);
                MainMenu_FormatSavegameText(2);
                PutWindowTilemap(2);
                PutWindowTilemap(3);
                PutWindowTilemap(4);
                CopyWindowToVram(2, COPYWIN_GFX);
                CopyWindowToVram(3, COPYWIN_GFX);
                CopyWindowToVram(4, COPYWIN_GFX);
                DrawMainMenuWindowBorder(&sWindowTemplates_MainMenu[2], MAIN_MENU_BORDER_TILE);
                DrawMainMenuWindowBorder(&sWindowTemplates_MainMenu[3], MAIN_MENU_BORDER_TILE);
                DrawMainMenuWindowBorder(&sWindowTemplates_MainMenu[4], MAIN_MENU_BORDER_TILE);
                break;
            case HAS_SAVED_GAME_NO_POKE:
                FillWindowPixelBuffer(5, PIXEL_FILL(0xA));
                FillWindowPixelBuffer(6, PIXEL_FILL(0xA));
                FillWindowPixelBuffer(7, PIXEL_FILL(0xA));
                AddTextPrinterParameterized3(5, FONT_SHORT, GetStringCenterAlignXOffset(FONT_SHORT, gText_MainMenuContinue, 208), 1, sTextColor_Headers, TEXT_SKIP_DRAW, gText_MainMenuContinue);
                AddTextPrinterParameterized3(6, FONT_SHORT, GetStringCenterAlignXOffset(FONT_SHORT, gText_MainMenuNewGame, 208), 1, sTextColor_Headers, TEXT_SKIP_DRAW, gText_MainMenuNewGame);
                AddTextPrinterParameterized3(7, FONT_SHORT, GetStringCenterAlignXOffset(FONT_SHORT, gText_MainMenuOption, 208), 1, sTextColor_Headers, TEXT_SKIP_DRAW, gText_MainMenuOption);
                MainMenu_FormatSavegameText(5);
                PutWindowTilemap(5);
                PutWindowTilemap(6);
                PutWindowTilemap(7);
                CopyWindowToVram(5, COPYWIN_GFX);
                CopyWindowToVram(6, COPYWIN_GFX);
                CopyWindowToVram(7, COPYWIN_GFX);
                DrawMainMenuWindowBorder(&sWindowTemplates_MainMenu[5], MAIN_MENU_BORDER_TILE);
                DrawMainMenuWindowBorder(&sWindowTemplates_MainMenu[6], MAIN_MENU_BORDER_TILE);
                DrawMainMenuWindowBorder(&sWindowTemplates_MainMenu[7], MAIN_MENU_BORDER_TILE);
                break;
        }
        gTasks[taskId].func = Task_HighlightSelectedMainMenuItem;
    }
}

static void Task_HighlightSelectedMainMenuItem(u8 taskId)
{
    HighlightSelectedMainMenuItem(gTasks[taskId].tMenuType, gTasks[taskId].tCurrItem, gTasks[taskId].tIsScrolled);
    gTasks[taskId].func = Task_HandleMainMenuInput;
}

static bool8 HandleMainMenuInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (JOY_NEW(A_BUTTON))
    {
        PlaySE(SE_SELECT);
        IsWirelessAdapterConnected();   // why bother calling this here? debug? Task_HandleMainMenuAPressed will check too
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_BLACK);
        gTasks[taskId].func = Task_HandleMainMenuAPressed;
    }
    else if (JOY_NEW(B_BUTTON))
    {
        PlaySE(SE_SELECT);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 0x10, RGB_WHITEALPHA);
        SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(0, DISPLAY_WIDTH));
        SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(0, DISPLAY_HEIGHT));
        gTasks[taskId].func = Task_HandleMainMenuBPressed;
    }
    else if ((JOY_NEW(DPAD_UP)) && tCurrItem > 0)
    {
        tCurrItem--;
        sCurrItemAndOptionMenuCheck = tCurrItem;
        return TRUE;
    }
    else if ((JOY_NEW(DPAD_DOWN)) && tCurrItem < tItemCount - 1)
    {
        tCurrItem++;
        sCurrItemAndOptionMenuCheck = tCurrItem;
        return TRUE;
    }
    return FALSE;
}

static void Task_HandleMainMenuInput(u8 taskId)
{
    if (HandleMainMenuInput(taskId))
        gTasks[taskId].func = Task_HighlightSelectedMainMenuItem;
}

static void Task_HandleMainMenuAPressed(u8 taskId)
{
    bool8 wirelessAdapterConnected;
    u8 action;

    if (!gPaletteFade.active)
    {
        ClearStdWindowAndFrame(0, TRUE);
        ClearStdWindowAndFrame(1, TRUE);
        ClearStdWindowAndFrame(2, TRUE);
        ClearStdWindowAndFrame(3, TRUE);
        ClearStdWindowAndFrame(4, TRUE);
        ClearStdWindowAndFrame(5, TRUE);
        ClearStdWindowAndFrame(6, TRUE);
        ClearStdWindowAndFrame(7, TRUE);
        wirelessAdapterConnected = IsWirelessAdapterConnected();
        switch (gTasks[taskId].tMenuType)
        {
            case HAS_NO_SAVED_GAME:
            default:
                switch (gTasks[taskId].tCurrItem)
                {
                    case 0:
                    default:
                        action = ACTION_NEW_GAME;
                        break;
                    case 1:
                        action = ACTION_OPTION;
                        break;
                }
                break;
            case HAS_SAVED_GAME:
            case HAS_SAVED_GAME_NO_POKE:
                switch (gTasks[taskId].tCurrItem)
                {
                    case 0:
                    default:
                        action = ACTION_CONTINUE;
                        break;
                    case 1:
                        action = ACTION_NEW_GAME;
                        break;
                    case 2:
                        action = ACTION_OPTION;
                        break;
                }
                break;
        }
        ChangeBgY(0, 0, BG_COORD_SET);
        ChangeBgY(1, 0, BG_COORD_SET);
        switch (action)
        {
            case ACTION_NEW_GAME:
            default:
                gPlttBufferUnfaded[0] = RGB_BLACK;
                gPlttBufferFaded[0] = RGB_BLACK;
                SetMainCallback2(CB2_NewGameIntro);
                DestroyTask(taskId);
                break;
            case ACTION_CONTINUE:
                gPlttBufferUnfaded[0] = RGB_BLACK;
                gPlttBufferFaded[0] = RGB_BLACK;
                SetMainCallback2(CB2_ContinueSavedGame);
                DestroyTask(taskId);
                break;
            case ACTION_OPTION:
                gMain.savedCallback = CB2_ReinitMainMenu;
                SetMainCallback2(CB2_InitOptionMenu);
                DestroyTask(taskId);
                break;
            case ACTION_MYSTERY_GIFT:
                SetMainCallback2(CB2_InitMysteryGift);
                DestroyTask(taskId);
                break;
            case ACTION_MYSTERY_EVENTS:
                SetMainCallback2(CB2_InitMysteryEventMenu);
                DestroyTask(taskId);
                break;
            case ACTION_EREADER:
                SetMainCallback2(CB2_InitEReader);
                DestroyTask(taskId);
                break;
            case ACTION_INVALID:
                gTasks[taskId].tCurrItem = 0;
                gTasks[taskId].func = Task_DisplayMainMenuInvalidActionError;
                gPlttBufferUnfaded[0xF1] = RGB_WHITE;
                gPlttBufferFaded[0xF1] = RGB_WHITE;
                SetGpuReg(REG_OFFSET_BG2HOFS, 0);
                SetGpuReg(REG_OFFSET_BG2VOFS, 0);
                SetGpuReg(REG_OFFSET_BG1HOFS, 0);
                SetGpuReg(REG_OFFSET_BG1VOFS, 0);
                SetGpuReg(REG_OFFSET_BG0HOFS, 0);
                SetGpuReg(REG_OFFSET_BG0VOFS, 0);
                BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
                return;
        }
        FreeAllWindowBuffers();
        if (action != ACTION_OPTION)
            sCurrItemAndOptionMenuCheck = 0;
        else
            sCurrItemAndOptionMenuCheck |= OPTION_MENU_FLAG;  // entering the options menu
    }
}

static void Task_HandleMainMenuBPressed(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        sCurrItemAndOptionMenuCheck = 0;
        FreeAllWindowBuffers();
        SetMainCallback2(CB2_InitTitleScreen);
        DestroyTask(taskId);
    }
}

static void Task_DisplayMainMenuInvalidActionError(u8 taskId)
{
    switch (gTasks[taskId].tCurrItem)
    {
        case 0:
            FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, DISPLAY_TILE_WIDTH, DISPLAY_TILE_HEIGHT);
            switch (gTasks[taskId].tMenuType)
            {
                case 0:
                    CreateMainMenuErrorWindow(gText_WirelessNotConnected);
                    break;
                case 1:
                    CreateMainMenuErrorWindow(gText_MysteryGiftCantUse);
                    break;
                case 2:
                    CreateMainMenuErrorWindow(gText_MysteryEventsCantUse);
                    break;
            }
            gTasks[taskId].tCurrItem++;
            break;
        case 1:
            if (!gPaletteFade.active)
                gTasks[taskId].tCurrItem++;
            break;
        case 2:
            RunTextPrinters();
            if (!IsTextPrinterActive(7))
                gTasks[taskId].tCurrItem++;
            break;
        case 3:
            if (JOY_NEW(A_BUTTON | B_BUTTON))
            {
                PlaySE(SE_SELECT);
                BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
                gTasks[taskId].func = Task_HandleMainMenuBPressed;
            }
    }
}

#undef tMenuType
#undef tCurrItem
#undef tItemCount
#undef tScrollArrowTaskId
#undef tIsScrolled
#undef tWirelessAdapterConnected

#undef tArrowTaskIsScrolled

static void SetMonIconsAnim(u8 animNum)
{
    u8 i;
    for (i = 0; i < gPlayerPartyCount; i++)
    {
        if (sContinueMonIconSpriteIds[i] != MAX_SPRITES)
            StartSpriteAnim(&gSprites[sContinueMonIconSpriteIds[i]], animNum);
    }
}

static void HighlightSelectedMainMenuItem(u8 menuType, u8 selectedMenuItem, s16 isScrolled)
{
    SetGpuReg(REG_OFFSET_WIN0H, MENU_WIN_HCOORDS);
    SetMonIconsAnim(4);

    switch (menuType)
    {
        case HAS_NO_SAVED_GAME:
        default:
            switch (selectedMenuItem)
            {
                case 0:
                default:
                    SetGpuReg(REG_OFFSET_WIN0V, MENU_WIN_VCOORDS(0));
                    break;
                case 1:
                    SetGpuReg(REG_OFFSET_WIN0V, MENU_WIN_VCOORDS(1));
                    break;
            }
            break;
        case HAS_SAVED_GAME:
            switch (selectedMenuItem)
            {
                case 0:
                default:
                    SetMonIconsAnim(0);
                    SetGpuReg(REG_OFFSET_WIN0V, MENU_WIN_VCOORDS(2));
                    break;
                case 1:
                    SetGpuReg(REG_OFFSET_WIN0V, MENU_WIN_VCOORDS(3));
                    break;
                case 2:
                    SetGpuReg(REG_OFFSET_WIN0V, MENU_WIN_VCOORDS(4));
                    break;
            }
            break;
        case HAS_SAVED_GAME_NO_POKE:
            switch (selectedMenuItem)
            {
                case 0:
                default:
                    SetMonIconsAnim(0);
                    SetGpuReg(REG_OFFSET_WIN0V, MENU_WIN_VCOORDS(5));
                    break;
                case 1:
                    SetGpuReg(REG_OFFSET_WIN0V, MENU_WIN_VCOORDS(6));
                    break;
                case 2:
                    SetGpuReg(REG_OFFSET_WIN0V, MENU_WIN_VCOORDS(7));
                    break;
            }
            break;
    }
}

static void CreateMainMenuErrorWindow(const u8 *str)
{
    FillWindowPixelBuffer(7, PIXEL_FILL(1));
    AddTextPrinterParameterized(7, FONT_NORMAL, str, 0, 1, 2, 0);
    PutWindowTilemap(7);
    CopyWindowToVram(7, COPYWIN_GFX);
    DrawMainMenuWindowBorder(&sWindowTemplates_MainMenu[8], MAIN_MENU_BORDER_TILE);
    SetGpuReg(REG_OFFSET_WIN0H, WIN_RANGE(9, DISPLAY_WIDTH - 9));
    SetGpuReg(REG_OFFSET_WIN0V, WIN_RANGE(113, DISPLAY_HEIGHT - 1));
}

static const u8 sGenderObjectSprite[2] = {
    [MALE]   = OBJ_EVENT_GFX_BRENDAN_NORMAL,
    [FEMALE] = OBJ_EVENT_GFX_MAY_NORMAL
};

static void MainMenu_FormatSavegameText(u8 windowId)
{
    MainMenu_FormatSavegamePlayer(windowId);
    // MainMenu_FormatSavegamePokedex(windowId);
    MainMenu_FormatSavegameTime(windowId);
    // MainMenu_FormatSavegameBadges(windowId);
    MainMenu_CreatePlayerSprite(windowId);
    MainMenu_CreatePokeIcons(windowId);
}

static void MainMenu_FormatSavegamePlayer(u8 windowId)
{
    StringExpandPlaceholders(gStringVar4, gText_ContinueMenuPlayer);
    AddTextPrinterParameterized3(windowId, FONT_SHORT, 0x58, 17, sTextColor_MenuInfo, TEXT_SKIP_DRAW, gStringVar4);
    AddTextPrinterParameterized3(windowId, FONT_SHORT, GetStringRightAlignXOffset(FONT_SHORT, gSaveBlock2Ptr->playerName, 0xC0), 17, sTextColor_MenuInfo, TEXT_SKIP_DRAW, gSaveBlock2Ptr->playerName);
}

static void MainMenu_FormatSavegameTime(u8 windowId)
{
    u8 str[0x20];
    u8 *ptr;

    StringExpandPlaceholders(gStringVar4, gText_ContinueMenuTime);
    AddTextPrinterParameterized3(windowId, FONT_SHORT, 0x58, 31, sTextColor_MenuInfo, TEXT_SKIP_DRAW, gStringVar4);
    ptr = ConvertIntToDecimalStringN(str, gSaveBlock2Ptr->playTimeHours, STR_CONV_MODE_LEFT_ALIGN, 3);
    *ptr = CHAR_COLON;
    ConvertIntToDecimalStringN(ptr + 1, gSaveBlock2Ptr->playTimeMinutes, STR_CONV_MODE_LEADING_ZEROS, 2);
    AddTextPrinterParameterized3(windowId, FONT_SHORT, GetStringRightAlignXOffset(FONT_SHORT, str, 0xC0), 31, sTextColor_MenuInfo, TEXT_SKIP_DRAW, str);
}

static void MainMenu_FormatSavegamePokedex(u8 windowId)
{
    u8 str[0x20];
    u16 dexCount;

    if (FlagGet(FLAG_SYS_POKEDEX_GET) == TRUE)
    {
        if (IsNationalPokedexEnabled())
            dexCount = GetNationalPokedexCount(FLAG_GET_CAUGHT);
        else
            dexCount = GetHoennPokedexCount(FLAG_GET_CAUGHT);
        StringExpandPlaceholders(gStringVar4, gText_ContinueMenuPokedex);
        AddTextPrinterParameterized3(windowId, FONT_SHORT, 0, 30, sTextColor_MenuInfo, TEXT_SKIP_DRAW, gStringVar4);
        ConvertIntToDecimalStringN(str, dexCount, STR_CONV_MODE_LEFT_ALIGN, 3);
        AddTextPrinterParameterized3(windowId, FONT_SHORT, GetStringRightAlignXOffset(FONT_SHORT, str, 100), 30, sTextColor_MenuInfo, TEXT_SKIP_DRAW, str);
    }
}

static void MainMenu_FormatSavegameBadges(u8 windowId)
{
    u8 str[0x20];
    u8 badgeCount = 0;
    u32 i;

    for (i = FLAG_BADGE01_GET; i < FLAG_BADGE01_GET + NUM_BADGES; i++)
    {
        if (FlagGet(i))
            badgeCount++;
    }
    StringExpandPlaceholders(gStringVar4, gText_ContinueMenuBadges);
    AddTextPrinterParameterized3(windowId, FONT_SHORT, 0x6C, 30, sTextColor_MenuInfo, TEXT_SKIP_DRAW, gStringVar4);
    ConvertIntToDecimalStringN(str, badgeCount, STR_CONV_MODE_LEADING_ZEROS, 1);
    AddTextPrinterParameterized3(windowId, FONT_SHORT, GetStringRightAlignXOffset(FONT_SHORT, str, 0xD0), 30, sTextColor_MenuInfo, TEXT_SKIP_DRAW, str);
}

static void MainMenu_CreatePlayerSprite(u8 windowId)
{
    u8 spriteId = CreateObjectGraphicsSprite(sGenderObjectSprite[gSaveBlock2Ptr->playerGender], SpriteCallbackDummy, 40, 34, 0);
    StartSpriteAnim(&gSprites[spriteId], 0);
    gSprites[spriteId].oam.priority = 0;
    spriteId = CreateSpriteAtEnd(gFieldEffectObjectTemplatePointers[1], 0, 0, 0x94);
    gSprites[spriteId].x = 40;
    gSprites[spriteId].y = 34 + (32 >> 1) - 4;
    gSprites[spriteId].oam.priority = 0;
    gSprites[spriteId].callback = SpriteCallbackDummy;
    gSprites[spriteId].oam.objMode = ST_OAM_OBJ_BLEND;
    gSprites[spriteId].coordOffsetEnabled = TRUE;
}

static void MainMenu_CreatePokeIcons(u8 windowId)
{
    u8 i, x, spriteId;
    u16 species;

    for (i = 0; i < PARTY_SIZE; i++)
    {
        x = 40 + 0x20 * i;
        BlitBitmapToWindow(2, sPokeBorderGfx, x - 32, 50, 32, 32);

        species = GetMonData(&gPlayerParty[i], MON_DATA_SPECIES);
        if (species != SPECIES_NONE)
        {
            if (GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG))
                species = SPECIES_EGG;

            LoadMonIconPalette(species);
            spriteId = CreateMonIcon(species, SpriteCB_MonIcon, x, 70, 1, GetMonData(&gPlayerParty[i], MON_DATA_PERSONALITY), TRUE);
            gSprites[spriteId].oam.priority = 0;
            sContinueMonIconSpriteIds[i] = spriteId;
        }
    }
}

static void LoadMainMenuWindowFrameTiles(u8 bgId, u16 tileOffset)
{
    LoadBgTiles(bgId, sMainMenuFrame_Gfx, 0x120, tileOffset);
    LoadPalette(sMainMenuFrame_Pal, 32, 32);
}

static void DrawMainMenuWindowBorder(const struct WindowTemplate *template, u16 baseTileNum)
{
    u16 r9 = 1 + baseTileNum;
    u16 r10 = 2 + baseTileNum;
    u16 sp18 = 3 + baseTileNum;
    u16 spC = 5 + baseTileNum;
    u16 sp10 = 6 + baseTileNum;
    u16 sp14 = 7 + baseTileNum;
    u16 r6 = 8 + baseTileNum;

    FillBgTilemapBufferRect(template->bg, baseTileNum, template->tilemapLeft - 1, template->tilemapTop - 1, 1, 1, 2);
    FillBgTilemapBufferRect(template->bg, r9, template->tilemapLeft, template->tilemapTop - 1, template->width, 1, 2);
    FillBgTilemapBufferRect(template->bg, r10, template->tilemapLeft + template->width, template->tilemapTop - 1, 1, 1, 2);
    FillBgTilemapBufferRect(template->bg, sp18, template->tilemapLeft - 1, template->tilemapTop, 1, template->height, 2);
    FillBgTilemapBufferRect(template->bg, spC, template->tilemapLeft + template->width, template->tilemapTop, 1, template->height, 2);
    FillBgTilemapBufferRect(template->bg, sp10, template->tilemapLeft - 1, template->tilemapTop + template->height, 1, 1, 2);
    FillBgTilemapBufferRect(template->bg, sp14, template->tilemapLeft, template->tilemapTop + template->height, template->width, 1, 2);
    FillBgTilemapBufferRect(template->bg, r6, template->tilemapLeft + template->width, template->tilemapTop + template->height, 1, 1, 2);
    CopyBgTilemapBufferToVram(template->bg);
}

static void ClearMainMenuWindowTilemap(const struct WindowTemplate *template)
{
    FillBgTilemapBufferRect(template->bg, 0, template->tilemapLeft - 1, template->tilemapTop - 1, template->tilemapLeft + template->width + 1, template->tilemapTop + template->height + 1, 2);
    CopyBgTilemapBufferToVram(template->bg);
}

#undef tTimer

void CreateYesNoMenuParameterized(u8 x, u8 y, u16 baseTileNum, u16 baseBlock, u8 yesNoPalNum, u8 winPalNum)
{
    struct WindowTemplate template = CreateWindowTemplate(0, x + 1, y + 1, 5, 4, winPalNum, baseBlock);
    CreateYesNoMenu(&template, baseTileNum, yesNoPalNum, 0);
}