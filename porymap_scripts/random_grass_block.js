function randInt(min, max) {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min)) + min;
}

// These are the grass metatiles in pokefirered.
const grassTiles = [0x8, 0x9, 0x10, 0x11];

// Porymap callback when a block is painted.
export function onBlockChanged(x, y, prevBlock, newBlock) {
    // Check if the user is painting a grass tile.
    if (map.getPrimaryTileset() != "gTileset_General")
        return;

    if (grassTiles.indexOf(newBlock.metatileId) != -1) {
        // Choose a random grass tile and paint it on the map.
        const i = randInt(0, grassTiles.length);
        map.setMetatileId(x, y, grassTiles[i]);
    }
}