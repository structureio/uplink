// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include "./servers.h"

namespace uplink {

struct DesktopUI;

//------------------------------------------------------------------------------

struct DesktopServer : Server
{
public:
     DesktopServer (const std::string& serviceName, int servicePort, objc_weak ServerDelegate* serverDelegate);
    ~DesktopServer ();

public:
    DesktopServerUI& ui() { return *_ui; }

private:
    DesktopServerUI* _ui;
};

//------------------------------------------------------------------------------

struct DesktopServerSession : ServerSession
{
    DesktopServerSession (int socketDescriptor, Server* server)
        : ServerSession(socketDescriptor, server)
    {

    }

    DesktopServer& server () { return *downcast<DesktopServer>(&ServerSession::server()); }
};

//------------------------------------------------------------------------------

} // uplink namespace

# include "./desktop-server.hpp"
