#include "global.h"
#include "intro_speech.h"
#include "bg.h"
#include "decompress.h"
#include "dma3.h"
#include "field_effect.h"
#include "field_message_box.h"
#include "gpu_regs.h"
#include "list_menu.h"
#include "main.h"
#include "main_menu.h"
#include "menu.h"
#include "menu_helpers.h"
#include "naming_screen.h"
#include "overworld.h"
#include "palette.h"
#include "random.h"
#include "rtc.h"
#include "sound.h"
#include "scanline_effect.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"
#include "text_window.h"
#include "trainer_pokemon_sprites.h"
#include "wallclock.h"
#include "window.h"
#include "constants/rgb.h"
#include "constants/songs.h"
#include "constants/trainers.h"

static const u16 sBirchSpeechBgPals[][16] = {
    INCBIN_U16("graphics/birch_speech/bg0.gbapal"),
    INCBIN_U16("graphics/birch_speech/bg1.gbapal")
};

static const u32 sBirchSpeechShadowGfx[] = INCBIN_U32("graphics/birch_speech/shadow.4bpp.lz");
static const u32 sBirchSpeechBgMap[] = INCBIN_U32("graphics/birch_speech/map.bin.lz");
static const u16 sBirchSpeechBgGradientPal[] = INCBIN_U16("graphics/birch_speech/bg2.gbapal");
static const u16 sBirchSpeechPlatformBlackPal[] = {RGB_BLACK, RGB_BLACK, RGB_BLACK, RGB_BLACK, RGB_BLACK, RGB_BLACK, RGB_BLACK, RGB_BLACK};

