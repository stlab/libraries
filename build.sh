#!/bin/bash

# Run a command, and echo before doing so. Also checks the exit
# status and quits if there was an error.
echo_run ()
{
    echo "$@"
    "$@"
    r=$?
    if test $r -ne 0 ; then
        exit $r
    fi
}

cd `dirname $0`

if [ ! -d build ]; then
    mkdir build
fi

pushd build > /dev/null

conan install ./.. --build=missing

NPROC=`sysctl -n hw.ncpu`
if [ "$NPROC" == "" ] ; then
    NPROC=`nproc`
fi

echo "INFO : Found $NPROC processors."

echo_run cmake .. && make -j$NPROC

echo_run lcov -c -i -b .. -d . -o Coverage.baseline

pushd bin > /dev/null

# run all the binaries we find in here...
find . -type f -perm +111 | while read -r curbin
do
    echo_run "$curbin"
done

popd > /dev/null # bin

echo_run lcov -c -d . -b .. -o Coverage.out
echo_run lcov -a Coverage.baseline -a Coverage.out -o Coverage.lcov
echo_run rm Coverage.baseline Coverage.out

popd > /dev/null # build
