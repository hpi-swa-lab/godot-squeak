#!/bin/bash

set -e

cd "$(dirname $(realpath $0))/.."

tempdir=$(mktemp -d)
mkdir -p "$tempdir/godot-squeak/addons/squeak/image"

echo "=======> Created tempdir $tempdir"
echo "=======> Building plugin"

make
cp build/liblfqueue.so build/libsqueak.so build/libsqplugin.so addons/squeak/libsqplugin.gdnlib "$tempdir/godot-squeak/addons/squeak"

echo "=======> Creating image"

scripts/createImage.sh "$tempdir/godot-squeak/addons/squeak/image"
touch "$tempdir/godot-squeak/addons/squeak/image/.gdignore"

echo "=======> Adding readme"

echo -e 'Place addons/ in the Godot project folder.\n\nWhen running Godot, LD_LIBRARY_PATH must include addons/squeak/\nFor example:\nLD_LIBRARY_PATH="$LD_LIBRARY_PATH:/path/to/plugin/addons/squeak ./Godot' > "$tempdir/godot-squeak/README.txt"

echo "=======> Creating tarball"

commit_hash=$(git rev-parse --short HEAD)
printf -v datestr "%(%Y%m%d-%H%M%S)T" -1
output_file=${1:-$PWD/build/godot-squeak-$commit_hash-$datestr.tar.gz}

mkdir -p $(dirname "$output_file")
tar czf "$output_file" -C "$tempdir" godot-squeak 

echo "=======> Removing tempdir"

rm -rf $tempdir

echo "=======> Created new release at $output_file"
