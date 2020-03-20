// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

/*! \file PseudoURL: splits a 'pseudo-url' of the form
  '<type>://<fileanme>[:name=value]*' into its components of 'type'
    (e.g, 'points', 'slines', etc), filename, and 'name=value'
    argument pairs (e.g., 'format=xyzrgb') */

#include <string>
#include <vector>
#include "../common.h"

namespace ospcommon {
  namespace utility {

    //! \brief Tokenize the string passed on the desired delimiter
    void tokenize(const std::string &str,
                  const char delim,
                  std::vector<std::string> &tokens);

    /* a pseudo-url is of the form '<type>://<filename>[:name=value]*'
       into its components of 'type' (e.g, 'points', 'lines', etc),
       filename, and 'name=value' argument pairs (e.g.,
       'format=xyzrgb'). This class takes a string and splits it into these
       components */
    struct PseudoURL
    {
      /*! constructor - parse the given string into its components */
      PseudoURL(const std::string &inputString);

      /*! return the parsed type. may we empty string if none was specified */
      std::string getType() const;

      /*! return the parsed file name specifier. cannot be empty
        string */
      std::string getFileName() const;

      /*! return value for given parameters name, or throw an exception
        if not specified */
      std::string getValue(const std::string &name) const;

      /*! check if the given parameter was specified */
      bool hasParam(const std::string &name);

     private:
      /*! the type of the psueod-url, eg, for 'points://file.raw' this
          would be 'points'. If no "://" is specified, this gets set to "" */
      std::string type;
      /*! the filename - the thing after the <type>://, and before the
          ":" that starts parameters */
      std::string fileName;

      /*! the name-value pairs specified as parameters */
      std::vector<std::pair<std::string, std::string>> params;
    };

  }  // namespace utility
}  // namespace ospcommon