static const struct BgTemplate sBirchBgTemplates[2] = {
    {
        .bg = 0,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
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

static const struct WindowTemplate sIntroTextWindowTemplate = {
    .bg = 0,
    .tilemapLeft = 2,
    .tilemapTop = 2,
    .width = 26,
    .height = 16,
    .paletteNum = 15,
    .baseBlock = 0x1
};


static const struct WindowTemplate sIntroSpeechMessageTemplates[] = {
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 15,
        .width = 26,
        .height = 4,
        .paletteNum = 15,
        .baseBlock = 1
    },
    DUMMY_WIN_TEMPLATE
};

static const struct ScrollArrowsTemplate sGenderSelectionArrowsTemplate = {
    .firstArrowType = SCROLL_ARROW_LEFT,
    .firstX = 60,
    .firstY = 66,
    .secondArrowType = SCROLL_ARROW_RIGHT,
    .secondX = 180,
    .secondY = 66,
    .fullyUpThreshold = 0,
    .fullyDownThreshold = 1,
    .tileTag = 111,
    .palTag = 111,
    .palNum = 0,
};

static const u8 *const sText_NewGame_Intro[] = {
    gText_NewGame_IntroText1,
};

static const u8 *const sMalePresetNames[] = {
    gText_DefaultNameStu,
    gText_DefaultNameMilton,
    gText_DefaultNameTom,
    gText_DefaultNameKenny,
    gText_DefaultNameReid,
    gText_DefaultNameJude,
    gText_DefaultNameJaxson,
    gText_DefaultNameEaston,
    gText_DefaultNameWalker,
    gText_DefaultNameTeru,
    gText_DefaultNameJohnny,
    gText_DefaultNameBrett,
    gText_DefaultNameSeth,
    gText_DefaultNameTerry,
    gText_DefaultNameCasey,
    gText_DefaultNameDarren,
    gText_DefaultNameLandon,
    gText_DefaultNameCollin,
    gText_DefaultNameStanley,
    gText_DefaultNameQuincy
};

static const u8 *const sFemalePresetNames[] = {
    gText_DefaultNameKimmy,
    gText_DefaultNameTiara,
    gText_DefaultNameBella,
    gText_DefaultNameJayla,
    gText_DefaultNameAllie,
    gText_DefaultNameLianna,
    gText_DefaultNameSara,
    gText_DefaultNameMonica,
    gText_DefaultNameCamila,
    gText_DefaultNameAubree,
    gText_DefaultNameRuthie,
    gText_DefaultNameHazel,
    gText_DefaultNameNadine,
    gText_DefaultNameTanja,
    gText_DefaultNameYasmin,
    gText_DefaultNameNicola,
    gText_DefaultNameLillie,
    gText_DefaultNameTerra,
    gText_DefaultNameLucy,
    gText_DefaultNameHalie
};

static u8 StartNextTask(void);

static void SetupReturnCB(void);
static void CB2_IntroSpeech(void);
static void VBlankCB_IntroSpeech(void);

// Intro Texts
static void Task_StartDisplayIntroText(u8);
static void Task_DisplayIntroText(u8);

// Gender Selection
static void SpriteCB_GenderSelection(struct Sprite *sprite);
static void Task_StartGenderSelection(u8);
static void Task_GenderSelection(u8);
static void Task_GenderSelection_HandleInput(u8);
static void Task_GenderSelection_WaitForYesNo(u8);
static void Task_GenderSelection_YesNo_FadeOutSprite(u8);
static void Task_GenderSelection_YesNo(u8);
static void Task_GenderSelection_ProcessNameYesNoMenu(u8);
static void Task_GenderSelection_StartReturnToSelection(u8 taskId);
static void Task_GenderSelection_YesNo_FadeInSprite(u8);
static void Task_GenderSelection_ReturnToSelection(u8);
static void Task_GenderSelection_Cleanup(u8);

// Naming Screen
static void Task_StartNamingScreen(u8);
static void Task_NamingScreen(u8);
static void NamingScreen_SetDefaultName(u8);
static void CB2_ReturnFromNamingScreen(void);
static void Task_NamingScreen_WaitForYesNo(u8);
static void Task_NamingScreen_YesNo(u8);
static void Task_NamingScreen_ProcessNameYesNoMenu(u8);
static void Task_NamingScreen_ReturnToNaming(u8);
static void Task_NamingScreen_Cleanup(u8);

// Wallclock
static void Task_StartWallclock(u8);
static void CB2_ReturnFromWallclock(void);
static void Task_Wallclock_Cleanup(u8);

static void ShowDialogueWindow(u8, u8);
static void CreateDialogueWindowBorder(u8, u8, u8, u8, u8, u8);
static void ClearDialogueWindow(u8);

static const TaskFunc sTaskFuncs[] = {
    Task_StartDisplayIntroText,
    Task_StartGenderSelection,
    Task_StartNamingScreen,
    Task_StartWallclock
};

EWRAM_DATA static u8 sTaskIdx = 0;

static u8 StartNextTask(void)
{
    if (sTaskIdx < NELEMS(sTaskFuncs))
        return CreateTask(sTaskFuncs[sTaskIdx++], 0);

    return NUM_TASKS;
}

static void SetupReturnCB(void)
{
    InitBgsFromTemplates(0, sBirchBgTemplates, 2);
    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);
    FillPalette(0, 0, PLTT_SIZE);
    ResetPaletteFade();
    ResetTasks();
    ScanlineEffect_Stop();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetAllPicSprites();
    ShowBg(0);
    ShowBg(1);
    LoadPalette(GetOverworldTextboxPalettePtr(), 0xF0, 32);

    SetVBlankCallback(VBlankCB_IntroSpeech);
    SetMainCallback2(CB2_IntroSpeech);
}

static void CB2_IntroSpeech(void)
{
    RunTasks();
    AnimateSprites();
    BuildOamBuffer();
    DoScheduledBgTilemapCopiesToVram();
    UpdatePaletteFade();
}

static void VBlankCB_IntroSpeech(void)
{
    LoadOam();
    ProcessSpriteCopyRequests();
    TransferPlttBuffer();
}

