#ifndef GUARD_DAY_NIGHT_H
#define GUARD_DAY_NIGHT_H

extern u16 gPlttBufferDayNight[];

void UpdateDayNightPalettes(void);
void LoadCompressedDNPalette(const u32 *src, u16 offset, u16 size);
void LoadDNPalette(const void *src, u16 offset, u16 size);
u8 LoadSpriteDNPalette(const struct SpritePalette *palette);
void LoadCompressedSpriteDNPalette(const struct CompressedSpritePalette *src);

#endif // GUARD_DAY_NIGHT_H