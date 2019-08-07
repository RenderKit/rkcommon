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

#include "common.h"
#include "os/library.h"

namespace ospcommon {

  void removeArgs(int &ac, const char **&av, int where, int howMany)
  {
    for (int i = where + howMany; i < ac; i++)
      av[i - howMany] = av[i];
    ac -= howMany;
  }

  void loadLibrary(const std::string &name, bool anchor)
  {
    LibraryRepository::getInstance()->add(name, anchor);
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
    const double absVal = abs(val);
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
      osp_snprintf(result, 1000, "%lu", s);
    return result;
  }

#undef osp_snprintf

}  // namespace ospcommon
