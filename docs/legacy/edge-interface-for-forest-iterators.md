---
title: Edge Interface For Forward Iterators
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Edge_Interface_For_Forest_Iterators
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Introduction

We can define the concept of the forest iterator in terms of a general graph using the terminology from Boost graph - this is useful because at some point we may wish to generalize these constructs to apply to other graph structures.

A forest is (loosely) a [Bidirectional Graph](https://www.boost.org/doc/libs/1_64_0/libs/graph/doc/BidirectionalGraph.html).

A forest iterator consists of the following:

* <code>v</code>, a vertex descriptor (in the forest implementation this is the pointer to the node).
* <code>e</code>, an <code>in_edge</code> index - this specifies how we are pointing at the node.
* <code>p</code>, a property (this is fixed to a single property of the <code>value_type</code> of the iterator).
* <code>f</code>, a function to move from an <code>in_edge</code> to a corresponding <code>out_edge</code> (for a fullorder iterator on a forest this is the identity function).

An edge is a directed link (with a <code>from()</code> and <code>to()</code> function) - and because it is bidirectional you can find the corresponding input link.

The increment operation can be defined as follows in pseudo code (once I'm more fluent in Boost Graph this could be described more concretely):

<pre>
current_edge = out_edge(*v, e);
v = to(curent_edge);
e = f(find_in_edge(*v, current_edge));
</pre>

In this way we can think of a forest iterator as being a traversal defined over a Bidirectional Graph. The <code>edge()</code> interface on the iterator allows you to manipulate what <code>in_edge</code> is being used to reach the vertex, and correspondingly what <code>out_edge</code> will be followed next. The type of the result is the index for the edge sequence. In the case of forest this is an array, so <code>std::size_t</code> is the correct type, even though it will only be a value of 0 or 1.

The signature for <code>edge()</code> is:

<pre>
std::size_t edge() const; // read access to the edge
std::size_t& edge(); // write access to the edge
</pre>

## Child iterators

The same description holds for <code>child_iterator</code> except <code>f()</code> is replaced with <code>trailing_of(*v)</code> and it no longer depends on which <code>in_edge</code> you arrive on.

## The reverse iterator problem with edge()

The standard notion of a reverse iterator transforms a range <code>[first, last)</code> to the range <code>(first, last]</code> without introducing any extra positions by having the base iterator point one after the element. Reverse iterators for a strict sequence work fine with a forest, but a reverse iterator which supports <code>edge()</code> is trickier.

The next (or prior) edge is dependent on where you came from and it is a function of a vertex. So to read the edge when the iterator is after the current mark you need to <code>decrement</code> - but this means you are reading a vertex which is _before_ your offset range - this is because the edge is defined even on an end iterator (the vertex exists even if the property doesn't).

This violates our range just to read an <code>edge()</code> - to set an edge you defer the actual assignment of the edge until you reach the node - then set the edge to determine where to go next - so incrementing a reverse iterator looks like:

* Decrement the base iterator
* Set the edge to our next edge
* remember what the next edge should be

We run into a big problem if we set the edge and then decrement the reverse iterator - setting the edge may have pivoted on the previous node and we should land on a node which isn't reachable in one step from the current base!

The algorithm for decrementing the reverse iterator is

* Decrement the base iterator
* Set the edge to our next edge
* increment the base iterator ''twice''
* remember what the next edge should be

This all works on a forest, despite the range violations, because a forest is implemented as a loop structure and the prior vertex of any node is guaranteed to exist (that is, we have a <code>root()</code> as well as an <code>end()</code> and <code>--begin()</code> will yield <code>root()</code>).

A simpler form of a reverse iterator relies on the fact that the forest ranges are symmetrical. In that case <code>rend()</code> could return <code>root()</code> and we would scrap the whole off by one. This does not change the requirements of our reverse iterator at all, but for anyone familiar with STL reverse iterators it does change the behavior of <code>base()</code>.

### Final Resolution

The simpler form mentioned above was implemented and I've decided the simplifications are well worth it. The off-by-one issue of <code>base()</code> is resolved by simply having <code>base()</code> return <code>next(base_m)</code>. The requirement imposed by the reverse iterator is actually the same requirement imposed by <code>set_next()</code>. For any valid range <code>[first, last)</code> there must exist a valid range <code>(prior, last)</code> such that next(prior) == first.
