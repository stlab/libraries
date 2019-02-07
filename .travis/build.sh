#!/bin/bash
set -x

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
  #
  # It is nevessary to patch gcc std.variant because of a clang bug which is fixed in clang 8.0
  # further details are here: https://stackoverflow.com/questions/46506387/getstring-for-variants-fail-under-clang-but-not-g
  #
  sudo .travis/patches/patch_std_variant.sh

  sudo update-alternatives \
    --install /usr/bin/gcc gcc /usr/bin/gcc-7 90 \
    --slave /usr/bin/g++ g++ /usr/bin/g++-7 \
    --slave /usr/bin/gcov gcov /usr/bin/gcov-7
  sudo update-alternatives \
    --install /usr/bin/clang clang /usr/bin/clang-7 90 \
    --slave /usr/bin/clang++ clang++ /usr/bin/clang++-7
  sudo update-alternatives --config gcc
  sudo update-alternatives --config clang
  if [ "$CXX" = "clang++" ]; then
      export PATH=/usr/bin:$PATH
  fi
fi

if [ -z "${coverage+xxx}" ]; then export coverage=FALSE; fi

mkdir build
cd build
conan install ./.. --build=missing

NPROC=`sysctl -n hw.ncpu`
if [ "$NPROC" == "" ] ; then
  NPROC=`nproc`
fi

if [ ! -z "$flags" ]; then extra_flags="-D CMAKE_CXX_FLAGS=$flags"; fi
cmake -D CMAKE_BUILD_TYPE=$build_type $options $extra_flags ..
if [ $? -ne 0 ]; then exit 1; fi

make VERBOSE=1 -j$NPROC
if [ $? -ne 0 ]; then exit 1; fi

if $coverage; then lcov -c -i -b .. -d . -o Coverage.baseline; fi

ctest --output-on-failure -j$NPROC
if [ $? -ne 0 ]; then exit 1; fi

if $coverage; then
  lcov -c -d . -b .. -o Coverage.out
  lcov -a Coverage.baseline -a Coverage.out -o Coverage.lcov
fi;
