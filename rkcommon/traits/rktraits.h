// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <cstdlib>
#include <type_traits>

namespace rkcommon {
  namespace traits {

    using byte_t = unsigned char;

    // C++14 traits for C++11 /////////////////////////////////////////////////

    template <bool B, class T = void>
    using enable_if_t = typename std::enable_if<B, T>::type;

    // Helper operators ///////////////////////////////////////////////////////

    template <typename T, typename Arg>
    std::true_type operator==(const T &, const Arg &);

    // type 'T' having '==' operator //////////////////////////////////////////

    template <typename T, typename Arg = T>
    struct HasOperatorEqualsT
    {
      enum
      {
        value = !std::is_same<decltype(*(T *)(0) == *(Arg *)(0)),
                              std::true_type>::value
      };
    };

    template <typename T, typename TYPE>
    using HasOperatorEquals =
        typename std::enable_if<HasOperatorEqualsT<T>::value, TYPE>::type;

    template <typename T, typename TYPE>
    using NoOperatorEquals =
        typename std::enable_if<!HasOperatorEqualsT<T>::value, TYPE>::type;

    // type 'T' (decayed) is a valid parallel_for() index type ////////////////

    template <typename T>
    struct is_valid_index
    {
      using TYPE = typename std::decay<T>::type;

      enum
      {
        value = std::is_same<TYPE, byte_t>::value ||
                std::is_same<TYPE, short>::value ||
                std::is_same<TYPE, int>::value ||
                std::is_same<TYPE, unsigned>::value ||
                std::is_same<TYPE, long>::value ||
                std::is_same<TYPE, long long>::value ||
                std::is_same<TYPE, unsigned long long>::value ||
                std::is_same<TYPE, size_t>::value
      };
    };

    // type 'T' implementing T::operator() ////////////////////////////////////

    // NOTE(jda) - This checks at compile time if T implements the method
    //            'void T::operator()'.
    template <typename TASK>
    struct has_operator_method
    {
      using TASK_T = typename std::decay<TASK>::type;

      template <class, class>
      class checker;

      template <typename C>
      static std::true_type test(checker<C, decltype(&C::operator())> *);

      template <typename C>
      static std::false_type test(...);

      using type              = decltype(test<TASK_T>(nullptr));
      static const bool value = std::is_same<std::true_type, type>::value;
    };

    // type 'T' implementing T::operator(P) with P being integral /////////////

#ifdef _WIN32
    template <typename TASK, typename EXPECTED_PARAM_T>
    using has_operator_method_matching_param = has_operator_method<TASK>;
#else
    // NOTE(jda) - This checks at compile time if T implements the method
    //            'void T::operator(P taskIndex)', where P matches the second
    //            template parameter 'EXPECTED_PARAM_T'
    template <typename TASK, typename EXPECTED_PARAM_T>
    struct has_operator_method_matching_param
    {
      using TASK_T = typename std::decay<TASK>::type;

      template <typename P>
      using t_param     = void (TASK_T::*)(P) const;
      using operator_t  = decltype(&TASK_T::operator());
      using valid_param = std::is_same<t_param<EXPECTED_PARAM_T>, operator_t>;

      static const bool value =
          has_operator_method<TASK_T>::value && valid_param::value;
    };
#endif

    // type 'DERIVED' (decayed) comes from 'BASE' /////////////////////////////

    template <typename DERIVED, typename BASE>
    using is_base_of_t = enable_if_t<
        std::is_base_of<BASE, typename std::decay<DERIVED>::type>::value>;

    // type 'T' (decayed) is a class/struct ///////////////////////////////////

    template <typename T>
    using is_class_t =
        enable_if_t<std::is_class<typename std::decay<T>::type>::value>;

    // type 'T1' and 'T2' are not the same ////////////////////////////////////

    template <typename T1, typename T2>
    using is_not_same_t = enable_if_t<!std::is_same<T1, T2>::value>;

    // If a single type is convertible to another /////////////////////////////

    template <typename FROM, typename TO>
    using can_convert = std::is_convertible<TO, FROM>;

    template <typename FROM, typename TO>
    using can_convert_t = enable_if_t<can_convert<FROM, TO>::value>;

    // type 'T' is arithmetic /////////////////////////////////////////////////

    template <typename T>
    using is_arithmetic_t = enable_if_t<std::is_arithmetic<T>::value>;

    // type 'T1' and 'T2' are not the same and arithmetic /////////////////////

    template <typename T1, typename T2>
    using is_not_same_and_arithmetic_t = enable_if_t<!std::is_same<T1, T2>::value
        && std::is_arithmetic<T1>::value && std::is_arithmetic<T2>::value>;

  }  // namespace traits
}  // namespace rkcommon
