#!/bin/bash -x
## Copyright 2020 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

mkdir build
cd build

cmake --version

cmake -DRKCOMMON_TASKING_SYSTEM=INTERNAL ..

cmake --build .
