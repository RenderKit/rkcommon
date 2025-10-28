# rkcommon - C++/CMake infrastructure

This project represents a common set of C++ infrastructure and CMake utilities
used by various components of Intel® Rendering Toolkit (Render Kit).

### Requirements

- CMake v3.10 or higher
- C++11 compiler
- TBB 4.4.3 or higher (by default, other tasking system options available via
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
