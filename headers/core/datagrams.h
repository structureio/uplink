// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include "./platform.h"

//------------------------------------------------------------------------------

namespace uplink {

class DatagramBroadcaster
{
public:
    DatagramBroadcaster ();
   ~DatagramBroadcaster ();

public:
    bool open (int port);
    void close ();
    bool ready () const;

public:
    bool send (const Byte* bytes, Size length);
    bool send (CString);

private:
    UDPBroadcaster* broadcaster;
};

//------------------------------------------------------------------------------

class DatagramListener
{
public:
    DatagramListener ();
   ~DatagramListener ();

public:
    bool open (int port);
    void close ();
    bool ready () const;

public:
    bool receive (Byte* bytes, Size size, NetworkAddress& sender);

private:
    UDPListener* listener;
};

}

# include "./datagrams.hpp"
