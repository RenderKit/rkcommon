## Copyright 2009-2020 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

## Global CMake options ##

if (OSPCOMMON_TASKING_SYSTEM STREQUAL "OpenMP")
  cmake_minimum_required(VERSION 3.9) # NOTE(jda): rely on OpenMP targets
else()
  cmake_minimum_required(VERSION 3.1)
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

include(GNUInstallDirs)

## Establish project ##

project(ospcommon VERSION 1.3.0 LANGUAGES CXX)

## Add ospcommon specific macros ##

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

include(ospcommon_macros)

ospcommon_configure_build_type()
ospcommon_configure_compiler()
ospcommon_configure_tasking_system()
ospcommon_create_tasking_target(FALSE)

## Build options and specific configuration ##

option(BUILD_SHARED_LIBS "Build ospcommon as a shared library" ON)

include(CTest)

if (BUILD_TESTING)
  enable_testing()
endif()

if (WIN32)
  option(INSTALL_DEPS "Install ospcommon DLL dependencies" ON)
else()
  set(INSTALL_DEPS OFF)
endif()

if (INSTALL_DEPS)
  include(ospcommon_redist_deps)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

## Build library and tests ##

add_subdirectory(ospcommon)

if (BUILD_TESTING)
  add_subdirectory(tests)
endif()

## Configure CMake find_package() config files ##

include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
  "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
INSTALL_DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/ospcommon-${PROJECT_VERSION}
)

write_basic_package_version_file(
    "${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
  cmake/FindTBB.cmake
  cmake/ospcommon_macros.cmake
DESTINATION
  ${CMAKE_INSTALL_LIBDIR}/cmake/ospcommon-${PROJECT_VERSION}
)

# Must be last
include(CPack)
