// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#pragma once

// Quiet `fopen` MSVC warning
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <errno.h>
#include <stdio.h>
#include <string>

#include "../math/vec.h"
#include "../memory/malloc.h"

namespace rkcommon {
  namespace utility {

    template <typename COMP_T,
              int N_COMP,
              typename PIXEL_T,
              int PIXEL_COMP,
              bool FLIP>
    inline void writeImage(const std::string &fileName,
                           const char *const header,
                           const int sizeX,
                           const int sizeY,
                           const PIXEL_T *const pixel)
    {
      FILE *file = fopen(fileName.c_str(), "wb");
      if (file == nullptr)
        throw std::runtime_error("Can't open file for writeP[FP]M!");

      fprintf(file, header, sizeX, sizeY);
      auto out = STACK_BUFFER(COMP_T, N_COMP * sizeX);
      for (int y = 0; y < sizeY; y++) {
        auto *in = (const COMP_T *)&pixel[(FLIP ? sizeY - 1 - y : y) * sizeX];
        for (int x = 0; x < sizeX; x++)
          for (int c = 0; c < N_COMP; c++)
            out[N_COMP * x + c] = in[PIXEL_COMP * x + (N_COMP == 1 ? 3 : c)];
        fwrite(out, N_COMP * sizeX, sizeof(COMP_T), file);
      }
      fprintf(file, "\n");
      fclose(file);
    }

    inline void writePPM(const std::string &fileName,
                         const int sizeX,
                         const int sizeY,
                         const uint32_t *pixel)
    {
      writeImage<unsigned char, 3, uint32_t, 4, true>(
          fileName, "P6\n%i %i\n255\n", sizeX, sizeY, pixel);
    }

    inline void writePGM(const std::string &fileName,
                         const int sizeX,
                         const int sizeY,
                         const uint32_t *pixel)
    {
      writeImage<unsigned char, 1, uint32_t, 4, true>(
          fileName, "P5\n%i %i\n255\n", sizeX, sizeY, pixel);
    }

    template <typename T>
    inline void writePFM(const std::string &fName,
        const int sizeX,
        const int sizeY,
        const T *p) = delete;

    using namespace rkcommon::math;

    template <>
    inline void writePFM<float>(const std::string &fName,
        const int sizeX,
        const int sizeY,
        const float *p)
    {
      writeImage<float, 1, float, 1, false>(
          fName, "Pf\n%i %i\n-1.0\n", sizeX, sizeY, p);
    }
    template <>
    inline void writePFM<vec3f>(const std::string &fName,
        const int sizeX,
        const int sizeY,
        const vec3f *p)
    {
      writeImage<float, 3, vec3f, 3, false>(
          fName, "PF\n%i %i\n-1.0\n", sizeX, sizeY, p);
    }
    template <>
    inline void writePFM<vec3fa>(const std::string &fName,
        const int sizeX,
        const int sizeY,
        const vec3fa *p)
    {
      writeImage<float, 3, vec3fa, 4, false>(
          fName, "PF\n%i %i\n-1.0\n", sizeX, sizeY, p);
    }
    template <>
    inline void writePFM<vec4f>(const std::string &fName,
        const int sizeX,
        const int sizeY,
        const vec4f *p)
    {
      writeImage<float, 3, vec4f, 4, false>(
          fName, "PF\n%i %i\n-1.0\n", sizeX, sizeY, p);
    }

  }  // namespace utility
}  // namespace rkcommon
