#!/bin/bash
set -x

cd `dirname $0`

if [ ! -d build ]; then
    mkdir build
fi

cd build

if [ -z "$CC" ]; then
    export CC=clang++
fi

$CC --version

# env | sort

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

    git branch -u origin/$TRAVIS_BRANCH
    if [ $? -ne 0 ]; then exit 1; fi

    git co $TRAVIS_BRANCH
    if [ $? -ne 0 ]; then exit 1; fi

    git pull origin $TRAVIS_BRANCH
    if [ $? -ne 0 ]; then exit 1; fi

    cd ..
fi

if [ ! -d boost ]; then
    if [ ! -f boost.tgz ]; then
        echo "Downloading boost..."
        curl -L https://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.gz -o boost.tgz
        if [ $? -ne 0 ]; then exit 1; fi
    fi

    if [ ! -d boost ]; then
        mkdir boost
    fi

    echo "Unpacking boost headers..."
    tar -C boost -xzf boost.tgz --strip-components 1 boost_1_60_0/boost/
    if [ $? -ne 0 ]; then exit 1; fi

    echo "Unpacking boost sources..."
    tar -C boost -xzf boost.tgz --strip-components 1 boost_1_60_0/libs/
    if [ $? -ne 0 ]; then exit 1; fi
else
    echo "Found boost..."
fi

export CCFLAGS="--coverage -fno-inline -Ofast -Wall -Werror -x c++ -std=c++14"
export CCINCLUDES="-I./stlab -I./boost"

find ../libraries -name "*.cpp" | while read -r src
do
    export BASENAME=`basename $src`

    $CC $CCFLAGS $CCINCLUDES $src
    if [ $? -ne 0 ]; then exit 1; fi

    lcov -c -i -b .. -d . -o $BASENAME.baseline
    if [ $? -ne 0 ]; then exit 1; fi

    ./a.out
    if [ $? -ne 0 ]; then exit 1; fi

    lcov -c -d . -b .. -o $BASENAME.out
    if [ $? -ne 0 ]; then exit 1; fi

    lcov -a $BASENAME.baseline -a $BASENAME.out -o $BASENAME.lcov
    if [ $? -ne 0 ]; then exit 1; fi

    rm $BASENAME.baseline $BASENAME.out
done
