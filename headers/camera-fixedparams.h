// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include "./core/types.h"
# include "./core/serializers.h"

namespace uplink {

//------------------------------------------------------------------------------

class CameraFixedParams : public Message
{
public:
    UPLINK_MESSAGE_CLASS(CameraFixedParams);

public:
    CameraFixedParams ();
    
public:
    void swapWith (CameraFixedParams& other);
    
public:
    bool isValid () const;

public:
    bool serializeWith (Serializer& serializer);

public:
    float CMOSAndEmitterDistance;
    float refPlaneDistance;
    float planePixelSize;
};

//------------------------------------------------------------------------------

}

# include "./camera-fixedparams.hpp"
