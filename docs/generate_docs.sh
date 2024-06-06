#!/bin/bash

pushd $(dirname "$0") > /dev/null

CUR_DIR=`pwd`
SRC_ROOT="${CUR_DIR}/.."
HYDE_DST_ROOT="${CUR_DIR}/includes"

# Whole-directory documentation (when the time is right)
SRC_FILE_SET=`find ${SRC_ROOT}/stlab -name \*.hpp`
# SRC_FILE_SET="${SRC_ROOT}/stlab/*.hpp ${SRC_ROOT}/stlab/concurrency/*.hpp"
# Per-file documentation, separated by spaces (during piecemeal migration)
# SRC_FILE_SET="${SRC_ROOT}/stlab/copy_on_write.hpp ${SRC_ROOT}/stlab/forest.hpp"

for CUR_FILE in ${SRC_FILE_SET}; do
    echo "Processing $CUR_FILE"
    CUR_COMMAND="hyde --access-filter-protected --namespace-blacklist=detail --hyde-update \"${CUR_FILE}\" --hyde-yaml-dir=${HYDE_DST_ROOT} --hyde-src-root=${SRC_ROOT} -- -std=c++20 -I${SRC_ROOT}/../build/hyde/ -I${SRC_ROOT}/"

    echo $CUR_COMMAND
    eval $CUR_COMMAND
done

popd > /dev/null
