// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

#pragma once

// FIXME: Dynamically compute the table with the camera info data, instead of using this fixed one.

# include "./types.h"

namespace uplink {

uint16 shift2depth (uint16  shift);
void   shift2depth (uint16* buffer, size_t size);

//------------------------------------------------------------------------------

}

#include "shift2depth.hpp"
