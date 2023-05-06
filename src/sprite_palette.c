#include "global.h"
#include "sprite_palette.h"
#include "field_weather.h"
#include "palette.h"

EWRAM_DATA struct SpritePaletteReference sSpritePaletteReferences[SWAPABLE_PALETTES] = { 0 };

u8 FindSpritePaletteReference(u8 type, u16 tag)
{
    u8 i;
    for (i = 0; i < SWAPABLE_PALETTES; i++)
    {
        if (sSpritePaletteReferences[i].type == type && sSpritePaletteReferences[i].tag == tag)
            return i;
    }
    return 0xFF;
}

u8 AddSpritePaletteReference(u8 type, u16 tag)
{
    u8 i;
    for (i = 0; i < SWAPABLE_PALETTES; i++)
    {
        if (sSpritePaletteReferences[i].type == PAL_UNUSED)
        {
            sSpritePaletteReferences[i].type = type;
            sSpritePaletteReferences[i].tag = tag;
            sSpritePaletteReferences[i].count = 0;
            return i;
        }
    }

    return 0xFF;
}

u8 IncrementSpritePaletteReferenceCount(u8 idx)
{
    sSpritePaletteReferences[idx].count++;
    return idx;
}

void DecrementSpritePaletteReferenceCount(u8 idx)
{
    if (sSpritePaletteReferences[idx].type == PAL_UNUSED)
    {
        FillPalette(0, 0x100 + (idx << 4), 0x20);
        return;
    }

    if (sSpritePaletteReferences[idx].count > 0)
        sSpritePaletteReferences[idx].count--;

    if (sSpritePaletteReferences[idx].count == 0)
    {
        FillPalette(0, 0x100 + (idx << 4), 0x20);
        sSpritePaletteReferences[idx].type = PAL_UNUSED;
    }
}

void ClearSpritePaletteReferences(void)
{
    FillPalette(0, 0x100, 0x20 * SWAPABLE_PALETTES);
    memset(sSpritePaletteReferences, 0, sizeof(sSpritePaletteReferences));
}

u8 GetObjectPaletteSlot(u16 tag)
{
    u8 slot = FindSpritePaletteReference(PAL_NPC, tag);
    if (slot != 0xFF)
        return slot;

    return AddSpritePaletteReference(PAL_NPC, tag);
}

u8 GetReflectionPaletteSlot(u16 tag)
{
    u8 slot = FindSpritePaletteReference(PAL_REFLECTION, tag);
    if (slot != 0xFF)
        return slot;

    return AddSpritePaletteReference(PAL_REFLECTION, tag);
}

u8 GetItemIconPaletteSlot(u16 tag)
{
    u8 slot = FindSpritePaletteReference(PAL_ITEM_ICON, tag);
    if (slot != 0xFF)
        return slot;

    return AddSpritePaletteReference(PAL_ITEM_ICON, tag);
}