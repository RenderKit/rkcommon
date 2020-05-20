## Copyright 2009-2019 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

include(CMakeFindDependencyMacro)

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

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    set(RKCOMMON_COMPILER_ICC TRUE)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(RKCOMMON_COMPILER_GCC TRUE)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
          "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set(RKCOMMON_COMPILER_CLANG TRUE)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(RKCOMMON_COMPILER_MSVC TRUE)
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
    if (FROM_INSTALL)
      find_dependency(TBB)
    else()
      if(POLICY CMP0074)
        # Our FindTBB script uses TBB_ROOT, which is the NEW behaviour for
        # CMP0074.
        cmake_policy(SET CMP0074 NEW)
      endif()
      if (NOT RKCOMMON_TBB_ROOT STREQUAL "")
        set(TBB_ROOT ${RKCOMMON_TBB_ROOT})
      endif()
      find_package(TBB 4.4 REQUIRED tbb tbbmalloc)
    endif()

    if (TBB_FOUND)
      list(APPEND RKCOMMON_TASKING_LIBS TBB)
      set(RKCOMMON_TASKING_DEFINITIONS RKCOMMON_TASKING_TBB)
    endif()
  elseif(RKCOMMON_TASKING_OPENMP)
    find_dependency(OpenMP)
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
