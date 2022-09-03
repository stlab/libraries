# Software Technology Lab (STLab) Library Source Code Repository

ASL libraries will be migrated here in the `stlab` namespace, new libraries will be created here.

## Branch states

- **`main`:**
[![Main status](https://github.com/stlab/libraries/actions/workflows/conan.yml/badge.svg)](https://github.com/stlab/libraries/actions/workflows/conan.yml/badge.svg)
[![AppVeyor](https://ci.appveyor.com/api/projects/status/aaf2uibkql1625dl/branch/main?svg=true)](https://ci.appveyor.com/project/fosterbrereton/libraries/branch/main)
[![Code Coverage](https://codecov.io/github/stlab/libraries/coverage.svg?branch=main)](https://codecov.io/gh/stlab/libraries/branch/main)
[![Coverty Scan](https://scan.coverity.com/projects/13163/badge.svg)](https://scan.coverity.com/projects/stlab_libraries)

## Content

### [Concurrency](https://www.stlab.cc/libraries/concurrency/)
This library provides futures and channels, high level abstractions for implementing algorithms that eases the use of multiple CPU cores while minimizing contention. This library solves several problems of the C++11 and C++17 TS futures.

## Documentation

The complete documentation is available on the [STLab home page](http://stlab.cc).

Release changelogs are listed in [CHANGES.md](CHANGES.md).

## Tested on

- Linux with GCC 11
- Linux with Clang 14
- MacOS 11 with Apple-clang 13.0.0
- Windows with Visual Studio 16

## Requirements

- A standards-compliant C++14, C++17, or C++20 compiler
- **Use with a compiler in C++14-compliant mode** requires Boost.Optional and Boost.Variant >= 1.74.0
- **Building** the library requires CMake 3.23 or later
- **Testing or developing** the library requires Boost.Test >= 1.74.0

## Building

STLab is a standard CMake project. See the [running CMake](https://cmake.org/runningcmake) tutorial
for an introduction to this tool.

### Preparation

1. Create a build directory outside this library's source tree.  In this guide, we'll use a sibling
   directory called `BUILD`.

2. If you are using the library in C++14-compliant mode or need to run the test suite, be sure you
   have the necessary parts of Boost >= 1.74.0 installed.  Linux distributions usually make a
   suitable version available through standard package managers.  On macOS or Linux, you can easilly
   install Boost using [Homebrew](https://brew.sh/).  To install Boost on Windows, you can use
   Boost's [binary installers](https://sourceforge.net/projects/boost/files/boost-binaries/).

3. Install a version of CMake >= 3.23.  If you are on Debian or Ubuntu Linux you may need to use
   `snap` to find one that's new enough.

4. If you are using MSVC, you may need to set environment variables appropriately for your target
   architecture by invoking `VCVARSALL.BAT` with an appropriate option.

### Configure

Run CMake in the root directory of this project, setting `../BUILD` as your build directory.  The
basis of your command will be

```
cmake -S . -B ../BUILD -DCMAKE_BUILD_TYPE=# SEE BELOW
```

but there are other options you may need to append in order to be successful.  Among them:

* `-DCMAKE_BUILD_TYPE=`[`Release`|`Debug`] to build the given configuration (required unless you're using visual studio or another multi-config generator).
* `-DCMAKE_CXX_STANDARD=`[`14`|`17`|`20`|`23`] to build with compliance to the given C++ standard.
* `-DBUILD_TESTING=OFF` if you only intend to build, but not test, this library.
* `-DBoost_USE_STATIC_LIBS=TRUE` if you will be testing on Windows.

We also suggest the installation of [Ninja](https://ninja-build.org/) and its use by adding
`-GNinja` to your cmake command line… but ninja is not required.

A typical invocation might look like this:

```
cmake -S . -B ../BUILD -GNinja -DCMAKE_CXX_STANDARD=17 -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
```

If you organize the build directory into subdirectories you can support multiple configurations.

```
cmake -S . -B ../builds/portable -GXcode -DCMAKE_CXX_STANDARD=17 -DBUILD_TESTING=ON -DSTLAB_TASK_SYSTEM=portable
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

depending on which configuration (`CMAKE_BUILD_TYPE`) you chose to build.

## Generating Documentation

STLab uses [hyde](https://github.com/adobe/hyde) to generate documentation boilerplate by scanning its public headers. You will need to follow Hyde's installation instructions and have `hyde` on your `$PATH` (detectable by `which hyde`).

When adding a new public API, you should invoke

```
docker run --platform linux/x86_64 --mount type=bind,source="$(pwd)/..",target=/mnt/host \
    --tty --interactive \
    hyde bash
```

```
cd /mnt/host/libraries
./validate_docs.sh --update
```

(or, simply `-u`) to generate the boilerplate for it. Then, fill in any fields marked as `__MISSING__`. Fields marked as `__OPTIONAL__` may be omitted.
