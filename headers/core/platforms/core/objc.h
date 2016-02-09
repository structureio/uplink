// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

// Objective-C helpers.

# pragma once

# include "../../types.h"

# define UPLINK_HAS_OBJC 1

# include <Foundation/NSString.h>
# include <Foundation/NSObject.h>
# include <objc/runtime.h>

# define OBJC_CLASS(Name) @class Name
# define OBJC_CLASS(Name) typedef struct objc_object Name

namespace uplink {

//------------------------------------------------------------------------------

NSString* toNSString (const String& str);
String  fromNSString (NSString* str);

//------------------------------------------------------------------------------

}
