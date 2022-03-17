#!/bin/bash

set -e

IMAGE_URL="http://files.squeak.org/trunk/Squeak6.0alpha-21475-64bit/Squeak6.0alpha-21475-64bit-202112201228-Linux-x64.tar.gz"

output_dir=${1:-godot-squeak-image}
tempdir=$(mktemp -d)
tempfile=$tempdir/squeak-archive.temp.tar.gz

wget $IMAGE_URL -O $tempfile
tar -C $tempdir -xzf $tempfile --strip-components=1

mv $tempdir/shared/*.image $tempdir/shared/squeak.image
mv $tempdir/shared/*.changes $tempdir/shared/squeak.changes
$tempdir/squeak.sh $tempdir/shared/squeak.image "$(dirname $(realpath $0))/createImage.st"

mkdir -p $output_dir
mv "$tempdir/shared"/* "$output_dir"

rm -rf $tempdir
