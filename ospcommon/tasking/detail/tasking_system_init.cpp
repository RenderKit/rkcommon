// ======================================================================== //
// Copyright 2009-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "../tasking_system_init.h"

// tasking system internals
#if defined(OSPCOMMON_TASKING_TBB)
#define __TBB_NO_IMPLICIT_LINKAGE 1
#define __TBBMALLOC_NO_IMPLICIT_LINKAGE 1
#include <tbb/task_arena.h>
#include <tbb/task_scheduler_init.h>
#define TBB_PREVIEW_GLOBAL_CONTROL 1
#include <tbb/global_control.h>
#elif defined(OSPCOMMON_TASKING_OMP)
#include <omp.h>
#elif defined(OSPCOMMON_TASKING_INTERNAL)
#include "TaskSys.h"
#endif
// std
#include <thread>
// intrinsics
#include <xmmintrin.h>
// ospcommon
#include "../../common.h"

/* normally defined in pmmintrin.h, but we always need this */
#if !defined(_MM_SET_DENORMALS_ZERO_MODE)
#define _MM_DENORMALS_ZERO_ON (0x0040)
#define _MM_DENORMALS_ZERO_OFF (0x0000)
#define _MM_DENORMALS_ZERO_MASK (0x0040)
#define _MM_SET_DENORMALS_ZERO_MODE(x) \
  (_mm_setcsr((_mm_getcsr() & ~_MM_DENORMALS_ZERO_MASK) | (x)))
#endif

namespace ospcommon {
  namespace tasking {

    struct tasking_system_handle
    {
      tasking_system_handle(int numThreads)
          : numThreads(numThreads)
#if defined(OSPCOMMON_TASKING_TBB)
            ,
            tbb_gc(tbb::global_control::max_allowed_parallelism,
                   (numThreads > 0)
                       ? numThreads
                       : tbb::task_scheduler_init::default_num_threads())

#endif
      {
#if defined(OSPCOMMON_TASKING_OMP)
        if (numThreads > 0)
          omp_set_num_threads(numThreads);
#elif defined(OSPCOMMON_TASKING_INTERNAL)
        detail::initTaskSystemInternal(numThreads <= 0 ? -1 : numThreads + 1);
#endif
      }

      int num_threads()
      {
#if defined(OSPCOMMON_TASKING_TBB)
        return tbb::this_task_arena::max_concurrency();
#elif defined(OSPCOMMON_TASKING_OMP)
        return omp_get_max_threads();
#elif defined(OSPCOMMON_TASKING_INTERNAL)
        return detail::numThreadsTaskSystemInternal();
#else
        return 1;
#endif
      }

      int numThreads{-1};
#if defined(OSPCOMMON_TASKING_TBB)
      tbb::global_control tbb_gc;
#endif
    };

    static std::unique_ptr<tasking_system_handle> g_tasking_handle;

    void initTaskingSystem(int numThreads, bool flushDenormals)
    {
      if (flushDenormals) {
        _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
        _MM_SET_DENORMALS_ZERO_MODE(_MM_DENORMALS_ZERO_ON);
      }

      g_tasking_handle = make_unique<tasking_system_handle>(numThreads);
    }

    int numTaskingThreads()
    {
      if (!g_tasking_handle.get())
        return 0;
      else
        return g_tasking_handle->num_threads();
    }

  }  // namespace tasking
}  // namespace ospcommon
