---
title: To Do List
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/To_Do_List
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Goals
We are now in a mode of completing the core of ASL (defined to be roughly the libraries which make of the property model and layout libraries as well as anything which is complete enough to warrant inclusion).

To that end, we are reviewing all of the libraries and for each one determining:

* Is it used and/or useful and worth being completed. If not it will either be removed all together or partitioned into the platform libraries for later consideration.
* What is necessary to complete the space to create a completed body of work.

We are working to restructure the documentation to include the "final" set of libraries and to flesh out the documentation, test cases, and in some cases, the code.

## Property Model Library
[1/12/09] I'm now in the middle of a major overhaul of the property model library - This is slated for the 1.0.41 release. Here is the plan:

1. <del>Write a "flow" algorithm on the interface cells / relationships to determine the calculation order without the need for backtracking.</del>
1. <del>Implement caching so calculations don't need to be repeated.</del>
1. <del>Implement contributing tracking by watching cell access and remove it from the VM.</del>
1. <del>Implement a "contributing" function that can be called from within the VM (similar to todays function but taking a named cell instead of an expression).</del>
1. Rework contributing tracking on initializers as above to work with caching.
1. Verify that "considered" is being set correctly with the new flow algorithm.
1. Review all logic to ensure that no new bugs were introduced.
1. Review all error handling to ensure a proper separation of runtime checks and asserts.
1. Uncouple the VM from the sheet - the sheet should operate only on function objects.
    1. Provide a variable lookup function for the VM (as well as programatic inspection).

Client impact:
1. <code>contributing()</code> function no longer built into VM. Use sheet_t::contributing_function_t (wired into VM now by the xxx).
1. interface cells must be declared before being referenced in a relate clause.

Beyond the 1.0.41 release here are some things I'd like to see happen next:

1. Review of data-structures and algorithms and updating to use appropriate structures (boost graph?)
1. Factor out input cells/initializers and output cells from the core code (separate components?)

## dictionary.hpp

* The get_value() functions are poorly named and could be generalized to work with any indexed container.
(notes from conversation about removing get_value() from dictionary.hpp and providing a generalized form).

So - current summary and action items:
1) get_value() in dictionary.hpp becomes deprecated next release (hmm - move to a deprecated namespace?) in favor of new algorithms.
2) We add:
I find_at(I f, I l, const T&, P);
[along with all range variants and specialize for associative containers - P defaults to identity if value_type(I) == P)
otherwise P defaults to get_element<0> (or whatever the name is in adobe/functional.hpp)
3) consider that get_element<0> should have a default implementation of identity to normalize the case.
4) similarly provide find_value_at() and assign_at()

U& find_value_at(I, I, T, P);
I assign_at(I, I, T, U&, P);

5) review exisiting find() algorithms to see which it would also make sense to refine for containers (i.e. find() )
6) Add an is_associative_container<> trait so we can automatically pick up new ASL containers and can be made to work with user containers.

## string.hpp

* Break this up a bit - we have a container and string based algorithms.
* Factor out a guarded vector (a vector with a sentinel) as a base type for string_t and string16_t
* Add algorithms for guarded ranges (a range specified by a single iterator and a sentinel)
** should include an end_guarded() and size_guarded() functions - use these instead of the boost range functions when dealing with NTBS

## Algorithms

* every file where we shadow the std functions should go ahead and move the std function into the adobe namespace.

### Non-Mutating Algorithms

* Order Selection Networks -
    * [low priority] consider completing select.hpp to some k elements.
* [low priority] consider adding sorting networks (sort_2, sort_3, etc.)
* [done] Removed bound_if - replaced with key_function form of lower/upper_bound.

Write a proper partition() (ordering and for forward iterator), partition_point_n(), and is_partitioned.

non_strict_order_partition(range, element, strict_weak_ordering) arranges it so that elements are not greater followed by elements which are not less.

------

* [done] Pull count_min/max_element. Replace with two passes - max_element(), count().

-----

Binary searches

Write a better binary_search which returns lower_bound() if exists else last.

Write an adaptor for is_in_sorted_range - set_member?

find_first_of_set() - becomes find_if with adaptor.

We should try to adapt binary search algorithms such as upper bound and lower bound to work on sets and multisets.

### Finding and Counting

* [done] rename find_not_equal to find_not.

Extending find() - also provide find_not() and count_if_not() and count_not().
exists, none, all?

Check what other adobe/STL algorithms take unary predicates (partition_not?).

Versions of find and count need to take a binary predicate -
see find_match().

And find_n(), count_n() for all versions.

-----

Versions of for_each
for_each_value() - rename of for_each_position()
for_each_value_n()
for_each_n()

-----

Sorting Group
is_sorted
sorted() rename to sorted_run()

------

mismatch() gets grouped with equal() and search()
Add mismatch_n().

------

Look at reduce accumulate and related functions - do we keepr reduce_balanced()?

## Mutating Algorithms

--
copy family

Try to simply use the name copy() for copy_bound() - etc.

Add bounded forms of other functions, i.e. swap_ranges.

Also applies to find and count

```
copy      I I 0
copy_b  I I F F
copy_n  I n O
copy_b  I I O n
copy_b  I n F F
copy_b  I F F
             I I n F F
```

---
tranform_if replaces filter by splitting the function object.

transform_if_not.

Also add _if forms for other functions.

Also have copy_if and copy_if_not (as replacement of remove).
---
Explore iota and fit more into the copy notation.

With a value for increment -
---

It looks like remove_runs and mutate_runs are not used - remove them.

---

Add a reverse group and add reverse_until.

---

Remove the pair return from rotate and simply return m'

---



##  Notes

Check to see which intrinsics (like multiply returning long result) are in C99
