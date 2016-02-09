// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include "./core/apple.h"

# define UPLINK_HAS_IOS 1

# if TARGET_IPHONE_SIMULATOR
#   define UPLINK_HAS_IOS_SIMULATOR 1
# else
#   define UPLINK_HAS_IOS_DEVICE 1
# endif

namespace uplink {

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------

}
