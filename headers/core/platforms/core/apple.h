// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include "./posix.h"
# include "./apple-image-codecs.h"

# define UPLINK_HAS_APPLE 1

# if defined(__OBJC__)
#   include "./objc.h"
# endif

namespace uplink {

//------------------------------------------------------------------------------

int64  getTickCount ();
double getTickFrequency ();

//------------------------------------------------------------------------------

}
