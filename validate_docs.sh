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

HYDE_EXECUTABLE=`which hyde`
HYDE_VALIDATE_FLAG="--hyde-validate"
HYDE_UPDATE_FLAG="--hyde-update"
HYDE_ARGS="--access-filter-public --use-system-clang"
CLANG_ARGS="-I$XCODE_CPP_DIR -I$CMAKE_BUILD_DIR"
CMAKE_ARGS="-GNinja -DCMAKE_CXX_STANDARD=17 -DCMAKE_BUILD_TYPE=Release -DSTLAB_TASK_SYSTEM=portable"

#
# User-configured variables
#

ALL_NAMESPACES=0
FORCE=0
PATTERN="*.hpp"
UPDATE=0

#
# Argument Parsing
#

while [[ $# -gt 0 ]]; do
  case $1 in
    -u|--update)
      UPDATE=1
      shift # past flag
      ;;
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

#
# Only update the documentation if -u|--update was provided.
# By default, only run Hyde validation.
#

if [ $UPDATE -eq 1 ]
then
    HYDE_ARGS="$HYDE_UPDATE_FLAG $HYDE_ARGS"
else
    HYDE_ARGS="$HYDE_VALIDATE_FLAG $HYDE_ARGS"
fi

#
# Ignore detail namespaces by default, unless -a|--all-namespaces was provided.
#

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
    cmake -S. -B $CMAKE_BUILD_DIR $CMAKE_ARGS
fi

#
# Run Hyde on every file matching $PATTERN found in the stlab/ directory.
#

pushd $(dirname $0) > /dev/null

find stlab -name $PATTERN -exec $HYDE_EXECUTABLE $HYDE_ARGS {} -- $CLANG_ARGS \;

popd > /dev/null
