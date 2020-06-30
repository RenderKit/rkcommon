// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"

namespace rkcommon {
  namespace memory {

#define ALIGN_PTR(ptr, alignment) \
  ((((size_t)ptr) + alignment - 1) & ((size_t) - (ssize_t)alignment))

    /*! aligned allocation */
    RKCOMMON_INTERFACE void *alignedMalloc(size_t size, size_t align = 64);
    RKCOMMON_INTERFACE void alignedFree(void *ptr);

    template <typename T>
    __forceinline T *alignedMalloc(size_t nElements, size_t align = 64)
    {
      return (T *)alignedMalloc(nElements * sizeof(T), align);
    }

    inline bool isAligned(void *ptr, int alignment = 64)
    {
      return reinterpret_cast<size_t>(ptr) % alignment == 0;
    }

    // NOTE(jda) - can't use function wrapped alloca solution as Clang won't
    //             inline  a function containing alloca()...but works w/ gcc+icc
#if 0
    template<typename T>
    __forceinline T* stackBuffer(size_t nElements)
    {
      return static_cast<T*>(alloca(sizeof(T) * nElements));
    }
#else
#define STACK_BUFFER(TYPE, nElements) (TYPE *)alloca(sizeof(TYPE) * nElements)
#endif

  }  // namespace memory
}  // namespace rkcommon
