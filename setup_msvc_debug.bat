@ECHO OFF

set VC_ROOT=%VS140COMNTOOLS%..\..\VC\

if exist "%VC_ROOT%vcvarsall.bat" (
    call "%VC_ROOT%vcvarsall.bat" x86_amd64
)

MKDIR build_debug
PUSHD build_debug

::conan install .. -o Boost:shared=False --build=missing
conan install .. -s compiler="Visual Studio" -s compiler.version=14 -s build_type=Debug -s compiler.runtime=MDd

cmake -G "Visual Studio 14 Win64" ..

POPD
