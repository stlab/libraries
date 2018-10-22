#!/bin/bash

# This script assumes `stlab` and `stlab.github.io` are sibling repositories.

SCRIPT_DIR=$(dirname "$0")
SRC_ROOT="${SCRIPT_DIR}/../stlab"
HYDE_DST_ROOT="${SCRIPT_DIR}/libraries"

pushd ${SCRIPT_DIR} > /dev/null

# Whole-directory documentation (when the time is right)
#SRC_FILE_SET=${SRC_ROOT}/stlab/*.hpp
# Per-file documentation, separated by spaces (during piecemeal migration)
SRC_FILE_SET="${SRC_ROOT}/stlab/copy_on_write.hpp"

for CUR_FILE in ${SRC_FILE_SET}; do
    echo "Processing $CUR_FILE"
    hyde -hyde-src-root=${SRC_ROOT} -hyde-yaml-dir=${HYDE_DST_ROOT} -hyde-update "$CUR_FILE" -- # -I${APOLLO_PATH_INCLUDE} -I${APOLLO_ASL}
done

popd > /dev/null
