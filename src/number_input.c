#include "global.h"
#include "event_data.h"
#include "field_message_box.h"
#include "main.h"
#include "menu.h"
#include "number_input.h"
#include "script.h"
#include "string_util.h"
#include "strings.h"
#include "task.h"
#include "text.h"

static void DisplayNumberInputBox(void);
static void InitNumberInput(u8 taskId);
static s16 MoveCursor(s16 cursor, s8 delta);
static void ChangeNumber(s16 cursor, s16 *digits, s8 delta);
static u16 ConvertDigitsToNum(s16 *digits);
static void Task_HandleNumberInput(u8 taskId);

static const u8 sDigits[] = _("0123456789");

bool8 ScriptMenu_Number(void)
{
    u8 taskId;

    if (FuncIsActiveTask(Task_HandleNumberInput) == TRUE)
    {
        return FALSE;
    }
    else
    {
        gSpecialVar_Result = 0xFF;
        DisplayNumberInputBox();
        taskId = CreateTask(Task_HandleNumberInput, 0x50);
        InitNumberInput(taskId);
        return TRUE;
    }
}

static void DisplayNumberInputBox(void)
{
    LoadMessageBoxAndBorderGfx();
    DrawDialogueFrame(0, TRUE);
}

#define tCursorPos data[0]
#define tDoRedraw data[1]
#define tDigits (data + 2)

static void InitNumberInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data;
    tCursorPos = 3;
    tDoRedraw = 1;
    CpuFastFill16(0, tDigits, 4);
}

static s16 MoveCursor(s16 cursor, s8 delta)
{
    s16 newCursor = cursor + delta;

    if (newCursor < 0)
        return 3;
    else if (newCursor > 3)
        return 0;

    return newCursor;
}

static void ChangeNumber(s16 cursor, s16 *digits, s8 delta)
{
    s16 digit = digits[cursor];
    s16 newDigit = digit + delta;

    if (newDigit < 0)
        digits[cursor] = 9;
    else if (newDigit > 9)
        digits[cursor] = 0;
    else
        digits[cursor] = newDigit;
}

static u16 ConvertDigitsToNum(s16 *digits)
{
    u8 i;
    u16 num = 0;

    for (i = 0; i < 4; i++)
    {
        num *= 10;
        num += digits[i];
    }

    return num;
}

static void Task_HandleNumberInput(u8 taskId)
{
    s16 *data = gTasks[taskId].data, i;
    u8 *t;

    if (tDoRedraw)
    {
        t = gStringVar1;

        for (i = 0; i < tCursorPos; i++)
            *(t++) = sDigits[tDigits[i]];

        *(t++) = CHAR_RIGHT_ARROW;

        for (i = tCursorPos; i < 4; i++)
            *(t++) = sDigits[tDigits[i]];
        
        *t = EOS;

        AddTextPrinterParameterized(0, FONT_NORMAL, gStringVar1, 0, 1, 0, NULL);
        tDoRedraw = 0;
    }

    if (JOY_NEW(A_BUTTON))
    {
        gSpecialVar_Result = ConvertDigitsToNum(tDigits);
        ScriptContext_Enable();
        DestroyTask(taskId);
        HideFieldMessageBox();
    }

    if (JOY_NEW(DPAD_UP))
    {
        ChangeNumber(tCursorPos, tDigits, 1);
        tDoRedraw = 1;
    }

    else if (JOY_NEW(DPAD_DOWN))
    {
        ChangeNumber(tCursorPos, tDigits, -1);
        tDoRedraw = 1;
    }

    if (JOY_NEW(DPAD_LEFT))
    {
        tCursorPos = MoveCursor(tCursorPos, -1);
        tDoRedraw = 1;
    }

    else if (JOY_NEW(DPAD_RIGHT))
    {
        tCursorPos = MoveCursor(tCursorPos, 1);
        tDoRedraw = 1;
    }
}

#undef tCursorOis
#undef tDoRedraw
#undef tDigits
