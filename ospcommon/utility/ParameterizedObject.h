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

// stl
#include <vector>
// ospcommon
#include "Any.h"

namespace ospcommon {
  namespace utility {

    /*! \brief defines a basic object whose lifetime is managed by ospray */
    struct OSPCOMMON_INTERFACE ParameterizedObject
    {
      ParameterizedObject()          = default;
      virtual ~ParameterizedObject() = default;

      /*! \brief container for _any_ sort of parameter an app can assign
          to an ospray object */
      struct OSPCOMMON_INTERFACE Param
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
      param->query = true;
      if (!param->data.is<T>())
        return valIfNotFound;
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
}  // namespace ospcommon
