#ifndef GUARD_SPRITE_PALETTE_H
#define GUARD_SPRITE_PALETTE_H

#define SWAPABLE_PALETTES 15

enum {
    PAL_UNUSED,
    PAL_NPC,
    PAL_REFLECTION,
    PAL_ITEM_ICON
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

u8 GetObjectPaletteSlot(u16 tag);
u8 GetReflectionPaletteSlot(u16 tag);
u8 GetItemIconPaletteSlot(u16 tag);

#endif // GUARD_SPRITE_PALETTE_H