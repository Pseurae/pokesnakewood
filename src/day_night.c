#include "global.h"
#include "day_night.h"
#include "sprite.h"
#include "decompress.h"
#include "fieldmap.h"
#include "field_weather.h"
#include "overworld.h"
#include "palette.h"
#include "rtc.h"
#include "constants/rgb.h"

#define TINT_MORNING  Q_8_8(0.7), Q_8_8(0.7), Q_8_8(0.9)
#define TINT_DAY      Q_8_8(1.0), Q_8_8(1.0), Q_8_8(1.0)
#define TINT_NIGHT    Q_8_8(0.6), Q_8_8(0.6), Q_8_8(0.92)

// From CrystalDust
static const u16 sTimeOfDayTints[][3] = {
    [0] =   { TINT_NIGHT },
    [1] =   { TINT_NIGHT },
    [2] =   { TINT_NIGHT },
    [3] =   { TINT_NIGHT },
    [4] =   { Q_8_8(0.6), Q_8_8(0.65), Q_8_8(1.0) },
    [5] =   { TINT_MORNING },
    [6] =   { TINT_MORNING },
    [7] =   { TINT_MORNING },
    [8] =   { Q_8_8(0.9), Q_8_8(0.85), Q_8_8(1.0) },
    [9] =   { Q_8_8(1.0), Q_8_8(0.9), Q_8_8(1.0) },
    [10] =  { TINT_DAY },
    [11] =  { TINT_DAY },
    [12] =  { TINT_DAY },
    [13] =  { TINT_DAY },
    [14] =  { TINT_DAY },
    [15] =  { TINT_DAY },
    [16] =  { TINT_DAY },
    [17] =  { Q_8_8(1.0), Q_8_8(0.98), Q_8_8(0.9) },
    [18] =  { Q_8_8(0.9), Q_8_8(0.7), Q_8_8(0.67) },
    [19] =  { Q_8_8(0.75), Q_8_8(0.66), Q_8_8(0.77) },
    [20] =  { Q_8_8(0.7), Q_8_8(0.63), Q_8_8(0.82) },
    [21] =  { TINT_NIGHT },
    [22] =  { TINT_NIGHT },
    [23] =  { TINT_NIGHT },
};

EWRAM_DATA struct DayNight gDayNight = { 0 };
ALIGNED(4) EWRAM_DATA static u16 sPlttBufferDayNight[PLTT_BUFFER_SIZE] = { 0 };

/*
LoadTilesetPalette for tilesets.
PatchObjectPalette for objects.

DrawMainBattleBackground for battle background.
*/

void FillDNPalette(u16 offset, u16 size)
{
    CpuFill16(RGB_BLACK, &sPlttBufferDayNight[offset], size);
}

static bool8 BlendColors(const u16 *src1, const u16 *src2, u8 coeff)
{
    if (src1[0] != src2[0]
     && src1[1] != src2[1]
     && src1[2] != src2[2])
    {
        gDayNight.tint[0] = src1[0] + ((src2[0] - src1[0]) * coeff) / 60;
        gDayNight.tint[1] = src1[1] + ((src2[1] - src1[1]) * coeff) / 60;
        gDayNight.tint[2] = src1[2] + ((src2[2] - src1[2]) * coeff) / 60;
        return TRUE;
    }

    memcpy(gDayNight.tint, src1, sizeof(gDayNight.tint));
    return FALSE;
}

static bool8 IsDayNightApplicable(void)
{
    return !IsMapTypeIndoors(gMapHeader.mapType);
}

static void UpdateCurrentTint(void)
{
    u16 hours, minutes;
    const u16 *currTint, *nextTint;

    RtcCalcLocalTime();

    if (gDayNight.shouldOverrideTime)
    {
        hours = gDayNight.overrideTime[0];
        minutes = gDayNight.overrideTime[1];
    }
    else
    {
        hours = gLocalTime.hours;
        minutes = gLocalTime.minutes;
    }

    currTint = sTimeOfDayTints[hours];
    nextTint = sTimeOfDayTints[(hours + 1) % 24];

    BlendColors(currTint, nextTint, minutes);
}

