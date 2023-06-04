#ifndef GUARD_SPRITE_PALETTE_H
#define GUARD_SPRITE_PALETTE_H

#define SWAPABLE_PALETTES 12

enum {
    PAL_UNUSED,
    PAL_OBJEVENT,
    PAL_NORMAL,
    PAL_REFLECTION
};

struct SpritePaletteReference
{
    u8 type;
    u8 count;
    u16 tag;
};

u8 GetSpritePaletteReferenceType(u8 index);
u8 FindSpritePaletteReference(u8 type, u16 tag);
u8 AddSpritePaletteReference(u8 type, u16 tag);
u8 IncrementSpritePaletteReferenceCount(u8 idx);
void DecrementSpritePaletteReferenceCount(u8 idx);
void ClearSpritePaletteReferences(void);

u8 GetSpritePaletteSlot(u8 type, u16 tag);

#endif // GUARD_SPRITE_PALETTE_H