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