static void ApplyNormalPalette(void)
{
    u16 color;
    s32 i;

    for (i = 0; i < PLTT_BUFFER_SIZE; i++)
    {
        color = sPlttBufferDayNight[i];
        if (!color) continue;
        gPlttBufferUnfaded[i] = color;
        gPlttBufferFaded[i] = color;
    }
}

static void ApplyDayNightPalette(u16 offset, u16 size)
{
    u16 color, *tint;
    s32 r, g, b, i;

    UpdateCurrentTint();

    tint = gDayNight.tint;

    for (i = 0; i < size; i++)
    {
        color = sPlttBufferDayNight[offset + i];

        if (!color) continue;

        r = GET_R(color);
        g = GET_G(color);
        b = GET_B(color);

        r = (u16)((tint[0] * r)) >> 8;
        g = (u16)((tint[1] * g)) >> 8;
        b = (u16)((tint[2] * b)) >> 8;

        if (r > 31)
            r = 31;
        if (g > 31)
            g = 31;
        if (b > 31)
            b = 31;

        gPlttBufferUnfaded[offset + i] = RGB2(r, g, b);
    }
}

void UpdateDayNightPalettes(void)
{
    u8 state = gDayNight.tintState;

    if (!IsDayNightApplicable())
        return;

    if (!gDayNight.hasTinted)
        return;

    if (gDayNight.isDisabled)
        state = DN_TINT_STATE_RESET;

    switch (state)
    {
    case DN_TINT_STATE_BG:
        ApplyDayNightPalette(0, BG_PLTT_SIZE >> 1);
        gDayNight.tintState++;
        break;
    case DN_TINT_STATE_OBJ:
        ApplyDayNightPalette(BG_PLTT_SIZE >> 1, OBJ_PLTT_SIZE >> 1);
        gDayNight.tintState++;
        break;
    case DN_TINT_STATE_RESET:
        ApplyNormalPalette();
        gDayNight.tintState = DN_TINT_STATE_BG;
        gDayNight.hasTinted = FALSE;
        break;
    default:
        if (gWeatherPtr->palProcessingState == WEATHER_PAL_STATE_IDLE && !gPaletteFade.active)
            CpuFastCopy(gPlttBufferUnfaded, gPlttBufferFaded, PLTT_SIZE);
        gDayNight.tintState = DN_TINT_STATE_BG;
        break;
    }
}

void LoadDNPalette(const void *src, u16 offset, u16 size)
{
    CpuCopy16(src, &sPlttBufferDayNight[offset], size);
    ApplyGlobalTintToPaletteEntries(sPlttBufferDayNight + offset, size);
    CpuCopy16(src, &gPlttBufferUnfaded[offset], size);

    if (IsDayNightApplicable() && !gDayNight.isDisabled)
    {
        ApplyDayNightPalette(offset, size >> 1);
        gDayNight.hasTinted = TRUE;
    }

    CpuCopy16(&gPlttBufferUnfaded[offset], &gPlttBufferFaded[offset], size);
}

void LoadCompressedDNPalette(const u32 *src, u16 offset, u16 size)
{
    LZDecompressWram(src, gPaletteDecompressionBuffer);
    LoadDNPalette(gPaletteDecompressionBuffer, offset, size);
}

u8 LoadSpriteDNPalette(const struct SpritePalette *palette)
{
    u8 index = IndexOfSpritePaletteTag(palette->tag);
    if (index != 0xFF)
        return index;

    index = AllocSpritePalette(palette->tag);
    if (index != 0xFF)
        LoadDNPalette(palette->data, index * 16 + 0x100, 32);

    return index;
}

void LoadCompressedSpriteDNPalette(const struct CompressedSpritePalette *src)
{
    struct SpritePalette dest;

    LZ77UnCompWram(src->data, gDecompressionBuffer);
    dest.data = (void *) gDecompressionBuffer;
    dest.tag = src->tag;
    LoadSpriteDNPalette(&dest);
}