// Copyright 2009-2020 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <atomic>
#include <stdexcept>
#include <type_traits>

namespace rkcommon {
  namespace memory {

    class RefCountedObject
    {
     public:
      RefCountedObject()          = default;
      virtual ~RefCountedObject() = default;

      RefCountedObject(const RefCountedObject &) = delete;
      RefCountedObject &operator=(const RefCountedObject &) = delete;
      RefCountedObject(RefCountedObject &&) = delete;
      RefCountedObject &operator=(RefCountedObject &&) = delete;

      void refInc() const;
      void refDec() const;
      long long useCount() const;

     private:
      mutable std::atomic<long long> refCounter{1};
    };

    // Inlined definitions //

    inline void RefCountedObject::refInc() const
    {
      refCounter++;
    }

    inline void RefCountedObject::refDec() const
    {
      if ((--refCounter) == 0)
        delete this;
    }

    inline long long RefCountedObject::useCount() const
    {
      return refCounter.load();
    }

    ///////////////////////////////////////////////////////////////////////////
    // Pointer to a RefCountedObject ////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    template <typename T>
    class IntrusivePtr
    {
      static_assert(std::is_base_of<RefCountedObject, T>::value,
                    "IntrusivePtr<T> can only be used with objects derived "
                    "from RefCountedObject");

     public:
      T *ptr{nullptr};

      IntrusivePtr() = default;
      ~IntrusivePtr();

      IntrusivePtr(const IntrusivePtr &input);
      IntrusivePtr(IntrusivePtr &&input);

      template <typename O>
      IntrusivePtr(const IntrusivePtr<O> &input);
      IntrusivePtr(T *const input);

      IntrusivePtr &operator=(const IntrusivePtr &input);
      IntrusivePtr &operator=(IntrusivePtr &&input);
      IntrusivePtr &operator=(T *input);

      operator bool() const;

      const T &operator*() const;
      T &operator*();

      const T *operator->() const;
      T *operator->();
    };

    // Inlined definitions //

    template <typename T>
    inline IntrusivePtr<T>::~IntrusivePtr()
    {
      if (ptr)
        ptr->refDec();
    }

    template <typename T>
    inline IntrusivePtr<T>::IntrusivePtr(const IntrusivePtr<T> &input)
        : ptr(input.ptr)
    {
      if (ptr)
        ptr->refInc();
    }

    template <typename T>
    inline IntrusivePtr<T>::IntrusivePtr(IntrusivePtr<T> &&input)
        : ptr(input.ptr)
    {
      input.ptr = nullptr;
    }

    template <typename T>
    template <typename O>
    inline IntrusivePtr<T>::IntrusivePtr(const IntrusivePtr<O> &input)
        : ptr(input.ptr)
    {
      if (ptr)
        ptr->refInc();
    }

    template <typename T>
    inline IntrusivePtr<T>::IntrusivePtr(T *const input) : ptr(input)
    {
      if (ptr)
        ptr->refInc();
    }

    template <typename T>
    inline IntrusivePtr<T> &IntrusivePtr<T>::operator=(
        const IntrusivePtr &input)
    {
      if (input.ptr)
        input.ptr->refInc();
      if (ptr)
        ptr->refDec();
      ptr = input.ptr;
      return *this;
    }

    template <typename T>
    inline IntrusivePtr<T> &IntrusivePtr<T>::operator=(IntrusivePtr &&input)
    {
      ptr       = input.ptr;
      input.ptr = nullptr;
      return *this;
    }

    template <typename T>
    inline IntrusivePtr<T> &IntrusivePtr<T>::operator=(T *input)
    {
      if (input)
        input->refInc();
      if (ptr)
        ptr->refDec();
      ptr = input;
      return *this;
    }

    template <typename T>
    inline IntrusivePtr<T>::operator bool() const
    {
      return ptr != nullptr;
    }

    template <typename T>
    inline const T &IntrusivePtr<T>::operator*() const
    {
      return *ptr;
    }

    template <typename T>
    inline T &IntrusivePtr<T>::operator*()
    {
      return *ptr;
    }

    template <typename T>
    inline const T *IntrusivePtr<T>::operator->() const
    {
      return ptr;
    }

    template <typename T>
    inline T *IntrusivePtr<T>::operator->()
    {
      return ptr;
    }

    // Inlined operators //////////////////////////////////////////////////////

    template <typename T>
    inline bool operator<(const IntrusivePtr<T> &a, const IntrusivePtr<T> &b)
    {
      return a.ptr < b.ptr;
    }

    template <typename T>
    bool operator==(const IntrusivePtr<T> &a, const IntrusivePtr<T> &b)
    {
      return a.ptr == b.ptr;
    }

    template <typename T>
    bool operator!=(const IntrusivePtr<T> &a, const IntrusivePtr<T> &b)
    {
      return a.ptr != b.ptr;
    }

  }  // namespace memory
}  // namespace rkcommon
