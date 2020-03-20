// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <utility>

#ifdef OSPCOMMON_TASKING_TBB
#  define __TBB_NO_IMPLICIT_LINKAGE 1
#  define __TBBMALLOC_NO_IMPLICIT_LINKAGE 1
#  include <tbb/parallel_for.h>
#elif defined(OSPCOMMON_TASKING_INTERNAL)
#  include "TaskSys.h"
#endif

namespace ospcommon {
  namespace tasking {
    namespace detail {

      template<typename INDEX_T, typename TASK_T>
      inline void parallel_for_impl(INDEX_T nTasks, TASK_T&& fcn)
      {
#ifdef OSPCOMMON_TASKING_TBB
        tbb::parallel_for(INDEX_T(0), nTasks, std::forward<TASK_T>(fcn));
#elif defined(OSPCOMMON_TASKING_OMP)
#       pragma omp parallel for schedule(dynamic)
        for (INDEX_T taskIndex = 0; taskIndex < nTasks; ++taskIndex) {
          fcn(taskIndex);
        }
#elif defined(OSPCOMMON_TASKING_INTERNAL)
        detail::parallel_for_internal(nTasks, std::forward<TASK_T>(fcn));
#else // Debug (no tasking system)
        for (INDEX_T taskIndex = 0; taskIndex < nTasks; ++taskIndex) {
          fcn(taskIndex);
        }
#endif
      }

    } // ::ospcommon::tasking::detail
  } // ::ospcommon::tasking
} // ::ospcommon
