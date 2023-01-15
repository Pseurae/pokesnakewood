import glob
import os
import json

def clear_map_json(p):
    p = os.path.join(p, "map.json")
    f = open(p)
    d = json.load(f)
    f.close()
    d["object_events"] = []
    d["bg_events"] = []
    d["coord_events"] = []
    f = open(p, "w")
    json.dump(d, f, indent=2)

def clean_scripts(p):
    f = open(os.path.join(p, "scripts.pory"), "w")
    f.write(
f"""raw `
{p}_MapScripts::
    .byte 0
`
"""
    )
    f.close()

if __name__ == "__main__":
    for p in glob.glob("BattleFrontier_*"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("BattlePyramid*"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("TrainerHill_*"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("LilycoveCity_Contest*"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("*_Battle*"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("*_PokemonCenter_2F"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("*_PokemonLeague_2F"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("BattleColosseum_*"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("RecordCorner"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("MossdeepCity_GameCorner_*"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("TradeCenter"):
        clear_map_json(p)
        clean_scripts(p)

    for p in glob.glob("SootopolisCity_MysteryEventsHouse_*"):
        clear_map_json(p)
        clean_scripts(p)
