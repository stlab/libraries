#!/bin/bash

set -e
set -x

if [[ "$(uname -s)" == 'Darwin' ]]; then
    # brew update
    brew outdated pyenv || brew upgrade pyenv
    brew install pyenv-virtualenv
    brew install cmake || true

    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi

    pyenv install 3.8.6
    pyenv virtualenv 3.8.6 conan

    pyenv rehash
    pyenv activate conan
fi

pip install --upgrade pip
pip install conan_package_tools==0.35.0
pip install conan==1.32.0

conan --version
conan user
