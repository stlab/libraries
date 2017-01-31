@ECHO OFF

set VC_ROOT=%VS140COMNTOOLS%..\..\VC\

if exist "%VC_ROOT%vcvarsall.bat" (
    call "%VC_ROOT%vcvarsall.bat" x86_amd64
)

MKDIR build
PUSHD build

conan install .. -o Boost:shared=False --build=missing

cmake -G "Visual Studio 14 Win64" ..

POPD
