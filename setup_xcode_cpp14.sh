#!/bin/bash

mkdir -p build
pushd build

conan install .. --build=missing -s build_type=Debug -o testing=True -s compiler.cppstd=14

cmake -GXcode -D CMAKE_BUILD_TYPE=debug -D stlab_testing=ON ..

popd


mkdir -p build_wasm
pushd build_wasm

# -DCMAKE_FIND_ROOT_PATH=$PATH_TO_BOOST_BUILT_BY_EMSCRIPTEN
emcmake cmake -DCMAKE_BUILD_TYPE=Release -Dstlab_testing=ON -DCONAN_DISABLE_CHECK_COMPILER=1 .. && make && ctest --timeout 60 --verbose

popd