void CB2_NewGameIntro(void)
{
    ResetBgsAndClearDma3BusyFlags(0);

    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    SetGpuReg(REG_OFFSET_DISPCNT, DISPCNT_OBJ_ON | DISPCNT_OBJ_1D_MAP);

    InitBgsFromTemplates(0, sBirchBgTemplates, 2);
    SetVBlankCallback(NULL);

    SetGpuReg(REG_OFFSET_BG2CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG2HOFS, 0);
    SetGpuReg(REG_OFFSET_BG2VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);

    SetGpuReg(REG_OFFSET_WININ, WININ_WIN0_BG0 | WININ_WIN0_OBJ);
    SetGpuReg(REG_OFFSET_WINOUT, WINOUT_WIN01_BG0 | WINOUT_WIN01_OBJ | WINOUT_WIN01_CLR);
    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT1_BG0 | BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_DARKEN);
    SetGpuReg(REG_OFFSET_BLDY, 0);

    DmaFill16(3, 0, VRAM, VRAM_SIZE);
    DmaFill32(3, 0, OAM, OAM_SIZE);
    DmaFill16(3, 0, PLTT, PLTT_SIZE);

    ResetPaletteFade();
    ResetTasks();

    ScanlineEffect_Stop();
    ResetSpriteData();
    FreeAllSpritePalettes();
    ResetAllPicSprites();

    while (FreeTempTileDataBuffersIfPossible())
        ;

    CopyBgTilemapBufferToVram(0);

    ShowBg(0);
    ShowBg(1);

    LoadPalette(GetOverworldTextboxPalettePtr(), 0xF0, 32);

    EnableInterrupts(1);
    SetMainCallback2(CB2_IntroSpeech);
    SetVBlankCallback(VBlankCB_IntroSpeech);
    StartNextTask();
}

// Intro text

#define tState data[0]
#define tTextIdx data[1]
#define tWindowId data[2]
#define tTimer data[3]

static void Task_StartDisplayIntroText(u8 taskId)
{
    u16 *data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        tWindowId = AddWindow(&sIntroTextWindowTemplate);
        ClearDialogueWindow(tWindowId);
        FillWindowPixelBuffer(tWindowId, PIXEL_FILL(0));
        PutWindowTilemap(tWindowId);
        AddTextPrinterParameterized2(tWindowId, FONT_NORMAL, sText_NewGame_Intro[tTextIdx++], 0, NULL, TEXT_COLOR_WHITE, TEXT_COLOR_TRANSPARENT, TEXT_COLOR_TRANSPARENT);
        ScheduleBgCopyTilemapToVram(0);
        BeginNormalPaletteFade(PALETTES_ALL, 0, 16, 0, RGB_BLACK);
        gTasks[taskId].func = Task_DisplayIntroText;
    }
}

static void Task_DisplayIntroText(u8 taskId)
{
    u16 *data = gTasks[taskId].data;

    if (gPaletteFade.active)
        return;

    switch (tState)
    {
    case 0:
        if (JOY_NEW(A_BUTTON))
            tState++;
        break;
    case 1:
        tTimer++;
        SetGpuReg(REG_OFFSET_BLDY, tTimer);
        if (tTimer >= 16)
        {
            if (tTextIdx >= NELEMS(sText_NewGame_Intro))
                tState = 5;
            else
                tState++;
        }
        break;
    case 2:
        SetGpuReg(REG_OFFSET_BLDY, 16);
        FillWindowPixelBuffer(tWindowId, PIXEL_FILL(0));
        AddTextPrinterParameterized2(tWindowId, FONT_NORMAL, sText_NewGame_Intro[tTextIdx++], 0, NULL, TEXT_COLOR_WHITE, TEXT_COLOR_TRANSPARENT, TEXT_COLOR_TRANSPARENT);
        ScheduleBgCopyTilemapToVram(0);
        tState++;
        break;
    case 3:
        tTimer--;
        SetGpuReg(REG_OFFSET_BLDY, tTimer);
        if (tTimer <= 0)
            tState++;
        break;
    case 4:
        SetGpuReg(REG_OFFSET_BLDY, 0);
        tTimer = 0;
        tState = 0;
        break;
    case 5:
        ClearWindowTilemap(tWindowId);
        RemoveWindow(tWindowId);
        ScheduleBgCopyTilemapToVram(0);
        SetGpuReg(REG_OFFSET_BLDY, 0);
        StartNextTask();
        DestroyTask(taskId);
        break;
    }
}

