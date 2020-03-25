// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <atomic>
#include <functional>
#include <future>

#include "async.h"

namespace ospcommon {
  namespace tasking {

    template <typename T>
    struct AsyncTask
    {
      AsyncTask(std::function<T()> fcn);
      virtual ~AsyncTask();

      bool finished() const;
      bool valid() const;
      void wait() const;

      T get();

     private:
      std::function<T()> stashedTask;
      std::future<T> runningTask;
      std::atomic<bool> jobFinished;
    };

    // Inlined definitions ////////////////////////////////////////////////////

    template <typename T>
    inline AsyncTask<T>::AsyncTask(std::function<T()> fcn)
    {
      stashedTask    = fcn;
      jobFinished    = false;
      auto *thisTask = this;

      runningTask = ospcommon::tasking::async([=]() {
        T retval = thisTask->stashedTask();

        thisTask->jobFinished = true;
        return retval;
      });
    }

    template <typename T>
    inline AsyncTask<T>::~AsyncTask()
    {
      if (valid())
        runningTask.wait();
    }

    template <typename T>
    inline bool AsyncTask<T>::finished() const
    {
      return jobFinished;
    }

    template <typename T>
    inline bool AsyncTask<T>::valid() const
    {
      return runningTask.valid();
    }

    template <typename T>
    inline void AsyncTask<T>::wait() const
    {
      return runningTask.wait();
    }

    template <typename T>
    inline T AsyncTask<T>::get()
    {
      return runningTask.get();
    }

  }  // namespace tasking
}  // namespace ospcommon
