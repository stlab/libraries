@ECHO OFF

set VC_ROOT=%VS140COMNTOOLS%..\..\VC\

if exist "%VC_ROOT%vcvarsall.bat" (
    call "%VC_ROOT%vcvarsall.bat" x86_amd64
)

MKDIR build_debug
PUSHD build_debug

::conan install .. -o Boost:shared=False --build=missing
:: Use this line, when the package is not available
::conan install .. -s compiler="Visual Studio" -s compiler.version=15 -s build_type=Debug -s compiler.runtime=MDd -o Boost:shared=False --build boost
conan install .. -s compiler="Visual Studio" -s compiler.version=15 -s build_type=Debug -s compiler.runtime=MDd -o Boost:shared=False

cmake -G "Visual Studio 15 Win64" -D CMAKE_BUILD_TYPE=debug -D stlab_testing=ON ..

POPD
