# rkcommon - C++/CMake infrastructure

This project represents a common set of C++ infrastructure and CMake utilities
used by various components of Intel® oneAPI Rendering Toolkit.

### Requirements

- CMake
- C++11 compiler
- TBB (by default, other tasking system options available via
  the `RKCOMMON_TASKING_SYSTEM` CMake variable)

### Building

Build with:

```bash
git clone https://github.com/ospray/rkcommon.git
cd rkcommon
mkdir build
cd build
cmake ..
cmake --build .
```

Run tests from the build directory with:

```bash
ctest .
```
