This is the source code repository of the Software Technology Lab (stlab).

ASL libraries will be migrated here in the stlab namespace, new libraries will be created here.

# Branch states

- **`master`:**
[![Master status](https://travis-ci.org/stlab/libraries.svg?branch=master)](https://travis-ci.org/stlab/libraries)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/aaf2uibkql1625dl/branch/master?svg=true)](https://ci.appveyor.com/project/fosterbrereton/libraries/branch/master)
[![Code Coverage](https://codecov.io/github/stlab/libraries/coverage.svg?branch=master)](https://codecov.io/gh/stlab/libraries/branch/master)

- **`develop`:**
[![Travis status](https://travis-ci.org/stlab/libraries.svg?branch=develop)](https://travis-ci.org/stlab/libraries)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/aaf2uibkql1625dl/branch/develop?svg=true)](https://ci.appveyor.com/project/fosterbrereton/libraries/branch/develop)
[![Code Coverage](https://codecov.io/github/stlab/libraries/coverage.svg?branch=develop)](https://codecov.io/gh/stlab/libraries/branch/develop)


# Content

## [Concurrency](http://www.stlab.cc/libraries/concurrency/)
This library provides futures and channels, high level constructs for implementing algorithms that eases the use of 
multiple CPU cores while minimizing contention. This library solves several problems of the C++11 and C++17 TS futures.  

# Documentation

The complete documentation is available on the [stlab home page](http://stlab.cc) 

# Building

First, you will need the following tools:

- [`conan`](https://www.conan.io/) ([download](https://www.conan.io/downloads))
- [`CMake`](https://cmake.org/) ([download](https://cmake.org/download/))
- (Mac) Xcode 8.2.1 or later
- (MSVC) Visual Studio 2015 (14.0) Update 3 or later

`conan` and `cmake` are available on the Mac via [Homebrew](http://brew.sh). `cmake` is available on Windows via [`scoop`](http://scoop.sh/).

Once they're set up, run either `setup_xcode.sh` or `setup_msvc.bat` for the platform of your choice. It will setup all necessary library dependencies and create the platform-specific project file in the `./build/` directory.
