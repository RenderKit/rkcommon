// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include "common.h"
#include "os/library.h"
#include <cmath>

namespace rkcommon {

  void removeArgs(int &ac, const char **&av, int where, int howMany)
  {
    for (int i = where + howMany; i < ac; i++)
      av[i - howMany] = av[i];
    ac -= howMany;
  }

  void loadLibrary(const std::string &name, const void *anchorAddress)
  {
    LibraryRepository::getInstance()->add(name, anchorAddress);
  }

  void unloadLibrary(const std::string &name)
  {
    LibraryRepository::getInstance()->remove(name);
  }

  void loadDefaultLibrary()
  {
    LibraryRepository::getInstance()->addDefaultLibrary();
  }

  void *getSymbol(const std::string &name)
  {
    return LibraryRepository::getInstance()->getSymbol(name);
  }

#ifdef _WIN32
#define osp_snprintf sprintf_s
#else
#define osp_snprintf snprintf
#endif

  std::string prettyDouble(double val)
  {
    const double absVal = std::abs(val);
    char result[1000];

    if (absVal >= 1e+15f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e18f, 'E');
    else if (absVal >= 1e+15f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e15f, 'P');
    else if (absVal >= 1e+12f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e12f, 'T');
    else if (absVal >= 1e+09f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e09f, 'G');
    else if (absVal >= 1e+06f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e06f, 'M');
    else if (absVal >= 1e+03f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e03f, 'k');
    else if (absVal <= 1e-12f)
      osp_snprintf(result, 1000, "%.1f%c", val * 1e15f, 'f');
    else if (absVal <= 1e-09f)
      osp_snprintf(result, 1000, "%.1f%c", val * 1e12f, 'p');
    else if (absVal <= 1e-06f)
      osp_snprintf(result, 1000, "%.1f%c", val * 1e09f, 'n');
    else if (absVal <= 1e-03f)
      osp_snprintf(result, 1000, "%.1f%c", val * 1e06f, 'u');
    else if (absVal <= 1e-00f)
      osp_snprintf(result, 1000, "%.1f%c", val * 1e03f, 'm');
    else
      osp_snprintf(result, 1000, "%f", (float)val);
    return result;
  }

  std::string prettyNumber(size_t s)
  {
    const double val = s;
    char result[1000];

    if (val >= 1e+15f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e18f, 'E');
    else if (val >= 1e+15f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e15f, 'P');
    else if (val >= 1e+12f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e12f, 'T');
    else if (val >= 1e+09f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e09f, 'G');
    else if (val >= 1e+06f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e06f, 'M');
    else if (val >= 1e+03f)
      osp_snprintf(result, 1000, "%.1f%c", val / 1e03f, 'k');
    else
      osp_snprintf(result, 1000, "%zu", s);
    return result;
  }

#undef osp_snprintf

}  // namespace rkcommon
