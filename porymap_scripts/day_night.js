let lastHour = 10;

let timeOfDayTints = [
    [ 0.6, 0.6, 0.92 ],
    [ 0.6, 0.6, 0.92 ],
    [ 0.6, 0.6, 0.92 ],
    [ 0.6, 0.6, 0.92 ],
    [ 0.6, 0.65, 1.0 ],
    [ 0.7, 0.7, 0.9 ],
    [ 0.7, 0.7, 0.9 ],
    [ 0.7, 0.7, 0.9 ],
    [ 0.9, 0.85, 1.0 ],
    [ 1.0, 0.9, 1.0 ],
    [ 1.0, 1.0, 1.0 ],
    [ 1.0, 1.0, 1.0 ],
    [ 1.0, 1.0, 1.0 ],
    [ 1.0, 1.0, 1.0 ],
    [ 1.0, 1.0, 1.0 ],
    [ 1.0, 1.0, 1.0 ],
    [ 1.0, 0.98, 0.9 ],
    [ 0.9, 0.7, 0.67 ],
    [ 0.75, 0.66, 0.77 ],
    [ 0.7, 0.63, 0.82 ],
    [ 0.6, 0.6, 0.92 ],
    [ 0.6, 0.6, 0.92 ],
    [ 0.6, 0.6, 0.92 ]
];

function applyTintToPalettes(palettes, hour)
{
    for (let i = 0; i < palettes.length; i++)
    {
        let palette = palettes[i];

        for (let j = 0; j < palette.length; j++)
        {
            palette[j][0] *= timeOfDayTints[hour][0];
            palette[j][1] *= timeOfDayTints[hour][1];
            palette[j][2] *= timeOfDayTints[hour][2];
        }

        palettes[i] = palette;
    }
    return palettes;
}

export function applyNightTint() {
    let { input, ok } = utility.getInputNumber("Day Night Tinting", "Set Current Hour", lastHour, 0, 23);

    if (!ok || lastHour == input)
        return;

    lastHour = input;

    let primaryPalettes = map.getPrimaryTilesetPalettes();
    let secondaryPalettes = map.getSecondaryTilesetPalettes();

    applyTintToPalettes(primaryPalettes, lastHour);
    applyTintToPalettes(secondaryPalettes, lastHour);

    map.setPrimaryTilesetPalettesPreview(primaryPalettes, true);
    map.setSecondaryTilesetPalettesPreview(secondaryPalettes, true);
}

export function onMapOpened(mapName) {
    map.setPrimaryTilesetPalettesPreview(map.getPrimaryTilesetPalettes(), true);
    map.setSecondaryTilesetPalettesPreview(map.getSecondaryTilesetPalettes(), true);
}

// Porymap callback when project is opened.
export function onProjectOpened(projectPath) {
   utility.registerAction("applyNightTint", "View Night Tint", "T")
}