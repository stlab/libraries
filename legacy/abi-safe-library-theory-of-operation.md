---
title: ABI safe library theory of operation
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/ABI_safe_library_theory_of_operation
---
The idea with the ABI safe library (currently the version_1 namespace types) is to provide a set of types that have a defined binary representation regardless of compiler settings, version, or options. And so these types can be passed freely across a DLL boundary.

The template types are intended to be ABI safe so long as they are themselves instantiated with an ABI safe type.

The following are the specific techniques that the library uses:

1) Memory allocation - any heap allocated object will carry with it an capture_allocator (look up the actual type name). This allocator holds a pointer to a new/delete pair of proc pointers. All allocations go through the allocator and all frees also go through the allocator. This ensures that a consistent new/delete pair is used if a DLL or application of locally replaced these operations.

1a) The memory allocators in the ABI safe containers are copied with the pointers which were allocated by them. With STL containers it is undefined if the allocator is copied with the container, this means that the capture_allocator is not guaranteed to work with containers other than the version_1 containers.

2) Alignment - the types all try to ensure maximal alignment of types where necessary - this is usually done by aligning to the natural alignment for the type up to 64 bits (there may be issues with types that require 128 bit alignment). There is currently a known issue here with close_hash_map which relies on adobe::pair but makes no attempt to ensure alignment of the second element. So far this hasn't caused any real world problems and it should be fixed at some point by trying to ensure that the current "typical" alignment is kept.

3) No use of C++ compiler constructs, such as RTTI and v-tables which don't have a defined representation. Instead there is an adobe typeinfo system that relies on matching by name (rather than by a symbol that would need to be exported) and constructing v-tables with tables of proc-pointers (compiler generated as static structs - see any_regular_t).
