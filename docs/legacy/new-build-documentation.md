---
title: New Build Documentation
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/New_Build_Documentation
---

_This documentation last updated during the ASL 1.0.25 distribution._

#### Contents
{:.no_toc}
1. contents
{:toc}

## Building ASL : Requirements

In order to build ASL, the requirements are as follows:
1. A working IDE installation of:
    1. GCC 3.4.4 or better (cygwin)
    2. GCC 4.0.1 or better (MacOS X)
    3. MSVC 8.0 or better (Win32)
    4. MSVC 8 Express (additional instructions needed to get MSVC 8 Express ready to build ASL; Please see below.)
2. A ''patched'' copy of Boost 1.33.1 distribution
3. A copy of the latest ASL distribution
4. A dose of patience

## The Easy Way

### If you are Inside Adobe

Within Adobe the Software Technology Lab maintains Perforce servers for distributions of both Boost and the Adobe Source Libraries. This is the preferred method of downloading ASL within Adobe, as the Boost sources are already patched and placed correctly. Please see the STLab website for more information on getting ASL this way.

_The rest of this document is not for you._

### Building the Libraries Automatically

Two automated net install scripts have been written to ease in the downloading and building of the Adobe Source Libraries. Given a development environment, these scripts will do all the downloading, patching and building necessary to get a working build of ASL and Adobe Begin on your computer.

The Windows version can be downloaded from http://easynews.dl.sourceforge.net/sourceforge/adobe-source/asl_1.0.23_net_setup.bat

The *NIX (including Mac OS X) version can be downloaded from http://easynews.dl.sourceforge.net/sourceforge/adobe-source/asl_1.0.23_net_setup.sh

You may only run these scripts from a directory having no spaces or unusual characters in the path. This is due to a bjam restriction that will cause some of the tests to fail during the build process.

Please also be aware that the script requires use of FTP and HTTP. If you are behind a firewall or cannot otherwise use FTP, the scripts will not work.

_The rest of this document is not for you._

## The Other Way

### Obtaining and Patching Boost

The Boost 1.33.1 distribution can be downloaded from http://sourceforge.net/project/showfiles.php?group_id=7586

### Patching Boost

Decompress the Boost distribution and move the resulting directory to:

    ~/adobe-source/third_party/boost_tp/ (*nix)
    C:\adobe-source\third_party\boost_tp\ (Windows)

The folder should be named boost resulting in the structure:

    ~/adobe-source/third_party/boost_tp/boost/ (*nix)
    C:\adobe-source\third_party\boost_tp\boost\ (Windows)

Boost must be patched with a small number of minor changes. A patchfile
with those changes can be found at:

    ~/adobe-source/third_party/boost_tp/adobe/boost_1_33_1_patch_01.txt (*nix)
    C:\adobe-source\third_party\boost_tp\adobe\boost_1_33_1_patch_01.txt (Windows)

#### Patching under *nix

You can use the Unix patch command to make the necessary changes, but
first make sure all the line endings in the affected files are Unix line
endings. To use the supplied patch under Unix issue the following
command:

    ~/adobe-source/third_party/boost_tp/adobe/patch_boost.sh

#### Patching under Win32

Win32 users can use the patchfile provided, but there are some caveats. First, the patchfile is distributed with Unix line endings, which must first be converted to DOS line endings. If you do not have a program to accomplish this, there is a small command line tool called <code>leconv</code> that will do this for you. It is in:

    C:\adobe-source\adobe\tools\

The GNUWin32 project has made a version of patch for Win32. It
can be obtained at http://sourceforge.net/projects/gnuwin32

If you already use boost build, please make sure that the adobe
version of bjam will be the one found in your path, since ASL relies
on patched support files found relative to the bjam executable.

### Build Option 1: Using Boost Build (bjam)

