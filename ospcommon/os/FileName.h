// Copyright 2009-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "../common.h"

namespace ospcommon {

  /*! Convenience class for handling file names and paths. */
  class FileName
  {
   public:
    /*! create an empty filename */
    OSPCOMMON_INTERFACE FileName();

    /*! create a valid filename from a string */
    OSPCOMMON_INTERFACE FileName(const char *filename);

    /*! create a valid filename from a string */
    OSPCOMMON_INTERFACE FileName(const std::string &filename);

    /*! returns path to home folder */
    OSPCOMMON_INTERFACE static FileName homeFolder();

    /*! auto convert into a string */
    OSPCOMMON_INTERFACE operator std::string() const
    {
      return filename;
    }

    /*! returns a string of the filename */
    OSPCOMMON_INTERFACE const std::string &str() const
    {
      return filename;
    }

    /*! returns a c-string of the filename */
    OSPCOMMON_INTERFACE const char *c_str() const
    {
      return filename.c_str();
    }

    /*! returns the path of a filename with separator at the end */
    OSPCOMMON_INTERFACE std::string path() const;

    /*! returns the file of a filename  */
    OSPCOMMON_INTERFACE std::string base() const;

    /*! returns the base of a filename without extension */
    OSPCOMMON_INTERFACE std::string name() const;

    /*! returns the file extension */
    OSPCOMMON_INTERFACE std::string ext() const;

    /*! drops the file extension */
    OSPCOMMON_INTERFACE FileName dropExt() const;

    /*! replaces the file extension */
    OSPCOMMON_INTERFACE FileName setExt(const std::string &ext = "") const;

    /*! adds file extension */
    OSPCOMMON_INTERFACE FileName addExt(const std::string &ext = "") const;

    /*! concatenates two filenames to this/other */
    OSPCOMMON_INTERFACE FileName operator+(const FileName &other) const;

    /*! concatenates two filenames to this/other */
    OSPCOMMON_INTERFACE FileName operator+(const std::string &other) const;

    /*! removes the base from a filename (if possible) */
    OSPCOMMON_INTERFACE FileName operator-(const FileName &base) const;

    /*! == operator */
    OSPCOMMON_INTERFACE friend bool operator==(const FileName &a,
                                               const FileName &b);

    /*! != operator */
    OSPCOMMON_INTERFACE friend bool operator!=(const FileName &a,
                                               const FileName &b);

    /*! output operator */
    OSPCOMMON_INTERFACE friend std::ostream &operator<<(
        std::ostream &cout, const FileName &filename);

   private:
    std::string filename;
  };

}  // namespace ospcommon
