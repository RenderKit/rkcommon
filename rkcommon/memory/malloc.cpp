// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "malloc.h"
#if defined(RKCOMMON_TASKING_TBB)
#define __TBB_NO_IMPLICIT_LINKAGE 1
#define __TBBMALLOC_NO_IMPLICIT_LINKAGE 1
#include "tbb/scalable_allocator.h"
#else
#ifdef _WIN32
#include <malloc.h>
#elif defined(__APPLE__) && (defined(__aarch64__) || defined(__arm64__))
#include <stdlib.h>
#else
#include <xmmintrin.h>
#endif
#endif

namespace rkcommon {
  namespace memory {

    void *alignedMalloc(size_t size, size_t align)
    {
      assert((align & (align - 1)) == 0);
#if defined(RKCOMMON_TASKING_TBB)
      return scalable_aligned_malloc(size, align);
#else
#ifdef _WIN32
      return _aligned_malloc(size, align);
#elif defined(__APPLE__) && (defined(__aarch64__) || defined(__arm64__))
      void *ptr = nullptr;
      return (posix_memalign(&ptr, align, size) == 0) ? ptr : nullptr;
#else  // __UNIX__
      return _mm_malloc(size, align);
#endif
#endif
    }

    void alignedFree(void *ptr)
    {
#if defined(RKCOMMON_TASKING_TBB)
      scalable_aligned_free(ptr);
#else
#ifdef _WIN32
      _aligned_free(ptr);
#elif defined(__APPLE__) && (defined(__aarch64__) || defined(__arm64__))
      free(ptr);
#else  // __UNIX__
      _mm_free(ptr);
#endif
#endif
    }

  }  // namespace memory
}  // namespace rkcommon
