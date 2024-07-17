## Copyright 2020 Intel Corporation
## SPDX-License-Identifier: Apache-2.0

echo "Running tests"

$env:Path += ";.\build\Release"

.\build\Release\rkcommon_test_suite.exe

exit $LASTEXITCODE