#undef tTextIdx
#undef tWindowId
#undef tTimer

// Gender Selection

#define tGender data[1]
#define tBrendanSpriteId data[2]
#define tMaySpriteId data[3]
#define tFadeTimer data[4]
#define tScrollTaskId data[5]

static void SpriteCB_GenderSelection(struct Sprite *sprite)
{
    s16 *data = sprite->data;

    if (gTasks[data[0]].tGender == data[1])
        sprite->invisible = FALSE;
    else
        sprite->invisible = TRUE;
}

static const u8 sText_SelectGender[] = _("Select your character.");

static void Task_StartGenderSelection(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0:
        tGender = MALE;
        InitWindows(sIntroSpeechMessageTemplates);
        LoadMessageBoxGfx(0, 0xFC, 0xF0);
        LoadUserWindowBorderGfxOnBg(0, 0xF3, 32);
        tState++;
        break;
    case 1:
        tBrendanSpriteId = CreateTrainerSprite(TRAINER_PIC_BRENDAN, 120, 60, 0, &gDecompressionBuffer[0]);
        gSprites[tBrendanSpriteId].invisible = TRUE;
        gSprites[tBrendanSpriteId].oam.priority = 0;
        gSprites[tBrendanSpriteId].data[0] = taskId;
        gSprites[tBrendanSpriteId].data[1] = MALE;
        tState++;
        break;
    case 2:
        tMaySpriteId = CreateTrainerSprite(TRAINER_PIC_MAY, 120, 60, 0, &gDecompressionBuffer[0x800]);
        gSprites[tMaySpriteId].invisible = TRUE;
        gSprites[tMaySpriteId].oam.priority = 0;
        gSprites[tMaySpriteId].data[0] = taskId;
        gSprites[tMaySpriteId].data[1] = FEMALE;
        tState++;
        break;
    case 3:
        gTasks[taskId].func = Task_GenderSelection;
        tState = 0;
        break;
    }
}

static const u8 sText_GenderBoy[] = _("So you're a boy?");
static const u8 sText_GenderGirl[] = _("So you're a girl?");

static void Task_GenderSelection(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (tState)
    {
    case 0:
        ClearDialogueWindow(0);
        ShowDialogueWindow(0, TRUE);
        StringExpandPlaceholders(gStringVar4, sText_SelectGender);
        AddTextPrinterForMessage(FALSE);
        tState++;
        break;
    case 1:
        if (!RunTextPrintersAndIsPrinter0Active())
        {
            BeginNormalPaletteFade(PALETTES_OBJECTS, 0, 16, 0, RGB_BLACK);
            gSprites[tBrendanSpriteId].invisible = FALSE;
            gSprites[tMaySpriteId].invisible = FALSE;
            gSprites[tBrendanSpriteId].callback = SpriteCB_GenderSelection;
            gSprites[tMaySpriteId].callback = SpriteCB_GenderSelection;
            tState++;
        }
        break;
    case 2:
        if (!gPaletteFade.active)
        {
            tScrollTaskId = AddScrollIndicatorArrowPair(&sGenderSelectionArrowsTemplate, &tGender);
            gTasks[taskId].func = Task_GenderSelection_HandleInput;
        }
        break;
    }
}

static void Task_GenderSelection_HandleInput(u8 taskId)
{
    if (JOY_NEW(DPAD_LEFT))
    {
        gTasks[taskId].tGender = MALE;
    }
    else if (JOY_NEW(DPAD_RIGHT))
    {
        gTasks[taskId].tGender = FEMALE;
    }
    else if (JOY_NEW(A_BUTTON))
    {
        gTasks[taskId].tState = 0;
        gTasks[taskId].func = Task_GenderSelection_WaitForYesNo;
    }
}

