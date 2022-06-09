#!/bin/bash
set -x

if [ -z "${coverage+xxx}" ]; then export coverage=FALSE; fi

mkdir build
cd build
conan install ./.. -o testing=True --build=missing

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

export TESTS_ARGUMENTS=--log_level=message 
ctest -C Release -j$NPROC --verbose --no-compress-output
if [ $? -ne 0 ]; then exit 1; fi

if $coverage; then
  lcov -c -d . -b .. -o Coverage.out
  lcov -a Coverage.baseline -a Coverage.out -o Coverage.lcov
fi;
