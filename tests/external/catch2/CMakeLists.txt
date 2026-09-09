## Copyright 2026 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.16) # Catch2, which also raises the tests to C++14

include(FetchContent)
set(FETCHCONTENT_QUIET ON)
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2
  GIT_TAG ${RKCOMMON_CATCH2_GIT_TAG}
)

# rkcommon_test_suite is shipped, thus never add a Catch2 runtime dependency
set(BUILD_SHARED_LIBS OFF)

FetchContent_MakeAvailable(Catch2)

mark_as_advanced(FETCHCONTENT_BASE_DIR)
mark_as_advanced(FETCHCONTENT_FULLY_DISCONNECTED)
mark_as_advanced(FETCHCONTENT_QUIET)
mark_as_advanced(FETCHCONTENT_SOURCE_DIR_CATCH2)
mark_as_advanced(FETCHCONTENT_UPDATES_DISCONNECTED)
mark_as_advanced(FETCHCONTENT_UPDATES_DISCONNECTED_CATCH2)
