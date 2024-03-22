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

shutil.copyfile(build_dir / "TycheFront.html", pack_path / "TycheFront.html.prepatch")

# this whole mechanism is pretty bad. Sorry :(
# what we do, is look for a marker, and add a spot to patch just below that.

patch_file = pack_path / "TycheFront.html.prepatch"

replacement = '''
<div id=\"screen\"></div>
<script type="text/javascript">
    json_rpc_host = "{JSON_RPC_HOST}"
    image_host = "{IMAGE_HOST}"
</script>
'''

with open(patch_file) as f:
    fixed = f.read().replace("<div id=\"screen\"></div>", replacement)

with open(patch_file,'w') as f:
    f.write(fixed)

shutil.copyfile(build_dir.parent / "serve_wasm.py", pack_path / "serve_wasm.py")

shutil.make_archive(build_dir / "TycheFront_dist", 'zip', pack_path)