Boost 1.33.1 ships with a single version of the build tool that supports both v1 and v2 syntax. When the Adobe patch instructions are followed, boost build will be upgraded to be able to produce universal binaries on the macintosh, but only when bjam invoked using v2 syntax. V1 files are untouched by the patch. If you will be moving from v1 to v2 syntax then the guide at http://boost.sourceforge.net/boost-build2/doc/html/bbv2/vs_v1.html may be of some use.

#### Building Under Win32

The Adobe Source Libraries have been compiled and tested under MSVC 8.
Assuming you have installed ASL to the top of your C drive, the
directory structure for setting up a build is as follows:

    C:\adobe-source\adobe\
    C:\adobe-source\third_party\boost_tp\boost\

Note that the contents of the adobe directory are obtained through the
ASL distribution, while the boost directory contains a patched release
of Boost.

Building the ASL libraries is handled through a build script that will
setup the Boost build system and then build the ASL. Open up a Win32 command line terminal and run:

    cd C:\adobe-source\adobe\tools
    .\build.bat

Once this batch file has completed, you don't need to run it
again to build ASL. A more efficient way to build ASL would be to open
up a Win32 command line terminal, and run:

    cd C:\adobe-source\
    .\adobe\tools\bjam.exe

You must build from the adobe-source (top level) directory. Builds
invoked from other locations will fail.

##### For MSVC 8 Express Users

VC 8 Express is not capable of building Win32 executables out of the
box and will fail to build ASL. here are some notes on updating MSVC 8
Express to be Win32/ASL capable:

1.  You must begin by downloading the platform SDK from http://www.microsoft.com/downloads/details.aspx?FamilyId=0BAF2B35-C656-4969-ACE8-E4C0C0716ADB&displaylang=en

If you use the web install you can choose ''not'' to download most of the SDK components to save time. Only the Core SDK appears to be required.

2.  Copy the "bin", "include" and "lib" directory from the PSDK-installation into "%ProgramFiles%\Microsoft Visual Studio 8\VC"

At this point, for example, windows.h should be available within your
MSVC installation. This would be a good time to make sure that you can
successfully build the IDE project "Begin" located in
...\adobe-source\ide_projects\vc8

Another issue is that boost build v2 from boost 1.33.1 does not
provide a VC 8 Express savvy msvc-config, so we must configure boost
build manually. Follow the next steps to complete VC 8 Express
specific preparation:

3.  Choose a "home" directory, and add a %HOME% environment variable
referring to it. For example, if you chose "C:\Home", you would set
the env var by opening the system control
panel/advanced/environment variables and adding the variable HOME
for all users with value C:\Home.

4.  Now create a file in the chosen HOME directory named
user-config.jam . The file must contain the following contents
(assuming that you installed VC 8 Express in the default location):

    import toolset : using ;
    import msvc-config ;

    using msvc : 8.0 : "C:\Program Files\Microsoft Visual Studio 8\VC\bin\cl.exe" ;

5. Open a fresh command window (to pick up the new env var), cd to
your ...\adobe-source directory and invoke adobe\tools\build.bat
and you should be on your way.

#### Building for Mac, *NIX

The Adobe Source Libraries have been compiled and tested under GCC
4.0.1 and GCC 3.4.4 (cygwin). Assuming you have installed ASL to your home directory, the
directory structure for setting up a build is as follows:

    ~/adobe-source/adobe/
    ~/adobe-source/third_party/boost_tp/boost/

Note that the contents of the adobe directory are obtained through the
ASL distribution, while the boost directory contains a patched release
of Boost.

Building the ASL libraries is handled through a build script that will
setup the Boost build system and then build the ASL. The build script is called:

    ~/adobe-source/adobe/tools/build.sh

Running this script from a command line will begin the build process.
Once this batch file has completed, you don't need to run it again to
build ASL. A more efficient way to build ASL would be to open up a
command line terminal and run:

    cd ~/adobe-source/
    ./adobe/tools/bjam

You must build from the adobe-source (top level) directory. Builds
invoked from other locations will fail.

##### MacOS X Universal Binary Support

