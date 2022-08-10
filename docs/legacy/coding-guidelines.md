---
title: Coding Guidelines
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Coding_Guidelines
---
#### Contents
{:.no_toc}
1. contents
{:toc}

_This section is for collecting our coding standards and style. This is primarily of importance to contributors._

## Introduction

We could write an entire book on coding guidelines but luckily that has already been done. I'd recommend reading [_C++ Coding Standards: 101 Rules, Guidelines, and Best Practices_](https://www.amazon.com/dp/0321113586) by Herb Sutter, Andrei Alexandrescu.

I would also recommend the online paper [_C++ Coding Guidelines_](https://xa.yimg.com/df/boost/coding_guidelines.html?token=SbOCPKyzP4V7_D7y8_ItT3nLFvitGq6jwev1XY6bgGQ4xjnBJy7Gi45swQXOscy_DsTtbwBQIhEBc4QCPNuWotlkHuAyUHqbGpJIh6NSy3hTZ01LgQ&type=download) by David Abrahams and Nathan Myers. This document follows a similar format and I will only attempt to document where our guidelines differ or are more refined.

To understand the importance that naming plays in generic programming, I'd recommend working through [Notes on Programming](http://stepanovpapers.com/notes.pdf) by Alex Stepanov. In fact, before writing any code I'd recommend working through this document.

The style of ASL is a refinement of the style used in the standard library as well as in the Boost libraries. A good general rule is to defer to the highest authority.

ASL is, primarily, a research project even though we strive to make each piece of commercial quality (after all, it is research into how to write large scale systems correctly!). In order to be able to push ASL forward, it is important that each piece is written as correctly as we can. To that end, if there are improvements we can make to the style and interfaces we will, even if that breaks backward compatibility. The challenge of versioning the interfaces in a library such as ASL is an area of research I hope to tackle at some point but for now the burden of maintaining backward compatibility is too high.

## Files and the Preprocessor
### License

Every file must include the following license as the first item in the file:

<pre>
/*
    Copyright 2005-2007 Adobe Systems Incorporated
    Distributed under the MIT License (see accompanying file LICENSE_1_0_0.txt
    or a copy at http://opensource.adobe.com/licenses.html)
*/
</pre>

### File Naming and Structure

Source file names are all <code>lower_case</code>. C++ headers have a <code>.hpp</code> suffix. C++ source files have a <code>.cpp</code> suffix. The include files for ASL are all within the <code>adobe</code> directory and are included using the system include quotes, <code><...></code>.

Example:

<pre>
#include <adobe/algorithm/reverse.hpp>
</pre>
<br/>
Where possible - organize the files to match the organization used in STL and Boost. For example, algorithms can be found in <code>adobe/algorithm.hpp</code>. Break up files to a fine granularity to reduce the amount of information which must be included to be compiled. Organize the small headers in a directory with the same root as the master header - <code>adobe/algorithm.hpp</code> will include <code>adobe/algorithm/reverse.hpp</code>.

To further reduce the amount of code included, where possible include a header which only contains forward, or minimal declaration. Such header files should be suffixed with <code>_fwd</code> and only include by other header files. For example <code>adobe/name_fwd.hpp</code>.

### Include Guards

All header files should have internal include guards after the license. The include guards are named the same as the file, including the file path, with punctuation replaced by the underscore character.

Example:

<pre>
#ifndef ADOBE_ALGORITHM_REVERSE_HPP
#define ADOBE_ALGORITHM_REVERSE_HPP
//...
#endif
</pre>
<br/>
Although some compilers will let you get away with it, comments are not allowed in preprocessor directives.

Incorrect:

<pre>
#endif // ADOBE_ALGORITHM_REVERSE_HPP
</pre>

### Line Endings

All files in ASL should use a Unix line endings, a single LF character.

### Line Lengths

In general, no line of code should exceed 80 characters (including any indentation).

### Comments

Comment, but for the sake of the developer who will maintain your code, not for the sake of commenting.

### Spaces vs. Tabs

Due to horrific global conflict over the accepted tab-to-space conversion ratio, tabs are to be avoided at all cost within the sources. Indenting, however, is highly recommended where appropriate, and should be 4 spaces wide.
