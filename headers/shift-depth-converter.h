// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include <cmath>
# include <cstdint>

namespace uplink {

struct ShiftDepthConverter
{
public:
    void initialize (
        float cmosAndEmitterDistance,
        float referencePlaneDistance,
        float planePixelSize,
        int   sensorPixelSizeFactor,
        float registeredDepthOffset
    );

    void initializeWithDefaults ();
    
public:
    bool isValid () const;

public:
    float shiftToDepthInMillimeters (float shift);
    float depthInMillimetersToShift (float depthInMillimeters);
   
private:
    float  cmosAndEmitterDistance = NAN;
    float  referencePlaneDistance = NAN;
    float  planePixelSize         = NAN;
    double fRegisteredDepthOffset = NAN;
    int    sensorPixelSizeFactor  = -1;

private:
    double A = NAN;
    double B = NAN;
};

} // uplink namespace

# include "shift-depth-converter.hpp"
