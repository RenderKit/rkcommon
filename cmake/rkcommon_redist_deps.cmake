## Copyright 2009-2020 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

if (WIN32 AND RKCOMMON_TASKING_TBB)
  set(TBB_DLL_HINTS
    HINTS
    ${RKCOMMON_TBB_ROOT}/../redist/${TBB_ARCH}_win/tbb/vc14
    ${RKCOMMON_TBB_ROOT}/../redist/${TBB_ARCH}/tbb/vc14
    ${RKCOMMON_TBB_ROOT}/bin/${TBB_ARCH}/vc14
    ${RKCOMMON_TBB_ROOT}/bin
  )
  find_file(TBB_DLL tbb.dll ${TBB_DLL_HINTS})
  find_file(TBB_DLL_DEBUG tbb_debug.dll ${TBB_DLL_HINTS})
  find_file(TBB_DLL_MALLOC tbbmalloc.dll ${TBB_DLL_HINTS})
  find_file(TBB_DLL_MALLOC_DEBUG tbbmalloc_debug.dll ${TBB_DLL_HINTS})
  mark_as_advanced(TBB_DLL)
  mark_as_advanced(TBB_DLL_DEBUG)
  mark_as_advanced(TBB_DLL_MALLOC)
  mark_as_advanced(TBB_DLL_MALLOC_DEBUG)
  install(PROGRAMS ${TBB_DLL} ${TBB_DLL_MALLOC}
          DESTINATION ${CMAKE_INSTALL_BINDIR}
          CONFIGURATIONS Release RelWithDebInfo)
  install(PROGRAMS ${TBB_DLL_DEBUG} ${TBB_DLL_MALLOC_DEBUG}
          DESTINATION ${CMAKE_INSTALL_BINDIR}
          CONFIGURATIONS Debug)
endif()
