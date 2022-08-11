---
title: GitHub Migration and Status
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/GitHub_Migration_and_Status
---

ASL is now available on GitHub at <https://github.com/stlab/>.

The plan is to breakup ASL into separate libraries following the boost modular library structure: https://github.com/boostorg

We are following the gitflow model of development http://nvie.com/posts/a-successful-git-branching-model/

I'm (sean_parent) currently working only with XCode 4.5.2. The Begin test app project can currently be build with XCode.

To setup a build:
1.clone the legacy repository and switch to the development branch
2. download boost 1.52.0 from boost.org
3. create a symlink to your boost directory inside the legacy directory (here is the command from my worksheet):

        ln -s ~/development/third_party/boost_1_52_0 ~/development/github/stlab/legacy/boost_libraries

4. Open the project at adobe_platform_libraries/xcode_ide/begin.xcodproj
5. Select the begin scheme and run.

I have not been able to get a working build of boost yet through cmake using the modular structure. Holding off on that work for now and moving forward with bjam in the interim.

To build with bjam:

I've moved to requiring C++11 (clang when using XCode). Boost 1.52.0 is currently not defining the BOOST_NO_CXX11_NUMERIC_LIMITS correctly. I haven't yet figured out how to get these items into the appropriate jam files.

    bjam toolset=clang cxxflags="-stdlib=libc++ -std=c++11" linkflags="-stdlib=libc++" define="BOOST_NO_CXX11_NUMERIC_LIMITS"

There are still some of the test apps that fail to build in the platform libraries. Trying to figure those out.

1. Update (fbrereto, 15 Nov 2012) :
    1. BOOST_NO_CXX11_NUMERIC_LIMITS seems to be defined when using the darwin (gcc) toolset, so it would appear the lack of definition when using Clang is a support problem within boost/config.hpp. I discovered this while building the legacy/development branch using the darwin toolset and getting multiple #define errors of the BOOST_NO_CXX11_NUMERIC_LIMITS macro (one from the command line (bjam), one from a file in the boost/config tree.)
    1. Using bjam I am able to get Begin to compile but not link. Apparently though I am specifying <tt>address-model=32</tt> from the command line parts of ASL are still building using a 64-bit architecture and cannot be linked with a 32-bit Begin. Here's what I'm using to build (the clang toolset is implied from my use of a user-config.jam file, not seen here):

            bjam -a release -j8 address-model=32
