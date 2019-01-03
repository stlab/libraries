#!/bin/bash

# This script assumes `stlab` and `stlab.github.io` are sibling repositories.

pushd $(dirname "$0") > /dev/null

CUR_DIR=`pwd`
SRC_ROOT="${CUR_DIR}/../stlab"
HYDE_DST_ROOT="${CUR_DIR}/libraries"

# Whole-directory documentation (when the time is right)
#SRC_FILE_SET=${SRC_ROOT}/stlab/*.hpp
# Per-file documentation, separated by spaces (during piecemeal migration)
SRC_FILE_SET="${SRC_ROOT}/stlab/copy_on_write.hpp" # ${SRC_ROOT}/stlab/utility.hpp

for CUR_FILE in ${SRC_FILE_SET}; do
    echo "Processing $CUR_FILE"
    CUR_COMMAND="hyde -access-filter-protected -namespace-blacklist=detail -hyde-src-root=${SRC_ROOT} -hyde-yaml-dir=${HYDE_DST_ROOT} -hyde-update \"${CUR_FILE}\" -- -I${SRC_ROOT} -Wno-everything"
    echo $CUR_COMMAND
    eval $CUR_COMMAND
done

popd > /dev/null
