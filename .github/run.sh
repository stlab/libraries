#!/bin/bash

set -e
set -x

if [[ "$(uname -s)" == 'Darwin' ]]; then
    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi
    pyenv activate conan
fi

# echo $CONAN_CHANNEL
# eval $(python ci_utils/set_envvars_and_files.py)
# echo $CONAN_CHANNEL

# python ci_utils/process_conan_reqs.py

python build.py
