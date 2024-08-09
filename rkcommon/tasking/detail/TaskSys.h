// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../../common.h"
// enkiTS
#include "enkiTS/TaskScheduler.h"

namespace rkcommon {
  namespace tasking {
    namespace detail {

      // Public interface to the tasking system ///////////////////////////////

      using Task = enki::ITaskSet;

      void initTaskSystemInternal(int numThreads = -1);
      void shutdownTaskSystemInternal();
      int numThreadsTaskSystemInternal();

      void RKCOMMON_INTERFACE scheduleTaskInternal(Task *task);
      void RKCOMMON_INTERFACE waitInternal(Task *task);

      template <typename TASK_T>
      inline void parallel_for_internal(int nTasks, TASK_T &&fcn)
      {
        struct LocalTask : public Task
        {
          const TASK_T &t;
          LocalTask(int nunTasks, TASK_T &&fcn)

              : Task(nunTasks), t(std::forward<TASK_T>(fcn))
          {
          }

          ~LocalTask() override = default;

          void ExecuteRange(enki::TaskSetPartition tp, uint32_t) override
          {
            for (auto i = tp.start; i < tp.end; ++i)
              t(i);
          }
        };

        LocalTask task(nTasks, std::forward<TASK_T>(fcn));
        scheduleTaskInternal(&task);
        waitInternal(&task);
      }

      template <typename TASK_T>
      inline void schedule_internal(TASK_T &&fcn)
      {
        struct LocalTask : public Task
        {
          TASK_T t;

          LocalTask(TASK_T &&fcn) : Task(1), t(std::forward<TASK_T>(fcn)) {}

          ~LocalTask() override = default;

          void ExecuteRange(enki::TaskSetPartition, uint32_t) override
          {
            t();
            delete this;
          }
        };

        auto *task = new LocalTask(std::forward<TASK_T>(fcn));
        scheduleTaskInternal(task);
      }

    }  // namespace detail
  }    // namespace tasking
}  // namespace rkcommon
