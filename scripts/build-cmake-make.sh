#!/bin/bash -e
HERE=`cd "\`dirname \"$0\"\`";pwd`

CMAKE_GENERATOR="Unix Makefiles"
CMAKE_BUILD_TYPE=Release
UPLINK_DIR="$HERE/.."
BUILD_DIR="$UPLINK_DIR"/builds/cmake-make-r

mkdir -p "$BUILD_DIR"
pushd "$BUILD_DIR"
    cmake \
        -DCMAKE_BUILD_TYPE:STRING=$CMAKE_BUILD_TYPE \
        -G"$CMAKE_GENERATOR" \
        "$UPLINK_DIR"

    make
popd
