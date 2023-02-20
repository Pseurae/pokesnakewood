#!/usr/bin/env python3
""" Extract sprites from HGSS follower spritesheets. """

import os
import os.path
import subprocess
import sys
import shutil
from glob import glob
from tqdm import tqdm
from math import sqrt
import png

class FormatIconException(Exception):
    pass

def closest_color(c, palette):
    min_d = float('inf')
    best = 0
    r1, g1, b1 = c
    for i, (r2, g2, b2) in enumerate(palette[1:], 1):
        if (r2 - r1) < 128:
            d = (2 * (r2-r1))**2 + 4 * ((g2-g1))**2 + 3 * ((b2-b1))**2
        else:
            d = (3 * (r2-r1))**2 + 4 * ((g2-g1))**2 + 2 * ((b2-b1))**2
        if d < min_d:
            min_d = d
            best = i
    return best

def format_icon(fname, pltfname, outpath):
    if not os.path.exists(fname):
        raise FormatIconException(f"Cannot open {fname}.")

    if not os.path.exists(pltfname):
        raise FormatIconException(f"Cannot open {pltfname}.")

    pltf = png.Reader(pltfname)
    pltf.read()
    plt = [ c[:3] for c in pltf.palette() ]

    bgcol = f"rgb{plt[0][:3]}"

    subprocess.run(["convert", fname, "-background", bgcol, "-trim",
                    "-splice", "0x2", "-alpha", "remove", "-gravity", 
                    "center", "-extent", "32x32", outpath])
    subprocess.run(["convert", outpath, outpath, "-append", outpath])

    inp = png.Reader(outpath)
    w, h, rows, info = inp.read()
    src_palette = tuple(c[:3] for c in inp.palette())
    new_rows = [ [ closest_color(src_palette[c][:3], plt) if c else 0 for c in row ] for row in rows ]

    with open(outpath, 'wb') as f:
        w = png.Writer(width=w, height=h, bitdepth=4, palette=plt)
        try:
            w.write(f, new_rows)
        except:
            print(outpath, plt)
            os.unlink(outpath)

def get_pltf_name(root, i, female=False):
    if not female:
        fnames = [ "anim_front.png", "front.png" ]
    else:
        fnames = [ "anim_frontf.png", "frontf.png" ]

    for fname in fnames:
        pltfname = os.path.join(root, i, fname)
        if os.path.exists(pltfname):
            return pltfname

    return pltfname

def format_icons(palroot, root=""):
    os.makedirs("formatted", exist_ok=True)
    prog = tqdm(os.listdir(palroot))

    for i in prog:
        try:
            fname = os.path.join(root, i + ".png")
            outpath = os.path.join("formatted", i + ".png")
            pltfname = get_pltf_name(palroot, i)

            format_icon(fname, pltfname, outpath)

            megadir = os.path.join(palroot, i, "mega")

            if os.path.exists(megadir):
                fname = os.path.join(root, i + "-mega.png")
                outpath = os.path.join("formatted", i + "-mega.png")
                pltfname = get_pltf_name(palroot, i)

                format_icon(fname, pltfname, outpath)

            pltfname = get_pltf_name(palroot, i, True)

            if os.path.exists(pltfname):
                fname = os.path.join(root, "female", i + ".png")
                outpath = os.path.join("formatted", i + "-f.png")
                format_icon(fname, pltfname, outpath)

        except FormatIconException as e:
            prog.write(f"{e}")

def get_icon_path(root, i, mega=False, female=False):
    if not female:
        fnames = [ "icon.png" ]
    else:
        fnames = [ "iconf.png" ]

    mega_dir = "mega" if mega else ""

    for fname in fnames:
        pltfname = os.path.join(root, i, mega_dir, fname)
        if os.path.exists(pltfname):
            return pltfname

    return pltfname

def move_icons(palroot, root=""):
    prog = tqdm(glob(os.path.join(root, "*.png")))

    for i in prog:
        _, fname = os.path.split(i)
        fname, _ = os.path.splitext(fname)
        mega = fname.endswith("-mega")

        if mega:
            fname = fname[:-5]

        female = fname.endswith("-f")
        if female:
            fname = fname[:-2]

        outpath = get_icon_path(palroot, fname, mega, female)
        if not os.path.exists(outpath):
            prog.write(f"Cannot open {outpath}.")
            continue

        shutil.copy(i, outpath)

if __name__ == '__main__':
    palroot = os.path.abspath(os.path.join("../../", "graphics", "pokemon"))
    format_icons(palroot, "icons")
    move_icons(palroot, "formatted")