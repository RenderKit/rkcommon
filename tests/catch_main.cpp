// Copyright 2009 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "rkcommon/tasking/tasking_system_init.h"

int main(int argc, char* argv[]) {
  rkcommon::tasking::initTaskingSystem();
  return Catch::Session().run(argc, argv);
}
