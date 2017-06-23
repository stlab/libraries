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

echo_run cmake ..

make serial_queue_test_coverage

exit 0

make

pushd bin > /dev/null # It'd be great if we could just run all the binaries we find in here...

find . -type f -perm +111 | while read -r curbin
do
    echo_run "$curbin"
done

popd > /dev/null # bin

popd > /dev/null # build