By default ASL is set up to build Universal Binaries. In
order to disable universal binary building within ASL, open

    ~/adobe-source/Jamfile.v2

and comment out (or delete) the MACOSX case within the first switch
block in the file (dealing with the ARCH and ALIAS_ARCH variables).

#### Troubleshooting the Build

Build fails to run? Check:

* Boost is in its proper place
* Boost was properly patched
* Permissions allow execution of the build scripts
* Line endings of all files are correct for your platform
* Build commands were invoked from proper directory
* Do not try to use MSVC 8 Express without following Express-specific notes above
* Make sure no other bjam executables will be picked up in your PATH ahead of the adobe provided version.

#### bjam Details

Executing the build script will build various excecutables including
bjam (boost build, compatible with version 1 and 2) in a sub-directory
named after the platform, and copy it into the ASL tools directory. The
build will then cause all libraries including libasl, libasl_dev, and
the appropriate pieces of Boost to be built. It will also build and run
several test applications. Copious output will be produced, indicating
the success or failure of the build and the associated tests. Debug and
Release targets are supported. The default is to build and use static
libraries throughout, except that DLL versions of platform runtime
libraries are employed.

The "projects" used by BBv2 are always named Jamfile.v2. Each
Jamfile.v2 inherits settings from any Jamfile.v2's that appear in its
parent directories, so the Jamfile.v2's in the test directories are
relatively sparse.

##### Jamfile.v2 Miscellanea

* Some Jamfile.v2's specify build requirements and settings that will be applied to all users of a library.
* passing bjam the -d2 option will cause the commands used in the build to be displayed.
* Adobe libs and executables are written to a sub-directory of

    ~/adobe-source/tools/ (*nix)

  or

    C:\adobe-source\tools\ (Windows)

  named according to the build settings. There are also 'bundled' artifact collections that will appear in:

    ~/adobe-source/install/ (*nix)

  or

    C:\adobe-source\install\ (Windows)

* The required boost libraries, including filesystem, threads, and signal, will be built in a sub-directory of

    ~/adobe-source/third_party/boost_tp/boost/bin/ (*nix)

  or

    C:\adobe-source\third_party\boost_tp\boost\bin\ (Windows)

* It is possible to create an IDE project for MSVC 7.1 (and probably for XCode, although this has not been tested) that links to the static libs in the appropriate bin folders. In this scenario it is very important to make sure that the compiler and linker settings used in the IDE match those used for the artifacts produced using BBv2. The -d2 option above is helpful for this purpose. If you try this and receive many link errors on Windows it maybe that your C++/code generation/runtime settings are set to /MTd instead of /MDd.

* More information on BBv2 can be found at http://boost.sourceforge.net/boost-build2/doc/html/index.html

### Build Option 2: Using an IDE

#### Building with MSVC 8

There are MSVC 8 .sln and .vcproj files available for use at

    C:\adobe-source\ide_projects\vc8\

Opening the asl.sln file and building the 'begin' project will build all the necessary library dependencies (including the needed Boost libraries.)

#### Building with XCode

There are XCode .xcodeproj files available for use at

    C:\adobe-source\ide_projects\darwin\

Opening the begin.xcodeproj file and building any configuration therein will build all the necessary library dependencies (including the needed Boost libraries.)

##### MacOS X Universal Binary Support

To disable building Universal Binaries with the XCode IDE projects, open
the top-level xcconfig file for your respective binary. For example:

    ~/adobe-source/ide_projects/darwin/adobe_xconfig_application.xcconfig

and edit the value in the "Artifact Architecture" section. Reloading the
XCode IDE projects at that time will cause Universal Binary support to
be dropped, building only PowerPC versions of the binaries.
Alternatively, you can drop "ppc" if you are only interested in building
MacTel versions of the binaries.

##### MacOS X Binary Artifact Compatibility

