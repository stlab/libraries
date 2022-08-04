---
title: Troubleshooting
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Troubleshooting
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Issues Surrounding MSVC 8.0's "Safe" Libraries

ASL does not support the non-standard "Safe" C and C++ libraries shipping with Visual C++ 8.0. This may result in many spurious warnings from ASL, Boost, and other standards-conforming C and C++ libraries. To suppress these warnings, define the macros
* <code>_CRT_SECURE_NO_DEPRECATE</code>, and
* <code>_SCL_SECURE_NO_DEPRECATE</code>

The MSVC command-line settings to define these macros is:

```
\D_CRT_SECURE_NO_DEPRECATE \D_SCL_SECURE_NO_DEPRECATE
```

The ASL header file <code><adobe/config.hpp></code> includes checks when compiling with MSVC 8.0 to make sure these macros are defined. To disable this check, define the following macro before including <code><adobe/config.hpp></code>:

```
\DADOBE_TEST_MICROSOFT_NO_DEPRECATE=0
```

## Problems Building ASL

### On Windows

#### Building Adobe Begin for Non-Development Machines

When an app is built using a version of MSVC, that version of
Microsoft's Runtimes Libraries must be 'findable' by the OS in order to
launch the application. Part of Microsoft's solution to this problem
includes the embedding of an application manifest file into the
application. On development machines, these files are automatically
installed in C:\WINDOWS\system32\. The manifest file describes the
binary's dependencies to external DLLs, and the OS reads the file and
hunts down the dependencies to load them. In a clean-install XP system,
the Runtime Libraries for MSVC 7.0, 7.1, and 8 don't exist, which will
lead ultimately to the failure to launch applications built with these
compilers on non-development machines.

Our current workaround to this problem is to *not* embed the manifest
file in the binary directly, but leave it as a sibling to the app. Then the
necessary runtime libraries must also be placed as siblings to the app,
along with a manifest file describing them to the OS. For some reason
both of these steps are required, and then the app will run in a clean
XP installation. Note that this bug wasn't caught for a long time
because these runtime libraries were automatically found when smoke
testing the apps. (A big thanks goes to Ken Silver for being the first
one report Adobe Begin failing to load on his machine, which isn't set
up for development.)
