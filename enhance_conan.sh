#!/bin/bash
#

if conan remote list | grep -q 'community'; then
    echo "conan community already configured";
else
    echo "conan add community"
    conan remote add community https://api.bintray.com/conan/conan-community/conan;
fi

