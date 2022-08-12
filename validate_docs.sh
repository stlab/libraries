#!/bin/bash

#
# This script invokes Hyde on the headers in the stlab directory.
# Also see .hyde-config.
#

#
# Constants
#

CMAKE_BUILD_DIR=../BUILD-DOCS

# XCODE_TOOLCHAIN=$(xcode-select -p)
# XCODE_CPP_DIR=${XCODE_TOOLCHAIN}/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1

HYDE_EXECUTABLE=`which hyde`
HYDE_VALIDATE_FLAG="--hyde-validate"
HYDE_UPDATE_FLAG="--hyde-update"
#HYDE_ARGS="--access-filter-public --use-system-clang"
HYDE_ARGS="--access-filter-public"
# CLANG_ARGS="-I$XCODE_CPP_DIR -I$CMAKE_BUILD_DIR"
CLANG_ARGS="-I$CMAKE_BUILD_DIR --std=c++17 --stdlib=libc++"
CMAKE_ARGS="-GNinja -DCMAKE_CXX_STANDARD=17 -DCMAKE_BUILD_TYPE=Release -DSTLAB_TASK_SYSTEM=portable -DBUILD_TESTING=OFF"

#
# User-configured variables
#

ALL_NAMESPACES=0
FORCE=0
PATTERN="*.hpp"
UPDATE=0

#
# Help
#

Help()
{
   echo "validate_docs.sh: Validate and update documentation with Hyde"
   echo
   echo "Usage: ./validate_docs.sh [-h] [-a|-f|-u] [-p] PATTERN"
   echo "options:"
   echo "-a | --all-namespaces    | Process symbols in all namespaces, including \`detail\` and \`unsafe\`."
   echo "-f | --force             | Overwrite the build directory located at $CMAKE_BUILD_DIR."
   echo "-h | --help              | Print this help message."
   echo "-p | --pattern <PATTERN> | File glob passed to \`find\`. Defaults to \"$PATTERN\". See \`man find\` for more."
   echo "-u | --update            | Use --hyde-update to generate new documentation"
   echo
}

#
# Argument Parsing
#

while [[ $# -gt 0 ]]; do
  case $1 in
    -a|--all-namespaces)
      ALL_NAMESPACES=1
      shift # past flag
      ;;
    -f|--force)
      FORCE=1
      shift # past flag
      ;;
    -h|--help)
      Help
      exit
      ;;
    -p|--pattern)
      PATTERN=$2
      shift # past flag
      shift # past value
      ;;
    -u|--update)
      UPDATE=1
      shift # past flag
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
