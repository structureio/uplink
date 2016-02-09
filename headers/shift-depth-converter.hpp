// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include "shift-depth-converter.h"

# include <cassert>

namespace uplink {

inline void
ShiftDepthConverter::initialize (
        float cmosAndEmitterDistance,
        float referencePlaneDistance,
        float planePixelSize,
        int   sensorPixelSizeFactor,
        float registeredDepthOffset
)
{
    // Save input parameters.
    this->cmosAndEmitterDistance = cmosAndEmitterDistance;
    this->referencePlaneDistance = referencePlaneDistance;
    this->planePixelSize         = planePixelSize;
    this->fRegisteredDepthOffset = registeredDepthOffset;
    this->sensorPixelSizeFactor  = sensorPixelSizeFactor;

    assert(!isnan(cmosAndEmitterDistance));
    assert(!isnan(referencePlaneDistance));
    assert(!isnan(planePixelSize));
    assert(sensorPixelSizeFactor > 0);
    
    double nZeroPlaneDistance = referencePlaneDistance; // As measured to camera pupil.
    double fEmitterDCmosDistance = cmosAndEmitterDistance;
    
    const int nPixelSizeFactor = sensorPixelSizeFactor;
    const int nShiftScale= 10;
    const int nParamCoeff= 4;
    const int nConstShiftWithoutParamCoeff = 200;
    
    const double dPlanePixelSize = planePixelSize * nPixelSizeFactor;
    const double dPlaneDsr = nZeroPlaneDistance;
    const double dPlaneDcl = fEmitterDCmosDistance;

    double nConstShift = double(nParamCoeff * nConstShiftWithoutParamCoeff) / nPixelSizeFactor;
    
    // Matlab code to recover the expressions below:
    //
    //     syms X nConstShift nParamCoeff dPlanePixelSize nShiftScale dPlaneDsr dPlaneDcl dPlaneDsr
    //     collect(nShiftScale * ((((dPlanePixelSize*(((X - nConstShift)/nParamCoeff) - 0.375)) * dPlaneDsr) / (dPlaneDcl - (dPlanePixelSize * (((X - nConstShift) / nParamCoeff) - 0.375)))) + dPlaneDsr), X)
    //
    // Then manual simplification.

    this->A = (-8 * dPlaneDcl * dPlaneDsr * nParamCoeff * nShiftScale) / (8 * dPlanePixelSize);
    this->B = (-8 * dPlanePixelSize * nConstShift - 8 * dPlaneDcl * nParamCoeff - 3 * dPlanePixelSize * nParamCoeff) / (8 * dPlanePixelSize);
}

inline void
ShiftDepthConverter::initializeWithDefaults ()
{
    static const float cmosAndEmitterDistance = 6.5f;   // CMOS and emitter distance value is in centimeters and fixed for all Structure Sensors.
    static const float referencePlaneDistance = 90.0f;  // Reference plane distance value is in centimeters and fixed for all Structure Sensors.
    static const float planePixelSize         = 0.078f; // Plane pixel size is normally a sensor-specific, factory-calibrated. Using an approximation here.
    static const int   sensorPixelSizeFactor  = 1;
    static const float registeredDepthOffset  = 0.f;

    initialize(cmosAndEmitterDistance, referencePlaneDistance, planePixelSize, sensorPixelSizeFactor, registeredDepthOffset);
}

inline bool
ShiftDepthConverter::isValid () const
{
    return !isnan(A) && !isnan(B);
}

inline float
ShiftDepthConverter::shiftToDepthInMillimeters (float shift)
{
    assert(isValid());

    return (A / (shift + B)) + fRegisteredDepthOffset;
}

inline float
ShiftDepthConverter::depthInMillimetersToShift (float depthInMillimeters)
{
    assert(isValid());

    return -B + 1.0 / ((depthInMillimeters - fRegisteredDepthOffset) / A);
}

} // uplink namespace
