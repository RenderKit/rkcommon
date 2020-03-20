// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "parallel_for.h"

#include <iterator>

namespace ospcommon {
  namespace tasking {

    template <typename ITERATOR_T, typename TASK_T>
    inline void parallel_foreach(ITERATOR_T begin, ITERATOR_T end, TASK_T &&f)
    {
      using ITERATOR_KIND =
          typename std::iterator_traits<ITERATOR_T>::iterator_category;

      static_assert(
          std::is_same<ITERATOR_KIND, std::random_access_iterator_tag>::value,
          "ospcommon::tasking::parallel_foreach() requires random-"
          "access iterators!");

      const size_t count = std::distance(begin, end);
      auto *v            = &(*begin);

      parallel_for(count, [&](size_t i) { f(v[i]); });
    }

    template <typename CONTAINER_T, typename TASK_T>
    inline void parallel_foreach(CONTAINER_T &&c, TASK_T &&f)
    {
      parallel_foreach(std::begin(c), std::end(c), std::forward<TASK_T>(f));
    }

  }  // namespace tasking
}  // namespace ospcommon
