## ======================================================================== ##
## Copyright 2009-2019 Intel Corporation                                    ##
##                                                                          ##
## Licensed under the Apache License, Version 2.0 (the "License");          ##
## you may not use this file except in compliance with the License.         ##
## You may obtain a copy of the License at                                  ##
##                                                                          ##
##     http://www.apache.org/licenses/LICENSE-2.0                           ##
##                                                                          ##
## Unless required by applicable law or agreed to in writing, software      ##
## distributed under the License is distributed on an "AS IS" BASIS,        ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. ##
## See the License for the specific language governing permissions and      ##
## limitations under the License.                                           ##
## ======================================================================== ##

if (WIN32 AND OSPCOMMON_TASKING_TBB)
  set(TBB_DLL_HINTS
    HINTS
    ${OSPCOMMON_TBB_ROOT}/../redist/${TBB_ARCH}_win/tbb/vc14
    ${OSPCOMMON_TBB_ROOT}/../redist/${TBB_ARCH}/tbb/vc14
    ${OSPCOMMON_TBB_ROOT}/bin/${TBB_ARCH}/vc14
    ${OSPCOMMON_TBB_ROOT}/bin
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
