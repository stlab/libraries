---
title: Debugging `any_regular_t` and short names
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Debugging_any_regular_t_and_short_names
---

Debugging the any_regular_t type can be problematic if you don't know what the data type of the value stored in it is. To make it simpler for to view an any_regular_t in the debugger, release 1.0.41 contains a four character short name for the type which is stored at the top of the vtable.

The structure of an any_regular_t is a pair of doubles, the first word of the first double contains a pointer to a vtable.

<table border='1'>
    <caption align='center'>any_regular_t</caption>
    <tr> <th colspan='2'>double</th><th>double</th> </tr>
    <tr> <td>vtable*</td><td>pad (on 32 bit machines)</td><td>data or data*</td> </tr>
</table>

The first word of the vtable contains a version number on release builds (currently 1) and a short name (a four character constant) on debug builds.

<table border='1'>
    <caption align='center'>vtable</caption>
    <tr><td>version or short name</td></tr>
    <tr><td>... proc pointers ...</td></tr>
</table>

By convention, the four character codes are stored in little endian order (so as to be readable in the debugger when viewing memory) -

<table border='1'>
    <caption align='center'>short name for adobe::version_1::dictionary_t</caption>
    <tr><td>'d'</td><td>'i'</td><td>'c'</td><td>'t'</td><td>(remainder of word is zeros on 64 bit machine)</td></tr>
</table>

Also by convention, ASL will only use lower case letters in their short names - clients are free to use upper case letters. Note that the short name facility is to aid debugging _only_. it is not intended as a unique name for every type.

You can define a name for your own type using the ADOBE_SHORT_NAME_TYPE() macro in the global scope defined in <adobe/typeinfo.hpp>. For example:

```cpp
namespace my_space {
class my_class { };
} // namespace my_space

ADOBE_SHORT_NAME_TYPE('M','y','C','l', my_space::my_class);
```

Once defined, a short name for a particular type can be referred to using the short_name type function:

```cpp
cout << adobe::short_name<double>::value;
```

The following is a list of the short names for the CEL types defined in ASL:

<table border='1'>
    <caption align='center'>CEL short names</caption>
    <tr><th>type</th><th>name</th></tr>
    <tr><td>double</td><td>dble</td></tr>
    <tr><td>bool</td><td>bool</td></tr>
    <tr><td>empty_t</td><td>emty</td></tr>
    <tr><td>array_t</td><td>arry</td></tr>
    <tr><td>dictionary_t</td><td>dict</td></tr>
    <tr><td>string_t</td><td>strg</td></tr>
    <tr><td>name_t</td><td>name</td></tr>
</table>

In addition, the following short names are defined though these won't typically appear in an any_regular_t:

<table border='1'>
    <caption align='center'>short names</caption>
    <tr><th>type</th><th>name</th></tr>
    <tr><td>string16_t</td><td>st16</td></tr>
    <tr><td>int</td><td>int_</td></tr>
    <tr><td>short</td><td>short</td></tr>
    <tr><td>unsigned int</td><td>uint</td></tr>
    <tr><td>unsigned short</td><td>ushr</td></tr>
    <tr><td>unsigned long</td><td>ulng</td></tr>
    <tr><td>char</td><td>char</td></tr>
    <tr><td>signed char</td><td>schr</td></tr>
    <tr><td>uchar</td><td>uchr</td></tr>
</table>

