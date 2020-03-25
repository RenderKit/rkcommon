// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "ParameterizedObject.h"

#include <algorithm>

namespace ospcommon {
  namespace utility {

    ParameterizedObject::Param::Param(const std::string &_name) : name(_name) {}

    void ParameterizedObject::removeParam(const std::string &name)
    {
      auto foundParam = std::find_if(
          paramList.begin(),
          paramList.end(),
          [&](const std::shared_ptr<Param> &p) { return p->name == name; });

      if (foundParam != paramList.end()) {
        paramList.erase(foundParam);
      }
    }

    ParameterizedObject::Param *ParameterizedObject::findParam(
        const std::string &name, bool addIfNotExist)
    {
      auto foundParam = std::find_if(
          paramList.begin(),
          paramList.end(),
          [&](const std::shared_ptr<Param> &p) { return p->name == name; });

      if (foundParam != paramList.end())
        return foundParam->get();
      else if (addIfNotExist) {
        paramList.push_back(std::make_shared<Param>(name));
        return paramList[paramList.size() - 1].get();
      } else
        return nullptr;
    }
  }  // namespace utility
}  // namespace ospcommon
