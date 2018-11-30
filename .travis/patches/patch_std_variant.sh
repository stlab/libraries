#!/bin/bash

if [ ! -d /usr/lib/gcc/x86_64-linux-gnu/7.3.0/../../../../include/c++/7.3.0/ ]; then
    echo "You must have a destination directory (gcc 7.3.0 includes) in order to patch!"
    exit 1;
fi

cd /usr/lib/gcc/x86_64-linux-gnu/7.3.0/../../../../include/c++/7.3.0/

patch -uN -p0 -g0 < "std_variant.patch" && echo "Patching of std.variant succeeded. Happy coding!"

exit 0