On Mac OS X there is an environment variable
<code>MACOSX_DEPLOYMENT_TARGET</code> that can be optionally set to
10.1, 10.2, 10.3, or 10.4. When this variable is set to one of those
values, it establishes the minimum operating system version that is
supported by your binary artifact. For the Adobe Source Libraries, all
binaries are built with a minimum compatibility of Mac OS X 10.3. (Our
true minimum requirement is 10.3.9, but the variable doesn't support
dot-upgrades).

In bjam, this variable is set in the top level Jamfile. (the gcc
command-line equivalent to <code>MACOSX_DEPLOYMENT_TARGET</code> is
<code>-mmacosx-version-min=10.X</code> -- check the gcc man page for
more info.)

In XCode, this is managed with the top-level xcconfig file for the
binary artifact.

## Known Issues

### General Issues

We are aware the release is generally quite cumbersome, and are still
figuring out how to package releases more efficiently.

BBv2 intentionally builds the debug variant of ASL with "warnings as
errors" turned on. The release variant is not built with this setting on
because of warnings within some library headers in the GCC STL.

### Issues Surrounding MSVC 8.0's "Safe" Libraries

ASL does not support the non-standard "Safe" C and C++ libraries
shipping with Visual C++ 8.0. This may result in many spurious warnings
from ASL, Boost, and other standards-conforming C and C++ libraries. To
suppress these warnings, define the macros

    _CRT_SECURE_NO_DEPRECATE

and

    _SCL_SECURE_NO_DEPRECATE

The MSVC command-line settings to define these macros is:

    \D_CRT_SECURE_NO_DEPRECATE \D_SCL_SECURE_NO_DEPRECATE

The ASL header file <code>adobe/config.hpp</code> includes checks when compiling
with MSVC 8.0 to make sure these macros are defined. To disable this
check, define the following macro before including <code>adobe/config.hpp</code>:

    ADOBE_TEST_MICROSOFT_NO_DEPRECATE=0

The Jamfile at the top of the ASL distribution has this macro defined by
default. Another option to disable the warnings in your code is by
supressing the warnings with a pragma:

    #pragma warning ( disable : 4996 )

### Executing Adobe Begin on Windows XP on a Non-Development Machine

When an app is built using a version of MSVC, that version of
Microsoft's Runtimes Libraries must be 'findable' by the OS in order to
launch the application. Part of Microsoft's solution to this problem
includes the embedding of an application manifest file into the
application. On development machines, these files are automatically
installed in C:\WINDOWS\system32\. The manifest file describes the
binary's dependencies to external DLLs, and the OS reads the file and
hunts down the dependencies to load them. In a clean-install XP system,
the Runtime Libraries for MSVC 7.0, 7.1, and 8 do not exist, which will
lead ultimately to the failure to launch applications built with these
compilers on non-development machines.

Our current workaround to this problem is to *not* embed the manifest
file in the binary directly, but leave it as a sibling to the app. Then
the necessary runtime libraries must also be placed as siblings to the
app, along with a manifest file describing them to the OS. For some
reason both of these steps are required, and then the app will run in a
clean XP installation. Note that this bug was not caught for a while
because these runtime libraries were automatically found when smoke
testing the apps. (A big thanks goes to Ken Silver for being the first
one report Adobe Begin failing to load on his machine, which was not set
up for development.)

## Feedback

The Adobe Source Libraries have been used with success within Adobe
applications to increase robustness and correctness of code. As
developers of an open source project, we are confident that there are
other success stories outside of Adobe as well. We encourage you to
contribute a success story of your own:

* What is the name of your project?
* What is the purpose of the project?
* What led you to choose ASL?
* Was ASL used from the start of the project? If not, how did its inclusion impact your project? How does using ASL affect your project development as a whole?
* How large is your project?
* How would you rate the success of your project?
* Is there any other interesting information worth sharing?

Submissions will be included in a success stories web page on the
opensource.adobe.com site. You can submit a success story by emailing
Foster Brereton at his adobe.com email address (fbrereto).

Of course, feedback of any kind is highly requested at all times. Please contact one of the project leads.
