// This file is part of Uplink, an easy-to-use cross-platform live RGBD streaming protocol.
// Copyright (c) 2016, Occipital, Inc.  All rights reserved.
// License: See LICENSE.

# pragma once

# include <pthread.h>

# define UPLINK_THREAD    pthread_t
# define UPLINK_MUTEX     pthread_mutex_t
# define UPLINK_CONDITION pthread_cond_t
