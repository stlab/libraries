#!/bin/bash

#
# The intent of this script is to generate the documentation shell for stlab.github.io.
# Note that stlab.github.io is a submodule of this repository, stored in /docs.
# Also see .hyde-config.
#

#
# Constants
#

CMAKE_BUILD_DIR=../BUILD-DOCS

XCODE_TOOLCHAIN=$(xcode-select -p)
XCODE_CPP_DIR=${XCODE_TOOLCHAIN}/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1

HYDE_EXECUTABLE=/Users/demarco/dev/hyde/build/hyde
HYDE_ARGS="--hyde-update --access-filter-public --namespace-blacklist=detail,unsafe --use-system-clang"
CLANG_ARGS="-I$XCODE_CPP_DIR -I$CMAKE_BUILD_DIR"

#
# User-specified variables
#

FORCE=0

#
# Argument Parsing
#

while [[ $# -gt 0 ]]; do
  case $1 in
    -f|--force)
      FORCE=1
      shift # past argument
      ;;
    -*|--*)
      echo "Unknown option $1"
      exit 1
      ;;
  esac
done

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

find stlab -name "*.hpp" -exec $HYDE_EXECUTABLE $HYDE_ARGS {} -- $CLANG_ARGS \;

popd > /dev/null
