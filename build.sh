#!/bin/bash

# Run a command, and echo before doing so. Also checks the exit
# status and quits if there was an error.
echo_run ()
{
    echo "$@"
    "$@"
    r=$?
    if test $r -ne 0 ; then
        exit $r
    fi
}

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

    echo_run git clone --branch $TRAVIS_BRANCH https://github.com/stlab/libraries.git stlab
else
    echo "Found stlab. Pulling $TRAVIS_BRANCH..."

    echo_run cd stlab
    echo_run git branch -u origin/$TRAVIS_BRANCH
    echo_run git co $TRAVIS_BRANCH
    echo_run git pull origin $TRAVIS_BRANCH
    echo_run cd ..
fi

if [ ! -d boost ]; then
    if [ ! -f boost.tgz ]; then
        echo "Downloading boost..."
        echo_run curl -L https://downloads.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.gz -o boost.tgz
    fi

    if [ ! -d boost ]; then
        mkdir boost
    fi

    echo "Unpacking boost headers..."
    echo_run tar -C boost -xzf boost.tgz --strip-components 1 boost_1_60_0/boost/

    echo "Unpacking boost sources..."
    echo_run tar -C boost -xzf boost.tgz --strip-components 1 boost_1_60_0/libs/
else
    echo "Found boost..."
fi

cd ..

find ./libraries -name "*.cpp" | while read -r src
do
  echo_run $CC -Wall -Werror -x c++ -std=c++14 -I./build/stlab -I./build/boost -o ./build/a.out $src

  ./build/a.out > /dev/null
done
