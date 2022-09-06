# `stlab` Documentation

Pull requests for typos, examples, and other improvements are welcome. To file an issue, please use the [libraries repository](https://github.com/stlab/libraries).

# Branch states

\[ These are from the old site - FIX ME \]

- **`master`:** [![master build](https://travis-ci.org/stlab/stlab.github.io.svg?branch=master)](https://travis-ci.org/stlab/stlab.github.io) [![master coverage](https://codecov.io/github/stlab/stlab.github.io/coverage.svg?branch=master)](https://codecov.io/gh/stlab/stlab.github.io/branch/master)

- **`develop`:** [![develop build](https://travis-ci.org/stlab/stlab.github.io.svg?branch=develop)](https://travis-ci.org/stlab/stlab.github.io)
[![develop coverage](https://codecov.io/github/stlab/stlab.github.io/coverage.svg?branch=develop)](https://codecov.io/gh/stlab/stlab.github.io/branch/develop)

# Building (Mac)

You'll need Homebrew to make sure you have the most recent version of Ruby.

## Setup
1. Clone the repo
2. `brew install ruby`
3. `sudo gem install bundle jekyll github-pages liquid`


Periodically run `gem update` and `bundle update` to make sure you have the latest jekyll tooling.

## Building Docs
```
cd ./docs
bundle exec jekyll serve --watch
```

Documentation viewable at `localhost:4000`
Modifying sources should auto-regenerate the documentation

## Building Examples
1. `./build.sh` will download dependencies, build, and run all the `*.cpp` files in the `libraries` directory.

## Running Hyde in Docker

The following directory structure is assumed.

[ The longer term plan is to migrate the docs for the libraries into the library repo. We also need a plan for the structure of the build directory, a]

```
.             # This directory stlab.github.io
../libraries  # The stlab/libraries repo
../builds/hyde      # The cmake build directory configures for building docs
```

Configure the build as follows:

```
cd ../builds/hyde
ccmake ../../libraries
```

Specify the following options:

```
 BUILD_TESTING                    OFF
 CMAKE_BUILD_TYPE
 CMAKE_INSTALL_PREFIX             /usr/local
 CMAKE_OSX_ARCHITECTURES
 CMAKE_OSX_DEPLOYMENT_TARGET
 CMAKE_OSX_SYSROOT                /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/
 Qt6_DIR                          Qt6_DIR-NOTFOUND
 STLAB_MAIN_EXECUTOR              none
 STLAB_NO_STD_COROUTINES          ON
 STLAB_TASK_SYSTEM                portable
 STLAB_THREAD_SYSTEM              pthread
 STLAB_USE_BOOST_CPP17_SHIMS      OFF
 stlab.coverage                   OFF
```

- Build the docker image per the instructions in the hyde repo, [current using the clang13 branch](https://github.com/adobe/hyde/tree/fosterbrereton/llvm13-updates).

```
cd ../..
docker run --platform linux/x86_64 --mount type=bind,source="$(pwd)/..",target=/mnt/host \
    --tty --interactive \
    hyde bash
```

From the docker prompt
```
cd /mnt/host/docs
./generate_docs.sh
```
