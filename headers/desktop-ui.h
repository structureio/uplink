// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include "./core/types.h"

namespace uplink {

struct DesktopServerUI
{
    DesktopServerUI();
    ~DesktopServerUI();

    void run ();

    void setColorImage(const uint8*  buffer, int width, int height);
    void setDepthImage(const uint16* buffer, int width, int height);

    struct Impl;
    Impl*  impl;
};

} // uplink namespace

# include "./desktop-ui.hpp"
