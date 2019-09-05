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

#include <atomic>
#include <stdexcept>
#include <type_traits>

namespace ospcommon {
  namespace memory {

    class RefCountedObject
    {
     public:
      RefCountedObject()          = default;
      virtual ~RefCountedObject() = default;

      RefCountedObject(const RefCountedObject &) = default;
      RefCountedObject &operator=(const RefCountedObject &) = default;

      void refInc() const;
      void refDec() const;

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

    ///////////////////////////////////////////////////////////////////////////
    // Pointer to a RefCountedObject ////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////

    template <typename T>
    class IntrusivePtr
    {
      static_assert(std::is_base_of<RefCountedObject, T>::value,
                    "IntrusivePtr<T> can onyl be used with objects derived "
                    "from RefCountedObject");

     public:
      T *ptr{nullptr};

      IntrusivePtr() = default;
      ~IntrusivePtr();

      IntrusivePtr(const IntrusivePtr &input);
      IntrusivePtr(T *const input);

      IntrusivePtr &operator=(const IntrusivePtr &input);
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
    inline IntrusivePtr<T>::IntrusivePtr(const IntrusivePtr &input)
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
}  // namespace ospcommon
