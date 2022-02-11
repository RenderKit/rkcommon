## Copyright 2009-2021 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

# use a backported version of find_dependency(), renamed as
# find_dependency_39(), from CMake 3.9.0, which correctly supports passing
# components to find_package(). this allows us to maintain our current minimum
# CMake version of 3.1.
macro(find_dependency_39 dep)
  if (NOT ${dep}_FOUND)
    set(cmake_fd_quiet_arg)
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
      set(cmake_fd_quiet_arg QUIET)
    endif()
    set(cmake_fd_required_arg)
    if(${CMAKE_FIND_PACKAGE_NAME}_FIND_REQUIRED)
      set(cmake_fd_required_arg REQUIRED)
    endif()

    get_property(cmake_fd_alreadyTransitive GLOBAL PROPERTY
      _CMAKE_${dep}_TRANSITIVE_DEPENDENCY
    )

    find_package(${dep} ${ARGN}
      ${cmake_fd_quiet_arg}
      ${cmake_fd_required_arg}
    )

    if(NOT DEFINED cmake_fd_alreadyTransitive OR cmake_fd_alreadyTransitive)
      set_property(GLOBAL PROPERTY _CMAKE_${dep}_TRANSITIVE_DEPENDENCY TRUE)
    endif()

    if (NOT ${dep}_FOUND)
      set(${CMAKE_FIND_PACKAGE_NAME}_NOT_FOUND_MESSAGE "${CMAKE_FIND_PACKAGE_NAME} could not be found because dependency ${dep} could not be found.")
      set(${CMAKE_FIND_PACKAGE_NAME}_FOUND False)
      return()
    endif()
    set(cmake_fd_required_arg)
    set(cmake_fd_quiet_arg)
    set(cmake_fd_exact_arg)
  endif()
endmacro()

## Macro for printing CMake variables ##
macro(print var)
  message("${var} = ${${var}}")
endmacro()

## Macro to print a warning message that only appears once ##
macro(rkcommon_warn_once IDENTIFIER MESSAGE)
  set(INTERNAL_WARNING "RKCOMMON_WARNED_${IDENTIFIER}")
  if(NOT ${INTERNAL_WARNING})
    message(WARNING ${MESSAGE})
    set(${INTERNAL_WARNING} ON CACHE INTERNAL "Warned about '${MESSAGE}'")
  endif()
endmacro()

