#!/bin/bash

pushd $(dirname "$0") > /dev/null

CUR_DIR=`pwd`
SRC_ROOT="${CUR_DIR}/.."
HYDE_DST_ROOT="${CUR_DIR}/include"

# Whole-directory documentation (when the time is right)
SRC_FILE_SET=`find ${SRC_ROOT}/include/stlab -name \*.hpp`
# SRC_FILE_SET="${SRC_ROOT}/stlab/*.hpp ${SRC_ROOT}/stlab/concurrency/*.hpp"
# Per-file documentation, separated by spaces (during piecemeal migration)
# SRC_FILE_SET="${SRC_ROOT}/stlab/copy_on_write.hpp ${SRC_ROOT}/stlab/forest.hpp"

for CUR_FILE in ${SRC_FILE_SET}; do
    echo "Processing $CUR_FILE"
    CUR_COMMAND="hyde --access-filter-protected --namespace-blacklist=detail --hyde-update \"${CUR_FILE}\" --hyde-yaml-dir=${HYDE_DST_ROOT} --hyde-src-root=${SRC_ROOT}/include/ -- -std=c++20 -I${SRC_ROOT}/build/hyde/include/ -I${SRC_ROOT}/include/"

    echo $CUR_COMMAND
    eval $CUR_COMMAND
done

popd > /dev/null
