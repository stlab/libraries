#!/bin/bash

mkdir -p build
pushd build

conan install .. --build=missing -s build_type=Debug -o testing=True -s compiler.cppstd=17

cmake -GXcode -D CMAKE_BUILD_TYPE=debug -D stlab_testing=ON ..

popd
