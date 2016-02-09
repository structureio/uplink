// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# include "./desktop-server.h"

# if UPLINK_HAS_DESKTOP_UI
#   include "./desktop-ui.h"
# endif

namespace uplink {

//------------------------------------------------------------------------------

inline
DesktopServer::DesktopServer (const std::string& serviceName, int servicePort, objc_weak ServerDelegate* serverDelegate)
        : Server(serviceName, servicePort, serverDelegate)
        , _ui(0)
    {
# if UPLINK_HAS_DESKTOP_UI
        _ui = new DesktopServerUI;
# endif
    }

inline
DesktopServer::~DesktopServer ()
{
    Server::clear();

    // No other threads should call us back, now.

# if UPLINK_HAS_DESKTOP_UI
    zero_delete(_ui);
# endif
}

//------------------------------------------------------------------------------

} // uplink namespace
