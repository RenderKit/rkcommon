## Copyright 2020 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

md build
cd build

cmake --version

cmake -L `
-G $args[0] `
-D RKCOMMON_TASKING_SYSTEM=INTERNAL `
..

cmake --build . --config Release --target ALL_BUILD

exit $LASTEXITCODE
