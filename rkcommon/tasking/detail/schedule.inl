// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <utility>

#ifdef RKCOMMON_TASKING_TBB
#  define __TBB_NO_IMPLICIT_LINKAGE 1
#  define __TBBMALLOC_NO_IMPLICIT_LINKAGE 1
#  include "tbb/task_arena.h"
#elif defined(RKCOMMON_TASKING_OMP)
#  include <thread>
#elif defined(RKCOMMON_TASKING_INTERNAL)
#  include "TaskSys.h"
#endif

namespace rkcommon {
  namespace tasking {
    namespace detail {

      template<typename TASK_T>
      inline void schedule_impl(TASK_T fcn)
      {
#ifdef RKCOMMON_TASKING_TBB
        tbb::task_arena ta = tbb::task_arena(tbb::task_arena::attach());
        ta.enqueue(fcn);
#elif defined(RKCOMMON_TASKING_OMP)
        std::thread thread(fcn);
        thread.detach();
#elif defined(RKCOMMON_TASKING_INTERNAL)
        detail::schedule_internal(std::move(fcn));
#else// Debug --> synchronous!
        fcn();
#endif
      }

    } // ::rkcommon::tasking::detail
  } // ::rkcommon::tasking
} // ::rkcommon
