#!/usr/bin/env python3

import os.path
from os.path import join as joinp
import subprocess
import sys
from glob import glob

import png

palettes = [
    "graphics/object_events/pics/people/lass.png",
    "graphics/object_events/pics/people/nurse.png",
    "graphics/object_events/pics/people/camper.png",
    "graphics/object_events/pics/people/little_boy.png",
]

def closest_color(c, palette):
    min_d = float('inf')
    best = 0
    r1, g1, b1 = c
    for i, (r2, g2, b2) in enumerate(palette[1:], 1):
        # Color diff from https://stackoverflow.com/questions/1847092/given-an-rgb-value-what-would-be-the-best-way-to-find-the-closest-match-in-the-d
        d = ((r2-r1)*0.30)**2 + ((g2-g1)*0.59)**2 + ((b2-b1)*0.11)**2
        if d < min_d:
            min_d = d
            best = i
    return best

def remap_to_palette(palette_file, input_file, output_file):  # Apply one file's palette to another
    plt = png.Reader(palette_file)
    plt.read()
    target_palette = tuple(c[:3] for c in plt.palette())
    inp = png.Reader(input_file)
    w, h, rows, info = inp.read()
    src_palette = tuple(c[:3] for c in inp.palette())
    new_rows = [ [ closest_color(src_palette[c][:3], target_palette) if c else 0 for c in row ] for row in rows ]

    with open(output_file, 'wb') as f:
        w = png.Writer(width=w, height=h, bitdepth=4, palette=target_palette)
        w.write(f, new_rows)

def change_palette(palette_file, input_file, output_file):  # Apply one file's palette to another
    plt = png.Reader(palette_file)
    plt.read()
    target_palette = tuple(c[:3] for c in plt.palette())
    inp = png.Reader(input_file)
    w, h, rows, info = inp.read()
    src_palette = tuple(c[:3] for c in inp.palette())
    # new_rows = [ [ closest_color(src_palette[c][:3], target_palette) if c else 0 for c in row ] for row in rows ]

    with open(output_file, 'wb') as f:
        w = png.Writer(width=w, height=h, bitdepth=4, palette=target_palette)
        w.write(f, rows)

def format_input_output_fn(basefn, out_n):
    input_file = joinp("graphics", "object_events", "pics", basefn)
    if not input_file.startswith(".png"):
        input_file += ".png"

    output_file = input_file[:-4] + f"{out_n}.png"

    return input_file, output_file

if __name__ == '__main__':
    is_remap = sys.argv[2] == "1"
    for i, pal_file in enumerate(palettes):
        input_file, output_file = format_input_output_fn(sys.argv[1], i)
        if is_remap:
            remap_to_palette(pal_file, input_file, output_file)
        else:
            change_palette(pal_file, input_file, output_file)