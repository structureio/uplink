// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include "./windows.h"

namespace uplink {

//------------------------------------------------------------------------------

inline void
platform_startup ()
{
    WindowsSocket::DLL::require();
}

inline void
platform_shutdown ()
{

}

//------------------------------------------------------------------------------

}

# include "./core/windows-sockets.hpp"
# include "./core/windows-threads.hpp"
# include "./core/windows-image-codecs.hpp"
