// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "sse2neon.h"

/* Dummy defines for floating point control */
#define _MM_MASK_MASK 0x1f80
#define _MM_MASK_DIV_ZERO 0x200
#define _MM_MASK_DENORM 0x100
#define _MM_SET_EXCEPTION_MASK(x)