static void Task_GenderSelection_WaitForYesNo(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    ClearDialogueWindow(0);
    if (tGender == MALE)
        StringExpandPlaceholders(gStringVar4, sText_GenderBoy);
    else
        StringExpandPlaceholders(gStringVar4, sText_GenderGirl);
    AddTextPrinterForMessage(FALSE);
    RemoveScrollIndicatorArrowPair(tScrollTaskId);
    gTasks[taskId].func = Task_GenderSelection_YesNo;
}

static void Task_GenderSelection_YesNo(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (!RunTextPrintersAndIsPrinter0Active())
    {
        if (gTasks[taskId].tGender == MALE)
        {
            gSprites[tMaySpriteId].invisible = TRUE;
            CreateYesNoMenuParameterized(22, 8, 0xF3, 0xDF, 2, 15);
        }
        else
        {
            gSprites[tBrendanSpriteId].invisible = TRUE;
            CreateYesNoMenuParameterized(1, 8, 0xF3, 0xDF, 2, 15);
        }
        gTasks[taskId].func = Task_GenderSelection_ProcessNameYesNoMenu;
    }
}

static void Task_GenderSelection_ProcessNameYesNoMenu(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
        case 0:
            gSaveBlock2Ptr->playerGender = tGender;
            gTasks[taskId].func = Task_GenderSelection_Cleanup;
            PlaySE(SE_SELECT);
            break;
        case MENU_B_PRESSED:
        case 1:
            gTasks[taskId].func = Task_GenderSelection_StartReturnToSelection;
            PlaySE(SE_SELECT);
            break;
    }
}

static void Task_GenderSelection_StartReturnToSelection(u8 taskId)
{
    s16 *data = gTasks[taskId].data;   

    gSprites[tBrendanSpriteId].invisible = FALSE;
    gSprites[tMaySpriteId].invisible = FALSE;
    ClearDialogueWindow(0);
    ShowDialogueWindow(0, TRUE);
    StringExpandPlaceholders(gStringVar4, sText_SelectGender);
    AddTextPrinterForMessage(0);

    gTasks[taskId].func = Task_GenderSelection_ReturnToSelection;
}

static void Task_GenderSelection_ReturnToSelection(u8 taskId)
{
    if (!RunTextPrintersAndIsPrinter0Active())
    {
        gTasks[taskId].tScrollTaskId = AddScrollIndicatorArrowPair(&sGenderSelectionArrowsTemplate, &gTasks[taskId].tGender);
        gTasks[taskId].func = Task_GenderSelection_HandleInput;
    }
}

static void Task_GenderSelection_Cleanup(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (!gPaletteFade.active)
    {
        DestroySpriteAndFreeResources(&gSprites[tBrendanSpriteId]);
        DestroySpriteAndFreeResources(&gSprites[tMaySpriteId]);
        StartNextTask();
        DestroyTask(taskId);
    }
}

#undef tGender
#undef tBrendanSpriteId
#undef tMaySpriteId
#undef tFadeTimer

// Naming Screen

#define tPlayerSpriteId data[1]

static const u8 sText_WhatIsYourName[] = _("What is your name?\p");

static void Task_StartNamingScreen(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        ClearDialogueWindow(0);
        ShowDialogueWindow(0, TRUE);
        StringExpandPlaceholders(gStringVar4, sText_WhatIsYourName);
        AddTextPrinterForMessage(0);

        if (gSaveBlock2Ptr->playerGender == MALE)
            gTasks[taskId].tPlayerSpriteId = CreateTrainerSprite(TRAINER_PIC_BRENDAN, 120, 60, 0, &gDecompressionBuffer[0]);
        else
            gTasks[taskId].tPlayerSpriteId = CreateTrainerSprite(TRAINER_PIC_MAY, 120, 60, 0, &gDecompressionBuffer[0]);

        gTasks[taskId].func = Task_NamingScreen;
    }
}

