## Copyright 2009-2021 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

if (WIN32 AND RKCOMMON_TASKING_TBB)
  if (NOT TBB_ARCH)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
      set(TBB_ARCH intel64)
    else()
      set(TBB_ARCH ia32)
    endif()
  endif()

  set(TBB_DLL_HINTS
    HINTS
    ${RKCOMMON_TBB_ROOT}/../redist/${TBB_ARCH}_win/tbb/vc14
    ${RKCOMMON_TBB_ROOT}/../redist/${TBB_ARCH}_win/tbb/vc14
    ${RKCOMMON_TBB_ROOT}/../redist/${TBB_ARCH}/tbb/vc14
    ${RKCOMMON_TBB_ROOT}/../redist/${TBB_ARCH}/tbb/vc14
    ${RKCOMMON_TBB_ROOT}/../redist/${TBB_ARCH}/vc14
    ${RKCOMMON_TBB_ROOT}/redist/${TBB_ARCH}/vc14
    ${RKCOMMON_TBB_ROOT}/bin/${TBB_ARCH}/vc14
    ${RKCOMMON_TBB_ROOT}/bin
  )

  find_file(TBB_DLL NAMES tbb12.dll tbb.dll ${TBB_DLL_HINTS})
  find_file(TBB_DLL_DEBUG NAMES tbb12_debug.dll tbb_debug.dll ${TBB_DLL_HINTS})
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
