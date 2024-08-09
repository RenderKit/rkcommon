// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "TaskSys.h"
// ospray
#include "../../platform.h"

namespace rkcommon {
  namespace tasking {
    namespace detail {

      // TaskSys definitions //////////////////////////////////////////////////

      static std::unique_ptr<enki::TaskScheduler> g_ts;

      // Interface definitions ////////////////////////////////////////////////

      void initTaskSystemInternal(int nThreads)
      {
        g_ts = std::unique_ptr<enki::TaskScheduler>(new enki::TaskScheduler());
        if (nThreads < 1)
          nThreads = enki::GetNumHardwareThreads();
        g_ts->Initialize(nThreads);
      }

      void shutdownTaskSystemInternal()
      {
        g_ts.reset();
      }

      int numThreadsTaskSystemInternal()
      {
        return g_ts->GetNumTaskThreads();
      }

      void scheduleTaskInternal(Task *task)
      {
        if (g_ts.get() == nullptr)
          initTaskSystemInternal(-1);

        g_ts->AddTaskSetToPipe(task);
      }

      void waitInternal(Task *task)
      {
        g_ts->WaitforTask(task);
      }

    }  // namespace detail
  }    // namespace tasking
}  // namespace rkcommon
