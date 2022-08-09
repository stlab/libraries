#!/bin/bash

#
# This script invokes Hyde on the headers in the stlab directory.
# TODO: Usage documentation
# Also see .hyde-config.
#

#
# Constants
#

CMAKE_BUILD_DIR=../BUILD-DOCS

XCODE_TOOLCHAIN=$(xcode-select -p)
XCODE_CPP_DIR=${XCODE_TOOLCHAIN}/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1

HYDE_EXECUTABLE=/Users/demarco/dev/hyde/build/hyde
CLANG_ARGS="-I$XCODE_CPP_DIR -I$CMAKE_BUILD_DIR -DSTLAB_TASK_SYSTEM=portable"

#
# User-configured variables
#

FORCE=0
ALL_NAMESPACES=0
PATTERN="*.hpp"
HYDE_ARGS="--hyde-update --access-filter-public --use-system-clang"

#
# Argument Parsing
#

while [[ $# -gt 0 ]]; do
  case $1 in
    -f|--force)
      FORCE=1
      shift # past flag
      ;;
    -a|--all-namespaces)
      ALL_NAMESPACES=1
      shift # past flag
      ;;
    -p|--pattern)
      PATTERN=$2
      shift # past flag
      shift # past value
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit 1
      ;;
  esac
done

if [ $ALL_NAMESPACES -eq 0 ]
then
    HYDE_ARGS="${HYDE_ARGS} --namespace-blacklist=detail,unsafe"
fi

#
# Invoke CMake, iff: 
# - the build directory does not exist, or
# - the user specified `--force`
#

if [ -d $CMAKE_BUILD_DIR ] && [ $FORCE -eq 0 ]
then
    echo "$CMAKE_BUILD_DIR already exists. Please delete it or use -f." 
    exit 1
else
    cmake -S. -B $CMAKE_BUILD_DIR -GNinja -DCMAKE_CXX_STANDARD=17 -DCMAKE_BUILD_TYPE=Release
fi

pushd $(dirname $0) > /dev/null

find stlab -name $PATTERN -exec $HYDE_EXECUTABLE $HYDE_ARGS {} -- $CLANG_ARGS \;

popd > /dev/null
