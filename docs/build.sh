#!/bin/bash
set -x

if [ "$TRAVIS_OS_NAME" = "linux" ]; then
  sudo update-alternatives \
    --install /usr/bin/gcc gcc /usr/bin/gcc-7 90 \
    --slave /usr/bin/g++ g++ /usr/bin/g++-7
  sudo update-alternatives \
    --install /usr/bin/clang clang /usr/bin/clang-7 90 \
    --slave /usr/bin/clang++ clang++ /usr/bin/clan7++-6
  sudo update-alternatives --config gcc
  sudo update-alternatives --config clang
  export NPROC=$(nproc)
else
  export NPROC=$(sysctl -n hw.cpu)
fi

if [ -z "$TRAVIS_BRANCH" ]; then
    export TRAVIS_BRANCH=`git rev-parse --abbrev-ref HEAD`
fi

if [ ! -d stlab ]; then
    echo "Cloning stlab/$TRAVIS_BRANCH..."

    git clone --branch $TRAVIS_BRANCH https://github.com/stlab/libraries.git stlab
    if [ $? -ne 0 ]; then exit 1; fi
else
    echo "Found stlab. Pulling $TRAVIS_BRANCH..."

    cd stlab

    git checkout origin/$TRAVIS_BRANCH
    if [ $? -ne 0 ]; then exit 1; fi

    cd ..
fi

if [ ! -d boost ]; then
    if [ ! -f boost.tgz ]; then
        echo "Downloading boost..."
        curl -L https://dl.bintray.com/boostorg/release/1.64.0/source/boost_1_64_0.tar.gz -o boost.tgz
        if [ $? -ne 0 ]; then exit 1; fi
    fi

    mkdir -p boost

    echo "Unpacking boost headers..."
    tar -C boost -xzf boost.tgz --strip-components 1 boost_1_64_0/boost/
    if [ $? -ne 0 ]; then exit 1; fi

    echo "Unpacking boost sources..."
    tar -C boost -xzf boost.tgz --strip-components 1 boost_1_64_0/libs/
    if [ $? -ne 0 ]; then exit 1; fi
else
    echo "Found boost..."
fi

mkdir -p build
cd build

cmake -D CMAKE_BUILD_TYPE=Debug -D BOOST_ROOT=../boost $@ ..
make -j$NPROC
ctest -j$NPROC
