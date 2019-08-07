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

## Macro for printing CMake variables ##
macro(print var)
  message("${var} = ${${var}}")
endmacro()

## Macro to print a warning message that only appears once ##
macro(ospcommon_warn_once IDENTIFIER MESSAGE)
  set(INTERNAL_WARNING "OSPCOMMON_WARNED_${IDENTIFIER}")
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
macro(ospcommon_configure_build_type)
  set(CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo")
  if (WIN32)
    if (NOT OSPCOMMON_DEFAULT_CMAKE_CONFIGURATION_TYPES_SET)
      set(CMAKE_CONFIGURATION_TYPES "${CONFIGURATION_TYPES}"
          CACHE STRING "List of generated configurations." FORCE)
      set(OSPCOMMON_DEFAULT_CMAKE_CONFIGURATION_TYPES_SET ON
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

macro(ospcommon_configure_compiler)
  if (WIN32)
    set(OSPCOMMON_PLATFORM_WIN  1)
    set(OSPCOMMON_PLATFORM_UNIX 0)
  else()
    set(OSPCOMMON_PLATFORM_WIN  0)
    set(OSPCOMMON_PLATFORM_UNIX 1)
  endif()

  # unhide compiler to make it easier for users to see what they are using
  mark_as_advanced(CLEAR CMAKE_CXX_COMPILER)

  option(OSPCOMMON_STRICT_BUILD "Build with additional warning flags" ON)
  mark_as_advanced(OSPCOMMON_STRICT_BUILD)

  option(OSPCOMMON_WARN_AS_ERRORS "Treat warnings as errors" OFF)
  mark_as_advanced(OSPCOMMON_WARN_AS_ERRORS)

  set(OSPCOMMON_COMPILER_ICC   FALSE)
  set(OSPCOMMON_COMPILER_GCC   FALSE)
  set(OSPCOMMON_COMPILER_CLANG FALSE)
  set(OSPCOMMON_COMPILER_MSVC  FALSE)

  if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
    set(OSPCOMMON_COMPILER_ICC TRUE)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(OSPCOMMON_COMPILER_GCC TRUE)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
          "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
    set(OSPCOMMON_COMPILER_CLANG TRUE)
  elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(OSPCOMMON_COMPILER_MSVC TRUE)
  else()
    message(FATAL_ERROR
            "Unsupported compiler specified: '${CMAKE_CXX_COMPILER_ID}'")
  endif()
endmacro()

## Tasking System macros ##

macro(ospcommon_configure_tasking_system)
  set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
  set(THREADS_PREFER_PTHREAD_FLAG TRUE)
  find_package(Threads REQUIRED)

  set(OSPCOMMON_TASKING_SYSTEM TBB CACHE STRING
      "Per-node thread tasking system [TBB,OpenMP,LibDispatch,Internal,Debug]")

  set_property(CACHE OSPCOMMON_TASKING_SYSTEM PROPERTY
               STRINGS TBB OpenMP Internal LibDispatch Debug)

  # NOTE(jda) - Make the OSPCOMMON_TASKING_SYSTEM build option case-insensitive
  string(TOUPPER ${OSPCOMMON_TASKING_SYSTEM} OSPCOMMON_TASKING_SYSTEM_ID)

  set(OSPCOMMON_TASKING_TBB      FALSE)
  set(OSPCOMMON_TASKING_OPENMP   FALSE)
  set(OSPCOMMON_TASKING_INTERNAL FALSE)
  set(OSPCOMMON_TASKING_DEBUG    FALSE)

  if(${OSPCOMMON_TASKING_SYSTEM_ID} STREQUAL "TBB")
    set(OSPCOMMON_TASKING_TBB TRUE)
    find_package(TBB REQUIRED)
  else()
    unset(TBB_INCLUDE_DIR          CACHE)
    unset(TBB_LIBRARY              CACHE)
    unset(TBB_LIBRARY_DEBUG        CACHE)
    unset(TBB_LIBRARY_MALLOC       CACHE)
    unset(TBB_LIBRARY_MALLOC_DEBUG CACHE)
    if(${OSPCOMMON_TASKING_SYSTEM_ID} STREQUAL "OPENMP")
      set(OSPCOMMON_TASKING_OPENMP TRUE)
      find_package(OpenMP)
    elseif(${OSPCOMMON_TASKING_SYSTEM_ID} STREQUAL "INTERNAL")
      set(OSPCOMMON_TASKING_INTERNAL TRUE)
    else()
      set(OSPCOMMON_TASKING_DEBUG TRUE)
    endif()
  endif()

  set(OSPCOMMON_TASKING_LIBS ${CMAKE_THREAD_LIBS_INIT})

  if(OSPCOMMON_TASKING_TBB)
    set(OSPCOMMON_TASKING_DEFINITIONS -DOSPCOMMON_TASKING_TBB)
    set(OSPCOMMON_TASKING_INCLUDES ${TBB_INCLUDE_DIRS})
    set(OSPCOMMON_TASKING_LIBS ${OSPCOMMON_TASKING_LIBS} ${TBB_LIBRARIES})
  elseif(OSPCOMMON_TASKING_OPENMP)
    if (OPENMP_FOUND)
      set(OSPCOMMON_TASKING_OPTIONS "${OpenMP_CXX_FLAGS}")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
      if(OSPCOMMON_COMPILER_ICC) # workaround linker issue #115
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -liomp5")
      endif()
      set(OSPCOMMON_TASKING_DEFINITIONS -DOSPCOMMON_TASKING_OMP)
    endif()
  elseif(OSPCOMMON_TASKING_INTERNAL)
    set(OSPCOMMON_TASKING_DEFINITIONS -DOSPCOMMON_TASKING_INTERNAL)
  else()#Debug
    # Do nothing, will fall back to scalar code (useful for debugging)
  endif()
endmacro()

macro(ospcommon_create_tasking_target)
  if (NOT TARGET ospcommon_tasking)
    add_library(ospcommon_tasking INTERFACE)

    target_include_directories(ospcommon_tasking
    INTERFACE
      ${OSPCOMMON_TASKING_INCLUDES}
    )

    target_link_libraries(ospcommon_tasking
    INTERFACE
      ${OSPCOMMON_TASKING_LIBS}
    )

    target_compile_definitions(ospcommon_tasking
    INTERFACE
      ${OSPCOMMON_TASKING_DEFINITIONS}
    )

    target_compile_options(ospcommon_tasking
    INTERFACE
      ${OSPCOMMON_TASKING_OPTIONS}
    )
  endif()
endmacro()
