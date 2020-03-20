// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

// std
#include <chrono>

namespace ospcommon {
  namespace utility {

    /*! Helper class that assists with timing a region of code. */
    struct CodeTimer
    {
      void start();
      void stop();

      double seconds() const;
      double milliseconds() const;
      double perSecond() const;

      double secondsSmoothed() const;
      double millisecondsSmoothed() const;
      double perSecondSmoothed() const;

     private:
      double smooth_nom{0.0};
      double smooth_den{0.0};

      std::chrono::time_point<std::chrono::steady_clock> frameEndTime;
      std::chrono::time_point<std::chrono::steady_clock> frameStartTime;
    };

    // Inlined CodeTimer definitions //////////////////////////////////////////

    inline void CodeTimer::start()
    {
      frameStartTime = std::chrono::steady_clock::now();
    }

    inline void CodeTimer::stop()
    {
      frameEndTime = std::chrono::steady_clock::now();

      smooth_nom  = smooth_nom * 0.8f + seconds();
      smooth_den  = smooth_den * 0.8f + 1.f;
    }

    inline double CodeTimer::seconds() const
    {
      auto diff = frameEndTime - frameStartTime;
      return std::chrono::duration<double>(diff).count();
    }

    inline double CodeTimer::milliseconds() const
    {
      auto diff = frameEndTime - frameStartTime;
      return std::chrono::duration<double, std::milli>(diff).count();
    }

    inline double CodeTimer::perSecond() const
    {
      return 1.0 / seconds();
    }

    inline double CodeTimer::secondsSmoothed() const
    {
      return 1.0 / perSecondSmoothed();
    }

    inline double CodeTimer::millisecondsSmoothed() const
    {
      return secondsSmoothed() * 1000.0;
    }

    inline double CodeTimer::perSecondSmoothed() const
    {
      return smooth_den / smooth_nom;
    }

  }  // namespace utility
}  // namespace ospcommon
