## Copyright 2009 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

## Global CMake options ##

if (RKCOMMON_TASKING_SYSTEM STREQUAL "OpenMP")
  cmake_minimum_required(VERSION 3.9) # NOTE(jda): rely on OpenMP targets
else()
  cmake_minimum_required(VERSION 3.5)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

## Establish project ##

project(rkcommon VERSION 1.14.2 LANGUAGES CXX)

include(GNUInstallDirs)

configure_file(
  ${PROJECT_SOURCE_DIR}/rkcommon/version.h.in
  ${PROJECT_BINARY_DIR}/rkcommon/version.h
  @ONLY
)
set(RKCOMMON_RESOURCE ${PROJECT_SOURCE_DIR}/rkcommon/rkcommon.rc)

## Add rkcommon specific macros ##

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

include(rkcommon_macros)

rkcommon_configure_build_type()
rkcommon_configure_compiler()
rkcommon_configure_tasking_system()
rkcommon_create_tasking_target(FALSE)

## Build options and specific configuration ##

option(BUILD_SHARED_LIBS "Build rkcommon as a shared library" ON)
option(RKCOMMON_ADDRSAN "Build rkcommon with dlclose disabled for addrsan" OFF)
option(RKCOMMON_NO_SIMD "Build rkcommon not using SIMD instructions" OFF)
option(RKCOMMON_WARN_AS_ERRORS "Treat warnings as errors" OFF)

set(CMAKE_SKIP_INSTALL_RPATH OFF)
if (APPLE)
  set(CMAKE_MACOSX_RPATH ON)
  set(CMAKE_INSTALL_RPATH "@loader_path/")
else()
  set(CMAKE_INSTALL_RPATH "\$ORIGIN")
endif()

include(CTest)

if (BUILD_TESTING)
  enable_testing()
endif()

if (WIN32)
  option(INSTALL_DEPS "Install rkcommon DLL dependencies" ON)
else()
  set(INSTALL_DEPS OFF)
endif()

if (INSTALL_DEPS)
  include(rkcommon_redist_deps)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

## Build library and tests ##

add_subdirectory(rkcommon)

if (BUILD_TESTING)
  add_subdirectory(tests)
endif()

## Configure CMake find_package() config files ##

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
INSTALL_DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/rkcommon-${PROJECT_VERSION}
)

write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  cmake/FindTBB.cmake
  cmake/rkcommon_macros.cmake
DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/rkcommon-${PROJECT_VERSION}
)

# Must be last
include(CPack)
