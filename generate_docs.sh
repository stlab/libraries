#!/bin/bash

# The intent of this script is to generate the documentation shell that will eventually be migrated
# to stlab.github.io. This script assumes `stlab/` and `stlab.github.io/` are sibling repositories.

pushd $(dirname $0) > /dev/null

XCODE_TOOLCHAIN=$(xcode-select -p)

# They moved it _again_! *shakes fist*
XCODE_11_5_CPP_DIR=${XCODE_TOOLCHAIN}/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1

# echo ${XCODE_11_5_CPP_DIR}

HYDE_ARGS="--hyde-update --access-filter-public --namespace-blacklist=detail,unsafe --use-system-clang"

hyde ${HYDE_ARGS} stlab/forest.hpp -- -I${XCODE_11_5_CPP_DIR}

popd > /dev/null
