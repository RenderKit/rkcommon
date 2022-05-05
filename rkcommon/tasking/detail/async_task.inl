// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#if defined(RKCOMMON_TASKING_TBB)
#define __TBB_NO_IMPLICIT_LINKAGE 1
#define __TBBMALLOC_NO_IMPLICIT_LINKAGE 1
#include <tbb/task_group.h>
#elif defined(RKCOMMON_TASKING_OMP)
#include <thread>
#elif defined(RKCOMMON_TASKING_INTERNAL)
#include "TaskSys.h"
#endif

namespace rkcommon {
  namespace tasking {
    namespace detail {

      template <typename TASK_T>
      struct AsyncTaskImpl
      {
        AsyncTaskImpl(TASK_T &&fcn);
        void wait();

       private:
#if defined(RKCOMMON_TASKING_TBB)
        tbb::task_group taskGroup;
#elif defined(RKCOMMON_TASKING_OMP)
        std::thread thread;
#elif defined(RKCOMMON_TASKING_INTERNAL)
        struct LocalTask : public enki::ITaskSet
        {
          TASK_T t;
          LocalTask(TASK_T &&fcn) : t(std::forward<TASK_T>(fcn)) {}
          void ExecuteRange(enki::TaskSetPartition, uint32_t) override
          {
            t();
          }
        };
        LocalTask task;
#endif
      };

      // Inlined definitions
      // ////////////////////////////////////////////////////

      template <typename TASK_T>
      inline AsyncTaskImpl<TASK_T>::AsyncTaskImpl(TASK_T &&fcn)
#if defined(RKCOMMON_TASKING_TBB)
      {
        taskGroup.run(std::forward<TASK_T>(fcn));
      }
#elif defined(RKCOMMON_TASKING_OMP)
          : thread(std::forward<TASK_T>(fcn))
      {
      }
#elif defined(RKCOMMON_TASKING_INTERNAL)
          : task(std::forward<TASK_T>(fcn))
      {
        detail::scheduleTaskInternal(&task);
      }
#else
      {
        fcn();
      }
#endif

      template <typename TASK_T>
      inline void AsyncTaskImpl<TASK_T>::wait()
      {
#if defined(RKCOMMON_TASKING_TBB)
        taskGroup.wait();
#elif defined(RKCOMMON_TASKING_OMP)
        if (thread.joinable())
          thread.join();
#elif defined(RKCOMMON_TASKING_INTERNAL)
        detail::waitInternal(&task);
#endif
      }
    }  // namespace detail
  }    // namespace tasking
}  // namespace rkcommon