static void Task_NamingScreen(u8 taskId)
{
    s16 *data = gTasks[taskId].data;

    if (RunTextPrintersAndIsPrinter0Active())
        return;

    switch (tState)
    {
    case 0:
        BeginNormalPaletteFade(PALETTES_ALL, 0, 0, 16, RGB_BLACK);
        tState++;
        break;
    case 1:
        if (!gPaletteFade.active)
        {
            FreeAllWindowBuffers();
            NamingScreen_SetDefaultName(Random() % 20);
            DoNamingScreen(NAMING_SCREEN_PLAYER, gSaveBlock2Ptr->playerName, gSaveBlock2Ptr->playerGender, 0, 0, CB2_ReturnFromNamingScreen);
            DestroyTask(taskId);
        }
        break;
    }
}

static void NamingScreen_SetDefaultName(u8 nameId)
{
    const u8 *name;
    u8 i;

    if (gSaveBlock2Ptr->playerGender == MALE)
        name = sMalePresetNames[nameId];
    else
        name = sFemalePresetNames[nameId];
    for (i = 0; i < PLAYER_NAME_LENGTH; i++)
        gSaveBlock2Ptr->playerName[i] = name[i];

    gSaveBlock2Ptr->playerName[PLAYER_NAME_LENGTH] = EOS;
}

static void CB2_ReturnFromNamingScreen(void)
{
    u8 taskId;
    SetupReturnCB();
    taskId = CreateTask(Task_NamingScreen_WaitForYesNo, 0);
    gTasks[taskId].tPlayerSpriteId = CreateTrainerSprite(TRAINER_PIC_BRENDAN, 120, 60, 0, &gDecompressionBuffer[0]);
}

static void Task_NamingScreen_WaitForYesNo(u8 taskId)
{
    InitWindows(sIntroSpeechMessageTemplates);
    LoadMessageBoxGfx(0, 0xFC, 0xF0);
    LoadUserWindowBorderGfxOnBg(0, 0xF3, 32);
    ClearDialogueWindow(0);
    ShowDialogueWindow(0, TRUE);
    StringExpandPlaceholders(gStringVar4, gText_Birch_SoItsPlayer);
    AddTextPrinterForMessage(0);
    gTasks[taskId].func = Task_NamingScreen_YesNo;
}

static void Task_NamingScreen_YesNo(u8 taskId)
{
    if (!RunTextPrintersAndIsPrinter0Active())
    {
        CreateYesNoMenuParameterized(1, 8, 0xF3, 0xDF, 2, 15);
        gTasks[taskId].func = Task_NamingScreen_ProcessNameYesNoMenu;
    }
}

static void Task_NamingScreen_ProcessNameYesNoMenu(u8 taskId)
{
    switch (Menu_ProcessInputNoWrapClearOnChoose())
    {
        case 0:
            gTasks[taskId].func = Task_NamingScreen_Cleanup;
            PlaySE(SE_SELECT);
            break;
        case MENU_B_PRESSED:
        case 1:
            gTasks[taskId].func = Task_NamingScreen_ReturnToNaming;
            PlaySE(SE_SELECT);
    }
}

static void Task_NamingScreen_ReturnToNaming(u8 taskId)
{
    if (!gPaletteFade.active)
    {
        ClearDialogueWindow(0);
        ShowDialogueWindow(0, TRUE);
        StringExpandPlaceholders(gStringVar4, sText_WhatIsYourName);
        AddTextPrinterForMessage(0);
        gTasks[taskId].func = Task_NamingScreen;
    }
}

static void Task_NamingScreen_Cleanup(u8 taskId)
{
    DestroySpriteAndFreeResources(&gSprites[gTasks[taskId].tPlayerSpriteId]);
    ClearDialogWindowAndFrameToTransparent(0, TRUE);
    StartNextTask();
    DestroyTask(taskId);
}

// Wallclock

static void Task_StartWallclock(u8 taskId)
{
    SetMainCallback2(CB2_StartWallClock);
    gMain.savedCallback = CB2_ReturnFromWallclock;
}

