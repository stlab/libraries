This is the source code repository of the Software Technology Lab (STLab).

ASL libraries will be migrated here in the `stlab` namespace, new libraries will be created here.

# Branch states

- **`main`:**
[![Main status](https://github.com/stlab/libraries/actions/workflows/conan.yml/badge.svg)](https://github.com/stlab/libraries/actions/workflows/conan.yml/badge.svg)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/aaf2uibkql1625dl/branch/main?svg=true)](https://ci.appveyor.com/project/fosterbrereton/libraries/branch/main)
[![Code Coverage](https://codecov.io/github/stlab/libraries/coverage.svg?branch=main)](https://codecov.io/gh/stlab/libraries/branch/main)
[![Coverty Scan](https://scan.coverity.com/projects/13163/badge.svg)](https://scan.coverity.com/projects/stlab_libraries)

# Content

## [Concurrency](https://www.stlab.cc/libraries/concurrency/)
This library provides futures and channels, high level abstractions for implementing algorithms that eases the use of multiple CPU cores while minimizing contention. This library solves several problems of the C++11 and C++17 TS futures.

# Documentation

The complete documentation is available on the [STLab home page](http://stlab.cc).

Release changelogs are listed in [CHANGES.md](CHANGES.md).

# Supported Platforms
- All OS that have a C++14 compliant compiler

# Requirements
- C++14 Compiler
-- Visual Studio 2019
-- gcc (>= 7)
-- clang (>= 6)
- [OPTIONAL] Boost.Optional and Boost.Variant. Required when `STLAB_USE_BOOST_CPP17_SHIMS` is enabled.
- [OPTIONAL] Boost.Test. Used when `BUILD_TESTING` is enabled.

# Building

STLab is a standard CMake project. See the [running CMake](https://cmake.org/runningcmake) tutorial for an introduction to this tool.
