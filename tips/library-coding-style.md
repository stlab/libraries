---
title: Library Coding Style
layout: page
tags: [tips]
comments: true
---

This guide is intended to augment existing code guidelines to ensure that code from one project can be easily reused within another. These guidelines are intended to make it easier to share code between internal projects and with external projects.

### Write all code as if it were a library

These guidelines should apply to all the code you write. Even if you don't intend your code to be reused, someone else may find it useful. Following these rules will make it easier for them and won't make your job significantly more difficult. You might even find these make your code more readable. When you write code, consider what would happen if an external library were written following the same style. Would you be able to use that external library or would it likely conflict with your own code?

## Namespaces

All your code should be in a namespace. Think of a namespace as defining a semantic space for names. For example, the word "vector" has a specific meaning when used within the std namespace.

### How to choose a namespace names

A root namespace name should be reasonably unique. Pick a root namespace that is broad enough to be under your control or influence. Usually, the proper scope for the root namespace is your product, not your particular sub-project. Don't create a namespace for every sub-project unless you plan to share that sub-project between products.

Do not use common names for root namespaces such as base and logging. When combining libraries from different sources, especially when they include forked version of libraries (see Patching, Augmenting, and Forking below) you end up with many difficult to resolve collisions.

### Using namespaces

Code which appears in a header file but is not part of the interface should be in a nested details namespace. Code in an implementation file which is only used within that file should be in an unnamed namespace.

You can use a nested namespace when you are dealing with a particular domain, or other library, that may have a different definition of common terms (such as `product::glib::array` would be an array type unique to glib). Use such namespaces sparingly.

Do not use using declarations or namespace aliases in a header file except inside a function definition. These may be used from within an implementation file.

## Macros and extern "C" names

Macros are not enclosed within a namespace, and names which are declared extern "C" do not have their namespace mangled into the linkage name (extern "C" is often used to interface with dynamic/shared libraries). Such names should be prefixed with the namespace name. For example:

```cpp
#define PRODUCT_CASE_RETURN_LABEL(label) ...
extern "C" product_library_load();
```

## Libraries that define symbols in the global scope

We deal with many C libraries or other third party libraries that define symbols in the global scope, fully qualify such names with ::symbol-name to avoid conflicts with locally defined symbols. Don't try to wrap the library in a namespace, i.e.:

```cpp
// DON'T DO THIS

namespace c_library {

#include <c_library.h>

}  // c_library
```

Using a fully qualified name as a template argument requires additional space because <: is an alternative token for [. Example:

```cpp
std::vector< ::GValue*> x;
```

## Directory structure of libraries and include paths

Don't depend on your children. It is common to see external dependencies nested within a sub-project. The problem is that trying to mix these libraries causes no end of conflicts from different versions of the same library, duplicate symbols getting compiled in, etc. Treat dependencies as peers - "third_party" should not appear in a `#include` path. If the dependencies are treated as peers, then a single copy can be used. This may require some patching to update libraries to all use the same version, but this is usually far simpler than fixing build scripts and include paths in every project. Use the system include style with `<>` when referring to files within another library and local file include style with `""` for local files from within implementation files. Header files should only use the system include style. It is acceptable to always use system include notation.

Examples:

```cpp
#include <adobe/vector.hpp>
#include <boost/range.hpp>
#include "local_file.hpp"
```

When adding code that augments an existing library name it to match the library it augments. For example `adobe/algorithm.hpp` augments the standard algorithm library, this pattern makes it much easier for the user to find routines. If a header becomes too large it can be split within a hierarchy with the top level header including the sub-headers. For example, `adobe/algorithm.hpp` includes `adobe/algorithm/find.hpp`.

## Patching, Augmenting, and Forking

Sometimes a library you depend on doesn't quite meet your needs. If the reason is a defect in the original library then write a patch. The patch code should be delineated with a comment as to what is being fixed and for what project. Then figure out the proper way to get the patch upstreamed. Likely this will require writing a bug and possibly submitting a changelist or pull-request. Include a comment with the bug and patch information in the local patch. Example:

```cpp
// FIX <ORGANIZATION> <PROJECT NAME>
// comparison between signed and unsigned type. Values expected to be positive
// so cast to unsigned value.
// External Library <http://external_library>
// bug# 2365, cl# 241

if (x < static_cast<unsigned int>(y))

// END FIX
```

Log the fix in a `README.md` file in the directory for the component.

Often is it tempting to start adding new functions to a component you've pulled in. Don't. Instead, add a companion library to augment the original library.

Note that making changes to an external library may require you to open source the change and/or submit it back to the licensing body. Copying code from an external library may contaminate the sources to your product with the license from the external library. You must first get approval before using any external library and you must use caution in how you use the code.

The temptation is often to fork a library with no effort to upstream the changes (good intentions aside). If you feel you must fork a library to make it suit your needs, then you must take ownership. Remap all the namespaces to your project namespace, as well as any macro and extern "C" names (this may be much more difficult if the library wasn't written to use namespaces or followed other rules). The forked library must be able to coexist with the original, otherwise, at some point in time, the two will meet again and I'll have to ask, "[Your agonizer, please](https://youtu.be/8_rTTXCOpL8?t=106)."
