#!/usr/bin/env python3

from pathlib import Path
import sys
import os
import shutil

if len(sys.argv) < 2:
    print("Needs path to WASM file")
    exit(1)

build_file = Path(sys.argv[-1])
build_dir = build_file.parent

pack_path = build_dir / "pack"

try:
    shutil.rmtree(pack_path)
except:
    pass

pack_path.mkdir(parents=True)


flist = [
    "qtloader.js",
    "qtlogo.svg",
    "TycheFront.html",
    "TycheFront.js",
    "TycheFront.wasm",
]

for f in flist:
    shutil.copyfile(build_dir / f, pack_path / f)

shutil.copyfile(build_dir.parent / "serve_wasm.py", pack_path / "serve_wasm.py")

shutil.make_archive(build_dir / "TycheFront_dist", 'zip', pack_path)
