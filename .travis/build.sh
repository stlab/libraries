#!/bin/bash
set -x

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
  sudo update-alternatives \
    --install /usr/bin/gcc gcc /usr/bin/gcc-5 90 \
    --slave /usr/bin/g++ g++ /usr/bin/g++-5 \
    --slave /usr/bin/gcov gcov /usr/bin/gcov-5
  sudo update-alternatives \
    --install /usr/bin/clang clang /usr/bin/clang-3.8 90 \
    --slave /usr/bin/clang++ clang++ /usr/bin/clang++-3.8
  sudo update-alternatives --config gcc
  sudo update-alternatives --config clang
  if [ "$CXX" = "clang++" ]; then
      export PATH=/usr/bin:$PATH
  fi
fi

if [ -z "${coverage+xxx}" ]; then export coverage=FALSE; fi

mkdir build
conan install ./.. --build=missing

NPROC=`sysctl -n hw.ncpu`
if [ "$NPROC" == "" ] ; then
  NPROC=`nproc`
fi

cmake -D CMAKE_BUILD_TYPE=$build_type -D stlab_appended_flags=$flags $options ..
if [ $? -ne 0 ]; then exit 1; fi

make -j$NPROC
if [ $? -ne 0 ]; then exit 1; fi

if $coverage; then lcov -c -i -b .. -d . -o Coverage.baseline; fi

ctest --output-on-failure -j$NPROC
if [ $? -ne 0 ]; then exit 1; fi

if $coverage; then
  lcov -c -d . -b .. -o Coverage.out
  lcov -a Coverage.baseline -a Coverage.out -o Coverage.lcov
fi;
