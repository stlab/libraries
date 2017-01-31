#!/bin/bash

mkdir -p build
pushd build

    conan install .. --build=missing -s build_type=Debug

    cmake -GXcode ..

popd
