// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "TimeStamp.h"

namespace rkcommon {
  namespace utility {

    std::atomic<size_t> TimeStamp::global{0};

    TimeStamp::TimeStamp(const TimeStamp &other)
    {
      this->value = other.value.load();
    }

    TimeStamp::TimeStamp(TimeStamp &&other)
    {
      this->value = other.value.load();
    }

    TimeStamp &TimeStamp::operator=(const TimeStamp &other)
    {
      this->value = other.value.load();
      return *this;
    }

    TimeStamp &TimeStamp::operator=(TimeStamp &&other)
    {
      this->value = other.value.load();
      return *this;
    }

    TimeStamp::operator size_t() const
    {
      return value;
    }

    void TimeStamp::renew()
    {
      value = nextValue();
    }

    size_t TimeStamp::nextValue()
    {
      return global++;
    }

  }  // namespace utility
}  // namespace rkcommon