static void CB2_ReturnFromWallclock(void)
{
    SetupReturnCB();
    CreateTask(Task_Wallclock_Cleanup, 0);
}

static void Task_Wallclock_Cleanup(u8 taskId)
{
    ClearDialogWindowAndFrameToTransparent(0, TRUE);
    // StartNextTask();
    SetMainCallback2(CB2_NewGame);
    DestroyTask(taskId);
}

#undef tState
#undef tPlayerSpriteId

#define tIsDoneFadingSprites data[5]

/* New implementation goes here. */

#define tMainTask data[0]
#define tAlphaCoeff1 data[1]
#define tAlphaCoeff2 data[2]
#define tDelay data[3]
#define tDelayTimer data[4]

static void Task_FadeOutTarget1InTarget2(u8 taskId)
{
    int alphaCoeff2;

    if (gTasks[taskId].tAlphaCoeff1 == 0)
    {
        gTasks[gTasks[taskId].tMainTask].tIsDoneFadingSprites = TRUE;
        DestroyTask(taskId);
    }
    else if (gTasks[taskId].tDelayTimer)
    {
        gTasks[taskId].tDelayTimer--;
    }
    else
    {
        gTasks[taskId].tDelayTimer = gTasks[taskId].tDelay;
        gTasks[taskId].tAlphaCoeff1--;
        gTasks[taskId].tAlphaCoeff2++;
        alphaCoeff2 = gTasks[taskId].tAlphaCoeff2 << 8;
        SetGpuReg(REG_OFFSET_BLDALPHA, gTasks[taskId].tAlphaCoeff1 + alphaCoeff2);
    }
}

static void StartFadeOutTarget1InTarget2(u8 taskId, u8 delay)
{
    u8 taskId2;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_OBJ);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(16, 0));
    SetGpuReg(REG_OFFSET_BLDY, 0);
    gTasks[taskId].tIsDoneFadingSprites = 0;
    taskId2 = CreateTask(Task_FadeOutTarget1InTarget2, 0);
    gTasks[taskId2].tMainTask = taskId;
    gTasks[taskId2].tAlphaCoeff1 = 16;
    gTasks[taskId2].tAlphaCoeff2 = 0;
    gTasks[taskId2].tDelay = delay;
    gTasks[taskId2].tDelayTimer = delay;
}

static void Task_FadeInTarget1OutTarget2(u8 taskId)
{
    int alphaCoeff2;

    if (gTasks[taskId].tAlphaCoeff1 == 16)
    {
        gTasks[gTasks[taskId].tMainTask].tIsDoneFadingSprites = TRUE;
        DestroyTask(taskId);
    }
    else if (gTasks[taskId].tDelayTimer)
    {
        gTasks[taskId].tDelayTimer--;
    }
    else
    {
        gTasks[taskId].tDelayTimer = gTasks[taskId].tDelay;
        gTasks[taskId].tAlphaCoeff1++;
        gTasks[taskId].tAlphaCoeff2--;
        alphaCoeff2 = gTasks[taskId].tAlphaCoeff2 << 8;
        SetGpuReg(REG_OFFSET_BLDALPHA, gTasks[taskId].tAlphaCoeff1 + alphaCoeff2);
    }
}

static void StartFadeInTarget1OutTarget2(u8 taskId, u8 delay)
{
    u8 taskId2;

    SetGpuReg(REG_OFFSET_BLDCNT, BLDCNT_TGT2_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT1_OBJ);
    SetGpuReg(REG_OFFSET_BLDALPHA, BLDALPHA_BLEND(0, 16));
    SetGpuReg(REG_OFFSET_BLDY, 0);
    gTasks[taskId].tIsDoneFadingSprites = 0;
    taskId2 = CreateTask(Task_FadeInTarget1OutTarget2, 0);
    gTasks[taskId2].tMainTask = taskId;
    gTasks[taskId2].tAlphaCoeff1 = 0;
    gTasks[taskId2].tAlphaCoeff2 = 16;
    gTasks[taskId2].tDelay = delay;
    gTasks[taskId2].tDelayTimer = delay;
}

