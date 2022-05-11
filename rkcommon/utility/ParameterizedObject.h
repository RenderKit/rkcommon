// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

// stl
#include <vector>
// rkcommon
#include "Any.h"

namespace rkcommon {
  namespace utility {

    /*! \brief defines a basic object whose lifetime is managed by ospray */
    struct RKCOMMON_INTERFACE ParameterizedObject
    {
      ParameterizedObject()          = default;
      virtual ~ParameterizedObject() = default;

      /*! \brief container for _any_ sort of parameter an app can assign
          to an ospray object */
      struct RKCOMMON_INTERFACE Param
      {
        Param(const std::string &name);
        ~Param() = default;

        template <typename T>
        void set(const T &v);

        utility::Any data;
        std::string name;

        bool query = false;
      };

      /*! \brief check if a given parameter is available */
      bool hasParam(const std::string &name);

      /*! set a parameter with given name to given value, create param if not
       *  existing */
      template <typename T>
      void setParam(const std::string &name, const T &t);

      template <typename T>
      T getParam(const std::string &name, T valIfNotFound);

      void removeParam(const std::string &name);

      void resetAllParamQueryStatus();

     protected:
      Param *findParam(const std::string &name, bool addIfNotExist = false);

      std::vector<std::shared_ptr<Param>>::iterator params_begin();
      std::vector<std::shared_ptr<Param>>::iterator params_end();

     private:
      // Data members //

      /*! \brief list of parameters attached to this object */
      // NOTE(jda) - Use std::shared_ptr because copy/move of a
      //             ParameterizedObject would end up copying parameters, where
      //             destruction of each copy should only result in freeing the
      //             parameters *once*
      std::vector<std::shared_ptr<Param>> paramList;
    };

    // Inlined ParameterizedObject definitions ////////////////////////////////

    template <typename T>
    inline void ParameterizedObject::Param::set(const T &v)
    {
      data = v;
    }

    inline bool ParameterizedObject::hasParam(const std::string &name)
    {
      return findParam(name, false) != nullptr;
    }

    template <typename T>
    inline void ParameterizedObject::setParam(const std::string &name,
                                              const T &t)
    {
      findParam(name, true)->set(t);
    }

    template <typename T>
    inline T ParameterizedObject::getParam(const std::string &name,
                                           T valIfNotFound)
    {
      Param *param = findParam(name);
      if (!param)
        return valIfNotFound;
      if (!param->data.is<T>())
        return valIfNotFound;
      param->query = true;
      return param->data.get<T>();
    }

    inline void ParameterizedObject::resetAllParamQueryStatus()
    {
      for (auto p = params_begin(); p != params_end(); ++p)
        (*p)->query = false;
    }

    inline std::vector<std::shared_ptr<ParameterizedObject::Param>>::iterator
    ParameterizedObject::params_begin()
    {
      return paramList.begin();
    }

    inline std::vector<std::shared_ptr<ParameterizedObject::Param>>::iterator
    ParameterizedObject::params_end()
    {
      return paramList.end();
    }

  }  // namespace utility
}  // namespace rkcommon
