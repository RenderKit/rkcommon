// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../traits/osptraits.h"
#include "detail/parallel_for.inl"

#include <algorithm>

namespace ospcommon {
  namespace tasking {

    // NOTE(jda) - This abstraction wraps "fork-join" parallelism, with an
    //             implied synchronizsation after all of the tasks have run.
    template <typename INDEX_T, typename TASK_T>
    inline void parallel_for(INDEX_T nTasks, TASK_T &&fcn)
    {
      using namespace traits;
      static_assert(is_valid_index<INDEX_T>::value,
                    "ospcommon::tasking::parallel_for() requires the type"
                    " INDEX_T to be unsigned char, short, int, uint, long,"
                    " long long, unsigned long long or size_t.");

      static_assert(has_operator_method_matching_param<TASK_T, INDEX_T>::value,
                    "ospcommon::tasking::parallel_for() requires the "
                    "implementation of method "
                    "'void TASK_T::operator(P taskIndex), where P is of "
                    "type INDEX_T [first parameter of parallel_for()].");

      detail::parallel_for_impl(nTasks, std::forward<TASK_T>(fcn));
    }

    // NOTE(jda) - Allow serial version of parallel_for() without the need to
    //             change the entire tasking system backend
    template <typename INDEX_T, typename TASK_T>
    inline void serial_for(INDEX_T nTasks, const TASK_T &fcn)
    {
      using namespace traits;
      static_assert(is_valid_index<INDEX_T>::value,
                    "ospcommon::tasking::serial_for() requires the type"
                    " INDEX_T to be unsigned char, short, int, uint, long,"
                    " long long, unsigned long long or size_t.");

      static_assert(has_operator_method_matching_param<TASK_T, INDEX_T>::value,
                    "ospcommon::tasking::serial_for() requires the "
                    "implementation of method "
                    "'void TASK_T::operator(P taskIndex), where P is of "
                    "type INDEX_T [first parameter of serial_for()].");

      for (INDEX_T taskIndex = 0; taskIndex < nTasks; ++taskIndex)
        fcn(taskIndex);
    }

    /* NOTE(iw) - This abstraction extends the 'parallel_for' to mixed
       parallel/serial: we logically view the domain of N input tasks
       as grouped into roundUp(N/M) blocks of (at most) M items each;
       then 'itearte over the N/M blocks in parallel, and process each
       block serailly */
    template <int BLOCK_SIZE, typename INDEX_T, typename TASK_T>
    inline void parallel_in_blocks_of(INDEX_T nTasks, TASK_T &&fcn)
    {
      using namespace traits;
      static_assert(is_valid_index<INDEX_T>::value,
                    "ospcommon::tasking::parallel_for() requires the type"
                    " INDEX_T to be unsigned char, short, int, uint, long,"
                    " or size_t.");

      INDEX_T numBlocks = (nTasks + BLOCK_SIZE - 1) / BLOCK_SIZE;
      parallel_for(numBlocks, [&](INDEX_T blockID) {
        INDEX_T begin = blockID * (INDEX_T)BLOCK_SIZE;
        INDEX_T end   = std::min(begin + (INDEX_T)BLOCK_SIZE, nTasks);
        fcn(begin, end);
      });
    }

  }  // namespace tasking
}  // namespace ospcommon
