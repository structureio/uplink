// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

#pragma once

#include <memory>
#include <cstdint>

namespace uplink {

//------------------------------------------------------------------------------

class MemoryBlock;

bool
encode_image (
    graphics_ImageCodec     imageCodec,
    const uint8_t*        inputBuffer,
    size_t                inputSize,
    graphics_PixelFormat    inputFormat,
    size_t                inputWidth,
    size_t                inputHeight,
    MemoryBlock&          outputMemoryBlock,
    float                 outputQuality
);

bool
decode_image (
    graphics_ImageCodec     imageCodec,
    const uint8_t*        inputBuffer,
    size_t                inputSize,
    graphics_PixelFormat    outputFormat,
    size_t&               outputWidth,
    size_t&               outputHeight,
    MemoryBlock&          outputMemoryBlock
);

//------------------------------------------------------------------------------

} // uplink namespace
