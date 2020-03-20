// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"

#include <atomic>

namespace ospcommon {
  namespace utility {

    struct OSPCOMMON_INTERFACE TimeStamp
    {
      TimeStamp() = default;
      TimeStamp(const TimeStamp &);
      TimeStamp(TimeStamp &&);

      TimeStamp &operator=(const TimeStamp &);
      TimeStamp &operator=(TimeStamp &&);

      operator size_t() const;

      void renew();

     private:
      static size_t nextValue();

      // Data members //

      std::atomic<size_t> value{nextValue()};

      //! \brief the uint64_t that stores the time value
      static std::atomic<size_t> global;
    };

  }  // namespace utility
}  // namespace ospcommon
