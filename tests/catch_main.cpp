// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "rkcommon/tasking/tasking_system_init.h"

int main(int argc, char* argv[]) {

  // Spawn workers threads
  rkcommon::tasking::initTaskingSystem();

  // Run all tests
  auto result = Catch::Session().run(argc, argv);

  // Terminate worker threads
  rkcommon::tasking::shutdownTaskingSystem();
  return result;
}
