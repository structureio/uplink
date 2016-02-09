// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include "./pthreads.h"
# include "./bsd-sockets.h"

# define UPLINK_HAS_POSIX 1

namespace uplink {

//------------------------------------------------------------------------------

int vasprintf (char** output, const char* format, va_list args);

String getLocalHostName ();

int64 getTickCount ();

double getTickFrequency ();

//------------------------------------------------------------------------------

}
