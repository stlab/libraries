#!/bin/bash

cd `dirname $0`

if [ ! -d build ]; then
    mkdir build
fi

cd build

if [ ! $CC ]; then
    export CC=clang++
fi
 
$CC --version

if [ ! -d stlab ]; then
    git clone --depth=1 https://github.com/stlab/libraries.git stlab
fi

if [ ! -d boost ]; then
    if [ ! -f boost.tgz ]; then
        curl -L https://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.gz -o boost.tgz
    fi

    mkdir boost

    tar -C boost -xzf boost.tgz --strip-components 1 boost_1_60_0/boost/
    tar -C boost -xzf boost.tgz --strip-components 1 boost_1_60_0/libs/
fi

find ../libraries -name "*.cpp" | while read -r src
do
  echo $src

  dst=`basename $src`.exe

  $CC -x c++ -std=c++14 -stdlib=libc++ $src -I./stlab -I./boost -o $dst

  if [ $? != 0 ]; then
    exit $?;
  fi

  ./$dst > /dev/null

  if [ $? != 0 ]; then
    exit $?;
  fi
done