#undef tMainTask
#undef tAlphaCoeff1
#undef tAlphaCoeff2
#undef tDelay
#undef tDelayTimer

#undef tIsDoneFadingSprites

#define tMainTask data[0]
#define tPalIndex data[1]
#define tDelayBefore data[2]
#define tDelay data[3]
#define tDelayTimer data[4]

static void Task_FadePlatformIn(u8 taskId)
{
    if (gTasks[taskId].tDelayBefore)
    {
        gTasks[taskId].tDelayBefore--;
    }
    else if (gTasks[taskId].tPalIndex == 8)
    {
        DestroyTask(taskId);
    }
    else if (gTasks[taskId].tDelayTimer)
    {
        gTasks[taskId].tDelayTimer--;
    }
    else
    {
        gTasks[taskId].tDelayTimer = gTasks[taskId].tDelay;
        gTasks[taskId].tPalIndex++;
        LoadPalette(&sBirchSpeechBgGradientPal[gTasks[taskId].tPalIndex], 1, 16);
    }
}

static void StartFadePlatformIn(u8 taskId, u8 delay)
{
    u8 taskId2;

    taskId2 = CreateTask(Task_FadePlatformIn, 0);
    gTasks[taskId2].tMainTask = taskId;
    gTasks[taskId2].tPalIndex = 0;
    gTasks[taskId2].tDelayBefore = 8;
    gTasks[taskId2].tDelay = delay;
    gTasks[taskId2].tDelayTimer = delay;
}

static void Task_FadePlatformOut(u8 taskId)
{
    if (gTasks[taskId].tDelayBefore)
    {
        gTasks[taskId].tDelayBefore--;
    }
    else if (gTasks[taskId].tPalIndex == 0)
    {
        DestroyTask(taskId);
    }
    else if (gTasks[taskId].tDelayTimer)
    {
        gTasks[taskId].tDelayTimer--;
    }
    else
    {
        gTasks[taskId].tDelayTimer = gTasks[taskId].tDelay;
        gTasks[taskId].tPalIndex--;
        LoadPalette(&sBirchSpeechBgGradientPal[gTasks[taskId].tPalIndex], 1, 16);
    }
}

static void StartFadePlatformOut(u8 taskId, u8 delay)
{
    u8 taskId2;

    taskId2 = CreateTask(Task_FadePlatformOut, 0);
    gTasks[taskId2].tMainTask = taskId;
    gTasks[taskId2].tPalIndex = 8;
    gTasks[taskId2].tDelayBefore = 8;
    gTasks[taskId2].tDelay = delay;
    gTasks[taskId2].tDelayTimer = delay;
}

#undef tMainTask
#undef tPalIndex
#undef tDelayBefore
#undef tDelay
#undef tDelayTimer

static void ShowDialogueWindow(u8 windowId, u8 copyToVram)
{
    DrawDialogFrameWithCustomTileAndPalette(windowId, copyToVram, 0xFC, GetWindowAttribute(windowId, WINDOW_PALETTE_NUM));
}

static void CreateDialogueWindowBorder(u8 bg, u8 tilemapLeft, u8 tilemapTop, u8 width, u8 height, u8 palNum)
{
    s8 i, j;

    for (i = -2; i < width + 1; i++)
    {
        FillBgTilemapBufferRect(bg, 0xFC + 1, tilemapLeft + i, tilemapTop - 1, 1, 1, palNum);
        FillBgTilemapBufferRect(bg, BG_TILE_V_FLIP(0xFC + 1), tilemapLeft + i, tilemapTop + height, 1, 1, palNum);

        for (j = 0; j < height; j++)
            FillBgTilemapBufferRect(bg, 0xFC, tilemapLeft + i, tilemapTop + j, 1, 1, palNum);
    }
}

static void ClearDialogueWindow(u8 windowId)
{
    FillWindowPixelBuffer(windowId, PIXEL_FILL(1));
    CopyWindowToVram(windowId, COPYWIN_GFX);
}