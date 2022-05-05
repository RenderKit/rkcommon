// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <mutex>
#include <utility>
#include <vector>

namespace rkcommon {
  namespace containers {

    template <typename T>
    struct TransactionalBuffer
    {
      TransactionalBuffer() = default;

      // Insert into the buffer (producer)
      void push_back(const T &);
      void push_back(T &&);

      // Take all contents of the buffer (consumer)
      std::vector<T> consume();

      size_t size() const;

      bool empty() const;

     private:
      // Data members //

      std::vector<T> buffer;

      mutable std::mutex
          bufferMutex;  // NOTE(jda) - Marked mutable so 'const'
                        //            methods can take the lock...
    };

    // Inlined members ////////////////////////////////////////////////////////

    template <typename T>
    inline void TransactionalBuffer<T>::push_back(const T &v)
    {
      std::lock_guard<std::mutex> lock(bufferMutex);
      buffer.push_back(v);
    }

    template <typename T>
    inline void TransactionalBuffer<T>::push_back(T &&v)
    {
      std::lock_guard<std::mutex> lock(bufferMutex);
      buffer.push_back(std::forward<T>(v));
    }

    template <typename T>
    inline std::vector<T> TransactionalBuffer<T>::consume()
    {
      std::lock_guard<std::mutex> lock(bufferMutex);
      return std::move(buffer);
    }

    template <typename T>
    inline size_t TransactionalBuffer<T>::size() const
    {
      std::lock_guard<std::mutex> lock(bufferMutex);
      return buffer.size();
    }

    template <typename T>
    inline bool TransactionalBuffer<T>::empty() const
    {
      std::lock_guard<std::mutex> lock(bufferMutex);
      return buffer.empty();
    }

  }  // namespace containers
}  // namespace rkcommon
