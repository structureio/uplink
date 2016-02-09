#!/bin/bash -e
HERE=`cd "\`dirname \"$0\"\`";pwd`

CMAKE_GENERATOR="Xcode"
CMAKE_BUILD_TYPES="Release;Debug"
UPLINK_DIR="$HERE/.."
BUILD_DIR="$UPLINK_DIR"/builds/cmake-xcode

mkdir -p "$BUILD_DIR"
pushd "$BUILD_DIR"
    cmake \
        -DCMAKE_BUILD_TYPE:STRING=$CMAKE_BUILD_TYPE \
        -DCMAKE_CONFIGURATION_TYPES:STRING="$CMAKE_BUILD_TYPES" \
        -G"$CMAKE_GENERATOR" \
        "$UPLINK_DIR"

    open uplink.xcodeproj
popd
