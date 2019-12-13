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

set(TBB_VERSION_REQUIRED "4.4")

if (NOT OSPCOMMON_TBB_ROOT)
  set(OSPCOMMON_TBB_ROOT $ENV{OSPCOMMON_TBB_ROOT})
endif()
if (NOT OSPCOMMON_TBB_ROOT)
  set(OSPCOMMON_TBB_ROOT $ENV{TBB_ROOT})
endif()
if (NOT OSPCOMMON_TBB_ROOT)
  set(OSPCOMMON_TBB_ROOT $ENV{TBBROOT})
endif()

## Detect changed OSPCOMMON_TBB_ROOT ##

if (NOT OSPCOMMON_TBB_ROOT STREQUAL OSPCOMMON_TBB_ROOT_LAST)
  unset(TBB_INCLUDE_DIR CACHE)
  unset(TBB_LIBRARY CACHE)
  unset(TBB_LIBRARY_DEBUG CACHE)
  unset(TBB_LIBRARY_MALLOC CACHE)
  unset(TBB_LIBRARY_MALLOC_DEBUG CACHE)
endif()

if (WIN32)
  # workaround for parentheses in variable name / CMP0053
  set(PROGRAMFILESx86 "PROGRAMFILES(x86)")
  set(PROGRAMFILES32 "$ENV{${PROGRAMFILESx86}}")
  if (NOT PROGRAMFILES32)
    set(PROGRAMFILES32 "$ENV{PROGRAMFILES}")
  endif()
  if (NOT PROGRAMFILES32)
    set(PROGRAMFILES32 "C:/Program Files (x86)")
  endif()
  find_path(OSPCOMMON_TBB_ROOT include/tbb/task_scheduler_init.h
    DOC "Root of TBB installation"
    HINTS ${OSPCOMMON_TBB_ROOT}
    PATHS
      ${PROJECT_SOURCE_DIR}/tbb
      ${PROJECT_SOURCE_DIR}/../tbb
      "${PROGRAMFILES32}/IntelSWTools/compilers_and_libraries/windows/tbb"
      "${PROGRAMFILES32}/Intel/Composer XE/tbb"
      "${PROGRAMFILES32}/Intel/compilers_and_libraries/windows/tbb"
  )

  if (CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(TBB_ARCH intel64)
  else()
    set(TBB_ARCH ia32)
  endif()

  set(TBB_LIBDIR ${OSPCOMMON_TBB_ROOT}/lib)

  find_path(TBB_INCLUDE_DIR tbb/task_scheduler_init.h PATHS ${OSPCOMMON_TBB_ROOT}/include NO_DEFAULT_PATH)
  set(TBB_LIB_HINTS
    PATHS
    ${TBB_LIBDIR}/${TBB_ARCH}/vc14
    ${TBB_LIBDIR}
    NO_DEFAULT_PATH
  )
  find_library(TBB_LIBRARY tbb ${TBB_LIB_HINTS})
  find_library(TBB_LIBRARY_DEBUG tbb_debug ${TBB_LIB_HINTS})
  find_library(TBB_LIBRARY_MALLOC tbbmalloc ${TBB_LIB_HINTS})
  find_library(TBB_LIBRARY_MALLOC_DEBUG tbbmalloc_debug ${TBB_LIB_HINTS})

else ()

  find_path(OSPCOMMON_TBB_ROOT include/tbb/task_scheduler_init.h
    DOC "Root of TBB installation"
    HINTS ${OSPCOMMON_TBB_ROOT}
    PATHS
      ${PROJECT_SOURCE_DIR}/tbb
      /opt/intel/composerxe/tbb
      /opt/intel/compilers_and_libraries/tbb
      /opt/intel/tbb
  )

  find_path(TBB_INCLUDE_DIR tbb/task_scheduler_init.h PATHS ${OSPCOMMON_TBB_ROOT}/include NO_DEFAULT_PATH)

  if (APPLE)
    set(TBB_HINTS PATHS ${OSPCOMMON_TBB_ROOT}/lib NO_DEFAULT_PATH)
  else()
    file(GLOB TBB_GCC_LIB_HINTS ${OSPCOMMON_TBB_ROOT}/lib/intel64/gcc*)
    list(REVERSE TBB_GCC_LIB_HINTS)
    set(TBB_HINTS
      HINTS
        ${TBB_GCC_LIB_HINTS}
        ${OSPCOMMON_TBB_ROOT}/lib
        ${OSPCOMMON_TBB_ROOT}/lib64
      PATHS
        /usr/lib/x86_64-linux-gnu/
      )
  endif()

  find_library(TBB_LIBRARY tbb ${TBB_HINTS})
  find_library(TBB_LIBRARY_DEBUG tbb_debug ${TBB_HINTS})
  find_library(TBB_LIBRARY_MALLOC tbbmalloc ${TBB_HINTS})
  find_library(TBB_LIBRARY_MALLOC_DEBUG tbbmalloc_debug ${TBB_HINTS})
endif()

set(OSPCOMMON_TBB_ROOT_LAST ${OSPCOMMON_TBB_ROOT} CACHE INTERNAL "Last value of OSPCOMMON_TBB_ROOT to detect changes")

set(TBB_ERROR_MESSAGE
  "Threading Building Blocks (TBB) with minimum version ${TBB_VERSION_REQUIRED} not found.
OSPRay uses TBB as default tasking system. Please make sure you have the TBB headers installed as well (the package is typically named 'libtbb-dev' or 'tbb-devel') and/or hint the location of TBB in OSPCOMMON_TBB_ROOT.
Alternatively, you can try to use OpenMP as tasking system by setting OSPCOMMON_TASKING_SYSTEM=OpenMP")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(TBB
  ${TBB_ERROR_MESSAGE}
  TBB_INCLUDE_DIR TBB_LIBRARY TBB_LIBRARY_MALLOC
)

## Check version ##

if (TBB_INCLUDE_DIR)
  file(READ ${TBB_INCLUDE_DIR}/tbb/tbb_stddef.h TBB_STDDEF_H)

  string(REGEX MATCH "#define TBB_VERSION_MAJOR ([0-9]+)" DUMMY "${TBB_STDDEF_H}")
  set(TBB_VERSION_MAJOR ${CMAKE_MATCH_1})

  string(REGEX MATCH "#define TBB_VERSION_MINOR ([0-9]+)" DUMMY "${TBB_STDDEF_H}")
  set(TBB_VERSION "${TBB_VERSION_MAJOR}.${CMAKE_MATCH_1}")

  if (TBB_VERSION VERSION_LESS TBB_VERSION_REQUIRED)
    message(FATAL_ERROR ${TBB_ERROR_MESSAGE})
  endif()

  set(TBB_VERSION ${TBB_VERSION} CACHE STRING "TBB Version")
  mark_as_advanced(TBB_VERSION)
endif()

## Build TBB target 'ospcommon_tbb' ##

if (TBB_FOUND AND NOT TARGET ospcommon_tbb)
  add_library(ospcommon_tbb INTERFACE IMPORTED)
  set_target_properties(ospcommon_tbb PROPERTIES
    INTERFACE_LINK_LIBRARIES "${TBB_LIBRARY};${TBB_LIBRARY_MALLOC}"
    INTERFACE_INCLUDE_DIRECTORIES "${TBB_INCLUDE_DIR}"
  )
endif()

mark_as_advanced(TBB_INCLUDE_DIR)
mark_as_advanced(TBB_LIBRARY)
mark_as_advanced(TBB_LIBRARY_DEBUG)
mark_as_advanced(TBB_LIBRARY_MALLOC)
mark_as_advanced(TBB_LIBRARY_MALLOC_DEBUG)
