# Software Technology Lab (STLab) Library Source Code Repository

ASL libraries will be migrated here in the `stlab` namespace, new libraries will be created here.

## Branch states

- **`main`:** [![Build and Tests](https://github.com/stlab/libraries/actions/workflows/stlab.yml/badge.svg)](https://github.com/stlab/libraries/actions/workflows/stlab.yml)

## Content

### [Concurrency](https://www.stlab.cc/libraries/concurrency/)

This library provides futures and channels, high-level abstractions for implementing algorithms that ease the use of multiple CPU cores while minimizing contention. This library solves several problems of the C++11 and C++17 TS futures.

## Documentation

The complete documentation is available on the [STLab home page](http://stlab.cc).

Release changelogs are listed in [CHANGES.md](CHANGES.md).

## Tested on

- Linux with GCC 11
- Linux with Clang 14
- MacOS 11 with Apple-clang 13.0.0
- Windows with Visual Studio 16

## Requirements

- A standards-compliant C++17, C++20, or C++23 compiler
- **Building** the library requires CMake 3.23 or later
- **Testing or developing** the library requires Boost.Test >= 1.74.0

## Building

STLab is a standard CMake project. See the [running CMake](https://cmake.org/runningcmake) tutorial
for an introduction to this tool.

### Preparation

1. Create a build directory outside this library's source tree. In this guide, we'll use a sibling
   directory called `BUILD`.

1. Install a version of CMake >= 3.23. If you are on Debian or Ubuntu Linux you may need to use
   `snap` to find one that's new enough.

1. If you are using MSVC, you may need to set environment variables appropriately for your target
   architecture by invoking `VCVARSALL.BAT` with an appropriate option.

### Configure

Run CMake in the root directory of this project, setting `./build` as your build directory. The
basis of your command will be

```
cmake -S . -B ../BUILD -DCMAKE_BUILD_TYPE=# SEE BELOW
```

but there are other options you may need to append in order to be successful. Among them:

- `-DCMAKE_BUILD_TYPE=`[**`Release`**|`Debug`] to build the given configuration (required unless you're using Visual Studio or another multi-config generator).
- `-DCMAKE_CXX_STANDARD=`[`17`|**`20`**|`23`] to build with compliance to the given C++ standard.
- `-DBUILD_TESTING=`[`ON`, `OFF`] turn off if you intend to build, but not test, this library.

STlab specific configuration options:

- `-DSTLAB_MAIN_EXECUTOR=`[`qt5`, `qt6`, `libdispatch`, `emscripten`, `none`] to select the main executor to use. Default is platform dependent.
- `-DSTLAB_MINIMAL_TASK_POOL=`[`ON`, **`OFF`**] to use the minimal task pool size, implies `STLAB_TASK_SYSTEM=portable`.
- `-DSTLAB_NO_STD_COROUTINES=`[`ON`, **`OFF`**] to suppress usage of standard coroutines. Useful for non-conforming compilers.
- `-DSTLAB_THREAD_SYSTEM=`[`win32`, `pthread`, `pthread-emscripten`, `pthread-apple`, `none`] to select the thread system to use. Default is platform dependent.
- `-DSTLAB_TASK_SYSTEM=`[`portable`, `libdispatch`, `windows`] to select the task system to use. Default is platform dependent.

We also suggest the installation of [Ninja](https://ninja-build.org/) and its use by adding
`-GNinja` to your cmake command line… but ninja is not required.

A typical invocation might look like this:

```
cmake -S . -B ../BUILD -GNinja -DCMAKE_CXX_STANDARD=17 -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
```

If you organize the build directory into subdirectories you can support multiple configurations.

```
rm -rf ../builds/portable
cmake -S . -B ../builds/portable -GXcode -DCMAKE_CXX_STANDARD=17 -DBUILD_TESTING=ON -DSTLAB_TASK_SYSTEM=portable -DCMAKE_OSX_DEPLOYMENT_TARGET=macosx14.4
```

### Build

If your configuration command was successful, go to your build directory (`cd ../BUILD`) and invoke:

```
cmake --build .
```

## Testing

Running the tests in the `BUILD` directory is as simple as invoking

```
ctest -C Debug
```

or

```
ctest -C Release
```

depending on which configuration (`CMAKE_BUILD_TYPE) you choose to build.

## Generating Documentation

For generating the documentation, see the [README.md](docs/README.md) in the `docs` directory.