## Get a list of subdirectories (single level) under a given directory
macro(get_subdirectories result curdir)
  file(GLOB children RELATIVE ${curdir} ${curdir}/*)
  set(dirlist "")
  foreach(child ${children})
    if(IS_DIRECTORY ${curdir}/${child})
      list(APPEND dirlist ${child})
    endif()
  endforeach()
  set(${result} ${dirlist})
endmacro()

## Setup CMAKE_BUILD_TYPE to have a default + cycle between options in UI
macro(rkcommon_configure_build_type)
  set(CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo")
  if (WIN32)
    if (NOT RKCOMMON_DEFAULT_CMAKE_CONFIGURATION_TYPES_SET)
      set(CMAKE_CONFIGURATION_TYPES "${CONFIGURATION_TYPES}"
          CACHE STRING "List of generated configurations." FORCE)
      set(RKCOMMON_DEFAULT_CMAKE_CONFIGURATION_TYPES_SET ON
          CACHE INTERNAL "Default CMake configuration types set.")
    endif()
  else()
    if(NOT CMAKE_BUILD_TYPE)
      set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type." FORCE)
    endif()
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CONFIGURATION_TYPES})
  endif()
endmacro()

## Compiler configuration macros ##

macro(rkcommon_configure_compiler)
  if (WIN32)
    set(RKCOMMON_PLATFORM_WIN  1)
    set(RKCOMMON_PLATFORM_UNIX 0)
  else()
    set(RKCOMMON_PLATFORM_WIN  0)
    set(RKCOMMON_PLATFORM_UNIX 1)
  endif()

  # unhide compiler to make it easier for users to see what they are using
  mark_as_advanced(CLEAR CMAKE_CXX_COMPILER)

  option(RKCOMMON_STRICT_BUILD "Build with additional warning flags" ON)
  mark_as_advanced(RKCOMMON_STRICT_BUILD)

  option(RKCOMMON_WARN_AS_ERRORS "Treat warnings as errors" OFF)
  mark_as_advanced(RKCOMMON_WARN_AS_ERRORS)

  set(RKCOMMON_COMPILER_ICC   FALSE)
  set(RKCOMMON_COMPILER_GCC   FALSE)
  set(RKCOMMON_COMPILER_CLANG FALSE)
  set(RKCOMMON_COMPILER_MSVC  FALSE)
  set(RKCOMMON_COMPILER_DPCPP FALSE)

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    set(RKCOMMON_COMPILER_ICC TRUE)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(RKCOMMON_COMPILER_GCC TRUE)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
          "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set(RKCOMMON_COMPILER_CLANG TRUE)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(RKCOMMON_COMPILER_MSVC TRUE)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "IntelLLVM")
    set(RKCOMMON_COMPILER_DPCPP TRUE)
  else()
    message(FATAL_ERROR
            "Unsupported compiler specified: '${CMAKE_CXX_COMPILER_ID}'")
  endif()
endmacro()

## Tasking System macros ##

macro(rkcommon_configure_tasking_system)
  set(RKCOMMON_TASKING_SYSTEM TBB CACHE STRING
      "Per-node thread tasking system [TBB,OpenMP,Internal,Debug]")

  set_property(CACHE RKCOMMON_TASKING_SYSTEM PROPERTY
               STRINGS TBB OpenMP Internal Debug)

  # NOTE(jda) - Make the RKCOMMON_TASKING_SYSTEM build option case-insensitive
  string(TOUPPER ${RKCOMMON_TASKING_SYSTEM} RKCOMMON_TASKING_SYSTEM_ID)

  set(RKCOMMON_TASKING_TBB      FALSE)
  set(RKCOMMON_TASKING_OPENMP   FALSE)
  set(RKCOMMON_TASKING_INTERNAL FALSE)
  set(RKCOMMON_TASKING_DEBUG    FALSE)

  if(${RKCOMMON_TASKING_SYSTEM_ID} STREQUAL "TBB")
    set(RKCOMMON_TASKING_TBB TRUE)
  else()
    unset(TBB_INCLUDE_DIR          CACHE)
    unset(TBB_LIBRARY              CACHE)
    unset(TBB_LIBRARY_DEBUG        CACHE)
    unset(TBB_LIBRARY_MALLOC       CACHE)
    unset(TBB_LIBRARY_MALLOC_DEBUG CACHE)
    if(${RKCOMMON_TASKING_SYSTEM_ID} STREQUAL "OPENMP")
      set(RKCOMMON_TASKING_OPENMP TRUE)
    elseif(${RKCOMMON_TASKING_SYSTEM_ID} STREQUAL "INTERNAL")
      set(RKCOMMON_TASKING_INTERNAL TRUE)
    else()
      set(RKCOMMON_TASKING_DEBUG TRUE)
    endif()
  endif()
endmacro()

macro(rkcommon_create_tasking_target FROM_INSTALL)
  set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
  set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  find_package(Threads REQUIRED)

  set(RKCOMMON_TASKING_LIBS ${CMAKE_THREAD_LIBS_INIT})

  if(RKCOMMON_TASKING_TBB)
    if(POLICY CMP0074)
      # Our FindTBB script uses TBB_ROOT, which is the NEW behaviour for
      # CMP0074.
      cmake_policy(SET CMP0074 NEW)
    endif()
    if (DEFINED RKCOMMON_TBB_ROOT AND NOT RKCOMMON_TBB_ROOT STREQUAL "")
      set(TBB_FIND_PACKAGE_OPTION "ONLY_CMAKE_FIND_ROOT_PATH")
      set(CMAKE_FIND_ROOT_PATH ${RKCOMMON_TBB_ROOT})
      set(TBB_ROOT ${RKCOMMON_TBB_ROOT})
      list(APPEND CMAKE_PREFIX_PATH ${RKCOMMON_TBB_ROOT})
    endif()

    # Try getting TBB via config first
    find_package(TBB 2021.1 QUIET COMPONENTS tbb tbbmalloc CONFIG ${TBB_FIND_PACKAGE_OPTION})
    if (TBB_FOUND)
      list(APPEND RKCOMMON_TASKING_LIBS TBB::tbb TBB::tbbmalloc)
      set(RKCOMMON_TASKING_DEFINITIONS RKCOMMON_TASKING_TBB)
    else()
      # If not found try getting older TBB via module (FindTBB.cmake)
      unset(TBB_DIR CACHE)
      if (${FROM_INSTALL})
        find_dependency_39(TBB 4.4 REQUIRED tbb tbbmalloc)
      else()
        find_package(TBB 4.4 REQUIRED tbb tbbmalloc)
      endif()
      if (TBB_FOUND)
        list(APPEND RKCOMMON_TASKING_LIBS TBB)
        set(RKCOMMON_TASKING_DEFINITIONS RKCOMMON_TASKING_TBB)
      endif()
    endif()
  elseif(RKCOMMON_TASKING_OPENMP)
    find_dependency_39(OpenMP)
    if (OPENMP_FOUND)
      list(APPEND RKCOMMON_TASKING_LIBS OpenMP::OpenMP_CXX)
      set(RKCOMMON_TASKING_DEFINITIONS RKCOMMON_TASKING_OMP)
    endif()
  elseif(RKCOMMON_TASKING_INTERNAL)
    set(RKCOMMON_TASKING_DEFINITIONS RKCOMMON_TASKING_INTERNAL)
  else()#Debug
    # Do nothing, will fall back to scalar code (useful for debugging)
  endif()

  if (NOT TARGET rkcommon_tasking)
    add_library(rkcommon_tasking INTERFACE IMPORTED)
    set_target_properties(rkcommon_tasking PROPERTIES
      INTERFACE_LINK_LIBRARIES "${RKCOMMON_TASKING_LIBS}"
      INTERFACE_COMPILE_DEFINITIONS "${RKCOMMON_TASKING_DEFINITIONS}"
    )
  endif()
endmacro()
