---
title: About Boost Patches
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/About_Boost_Patches
---
#### Contents
{:.no_toc}
1. contents
{:toc}

The question has come up as to what the purpose of the Boost patches are, when they are needed and how to go about adding a patch. I'll try to answer these questions here.

## Purpose

The Boost library patches are currently made for one of the following reasons:

* To silence a benign warning (such as a function parameter which isn't used). Often this is in response to a compiler not supported by Boost.
* To fix an actual bug discovered in Boost. This may have surfaced in ASL or it may be a bug that was discovered by a team using Boost at Adobe. Inside Adobe STLab is responsible for distributing the Boost libraries. The patches will reflect this usage.
* To support a compiler which is not currently supported by Boost. This will typically be to support CodeWarrior 9.6 on the Macintosh (a configuration not officially supported by ASL, but many teams at Adobe have not yet moved to XCode) or to support a release of either GCC or Visual C++ which is more recent than that supported by Boost (this may include seed release at Adobe which are not yet public).

## Are The Patches Required?

If you aren't seeing problems with ASL with an unpatched Boost than probably not. We currently haven't been good about adding regression tests for the patches (mostly because the problems encountered have appeared by simply building ASL). In the future we will be better about this. Our tests and builds will always assume a patched Boost library. If ASL works for you without patching Boost, great, but if you encounter an issue, please look over the patches before reporting a bug.

## Process for submitting a patch to Boost

If you aren't sure of the issue, discuss it on the [developer list](mailto:adobe-source-devel@lists.sourceforge.net). Once you have isolated the problem, [report the bug to Boost](https://www.boost.org/development/bugs.html) using the Boost [bug tracking facility](https://svn.boost.org/trac/boost/report).

Send an e-mail to the [mailto:adobe-source-devel@lists.sourceforge.net developer list] or directly to Sean Parent including:

* A complete description of the problem.
* The Boost bug number assigned from source forge.
* The complete path to the file(s) to be patched.
* The complete text of the patched files (please ZIP the files to avoid having mail mangle them).
* The patched portion of the file should include comments denoting the begin and end of the patched section, your e-mail address, and the Boost bug number, and the description of the problem being fixed. In C++ code this should look like:
<pre>
/*
   ADOBE (sparent(a)adobe.com) : Boost bug #12345. Removed unused parameter names to silence warnings.
*/
void function(int);
/*
   END ADOBE
*/
</pre>
* A .cpp test file to regress the bug (if it doesn't happen in the normal course of building ASL or running existing ASL tests.)

## How are Boost updates handled

We will assume that the Boost bugs have been fixed and will always start with a fresh release of Boost. If the regression tests show no problems then it is assumed that the patch is no longer necessary. One implication of this is that fixes for unsupported compilers will have to be resubmitted (this is one way to encourage developers to prioritize updating tools before libraries).

## Descriptions of Current Patches

[Annotated List of Boost Patches](annotated-list-of-boost-patches.html)
