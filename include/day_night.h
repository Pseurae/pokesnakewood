#ifndef GUARD_DAY_NIGHT_H
#define GUARD_DAY_NIGHT_H

#include "palette.h"

enum {
    DN_TINT_STATE_BG,
    DN_TINT_STATE_OBJ,
    DN_TINT_STATE_SET,
    DN_TINT_STATE_RESET = 0xFF
};

struct DayNight
{
    bool8 isDisabled;
    bool8 hasTinted;
    bool8 shouldOverrideTime;
    u16 tint[3];
    u8 tintState;
    u8 overrideTime[2];
};

extern struct DayNight gDayNight;

void FillDNPalette(u16 offset, u16 size);
void UpdateDayNightPalettes(void);
void LoadCompressedDNPalette(const u32 *src, u16 offset, u16 size);
void LoadDNPalette(const void *src, u16 offset, u16 size);
u8 LoadSpriteDNPalette(const struct SpritePalette *palette);
void LoadCompressedSpriteDNPalette(const struct CompressedSpritePalette *src);

#endif // GUARD_DAY_NIGHT_H