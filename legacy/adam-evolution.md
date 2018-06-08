---
title: Adam Evolution
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Adam_Evolution
---
#### Contents
{:.no_toc}
1. contents
{:toc}

This page is intended to document how the property model library (aka Adam) works, or in some cases is supposed to work.

A few notes on terminology:

property model library (aka Adam)
layout library (aka Eve)

The property model library is a system for modeling relationships between properties (simple key/values). These properties often represent parameters and related attributes of a function (this is the case when the library is used for a dialog box in an application) but can also be document properties (the case when used for a palette). The data structure known as a sheet (the name is borrowed from spreadsheet) is created to describe the properties and their relationships. The resulting model is used to assist the user in providing a valid set of parameters to a function or setting valid properties in a document. The model is independent of any specific UI, and serves equally well as the interface to a scripting system as to a graphical UI.

It is important to point out that the property model library doesn't solve all UI issues. The purpose of a user interface is to assist the user in selecting a function and providing a valid set of parameters to that function. The property model library is concerned with the later part of this, what it means to assist in providing a valid set of parameters to a function. The library only deals with a limited but common subset of the possible models.

A simple example to illustrate. Given a function declared like this:

<pre>void split_file(path source_file, path destination_directory, size_t size);</pre>

Result
: This function will split the source_file into ceil(size(source_file)/size) pieces, placing them into the destination_directory.

Given this description of the function we can begin to construct a sheet. We start with the basic parameters - I'll use the property model language to illustrate although this could be driven programmatically.

<pre>
sheet split_file
{
 input:
    source;
    destination;
    size: 4 * 1024; // default to 4K pieces
 output:
    result <== { source: source, destination: destination, size: size };
}
</pre>

At this point all we've done is collected the parameters to the function. Here we assume the source_file and destination_file is being provided from someplace. But what if they aren't valid? We can extend our model to ensure the preconditions for our function:

<pre>
sheet split_file
{
 input:
    source;
    destination;
    size: 4 * 1024; // default to 4K pieces
 output:
    result <== { source: source, destination: destination, size: size };
 invariant:
    valid_source <== source != empty;
    valid_destination <== destination != empty;
}
</pre>

Here are invariant checks are very simple, but Adam allows us to easily plug-in C++ functions so we could add a valid_path() function if we wanted to be more sophisticated in our model (Adam also has an easily extended type system).

If either invariant is violated, than any cells which contribute to the invariant are marked as "poison" and any cell derived from poisoned cells are marked as invalid. In this way if we had a GUI connected to this sheet, and an OK button bound to result then the OK button would disable unless we had a valid source and destination.

Invariants in our model are one way of expressing the functions pre-conditions.

At this point we are assuming that we are simply being handed all of the parameters at the start - parameters that can be adjusted are stored in interface cells. We'll move size to an interface cell. But we can do better than just letting the user adjust the size - in our description of our function there is a relationship between size, the size of the source file, and the number of files (count) in our result. This relationship is reversible between the size and count. We can express that in Adam as well:

<pre>
sheet split_file
{
 input:
    source;
    destination;
 interface:
    size: 4 * 1024; // default to 4K pieces
    count;
 logic:
    relate
    {
        count <== ceil(file_size(source)/size);
        size  <== ceil(file_size(source)/count);
    }
 output:
    result <== { source: source, destination: destination, size: size };
 invariant:
    valid_source <== source != empty;
    valid_destination <== destination != empty;
}
</pre>

We could connect a UI to this and the user could edit either size or count and the other value would be properly reflected. When the user clicked OK (which is bound to result) the application would receive a valid set of parameters for split_file. The application also could retrieve the values in the sheet which currently contribute to this for script recording. So if the user opts to create 10 files and let the system calculate the size, then you could record the count - and play it back on a new file to break that file into 10 pieces. Likewise, someone scripting this system could provide either count or size, and the system would behave correctly.

How does it work? The solver performs the following tasks:

1. (re)initialization
1. evaluate predicates for conditional relations
1. determine flow for active relations
1. evaluate invariants - marking any poison cells
1. evaluate output cells
1. evaluate any unresolved interface cells.

I'll talk a little about each -

(Re)initialization
: Initializers are evaluated at the time cells are added to the sheet (when used with the language parser, this is in the order they appear in the sheet. Initializers for interface cells are kept, and contributing values to the initializer are tracked. When input cells are set, those cells are marked as dirty and when reinitialize() is invoked on the sheet, any dependent initializers are evaluated. Note that initializers on non-interface cells are discarded. Also, an interface cell may have a value which is not calculated by it's initializer (such as a cell set by the user) after reinitialize if it wasn't dependent on a dirty input cell. When an interface cells value is set by an initializer, it's priority is also updated. There is no difference between using an initializer and programmatically setting the cell. Initializers provide a simple "spreadsheet like" declarative way to populate a model from a set of external properties.

Evaluate Predicates
: All predicates for conditional relations ("when" clauses) are evaluated. Cells contributing to a conditional relation contribute to all outputs. This is a conservative rule. A future version will track potentially connected components of relationships and only predicates of relate clauses which are potentially connected (would be connected if the predicate evaluated to true) to a cell which contributes to the output will contribute to the output. When clauses will also be decoupled from applying to a single relate clause, and will apply to a group of relate clauses. An "otherwise" construct will also be added. For example:

<pre>
when (p) {
    relate {
        x <== y;
        y <== x;
    }
    relate {
        z <== y;
        y <== z;
    }
} otherwise {
    relate {
        x <== z;
        z <== x;
    }
}
</pre>

Determine Flow for Active Relations
: For every active relate clause, exactly one of the terms must be executed in any state. To determine which term, we select the interface cell with the highest priority. Every interface cell has a unique priority, setting a cell raises the priority to the highest in the sheet. There is also an interface to touch cells which raises priority on a set of cells, without changing their relative order or values.] That cell becomes an anchor - it is currently evaluated immediately (and if it can't be fully evaluated, it is deferred and we tray again later). In the future it will not be evaluated immediately, only the flow will be marked.

Here there is a change in 1.0.13 from previous version, in previous versions determining the flow had no effect on existing priorities. In 1.0.13 the priorities are "rewritten" in the order the cells are evaluated in with all "roots" having the same relative order and all higher in priority then any derived cells. How this will happen will change in the future but the results should be the same. This change was done to prevent "back flow" of values when a conditional relation toggles (which could cause a cell to be derived from itself). This currently breaks the mark() function a bit, which remembers the current priority to filter out changes in contributing values for scripting. How mark() will work in the future is still a somewhat open issues (probably by capturing a complete list of contributing names).

Relate clauses can cause cycles in the system - if all cells involved in a relate clause are resolved, without evaluating the relate clause, then that relate clause is ignored and a diagnosed is issued to cerr on debug builds pointing out that this relationship wasn't necessary in the current state. It is possible to construct a system where the relation in the cycle which is being dropped is implied under certain circumstance - here it was determined that there needed to be a way to specify which relation in the cycle is dropped. The keyword "weak" was added to allow you to mark a weak relate clause - any weak relate clause is resolved after all non-weak relate clauses, and if all cells in it are resolved first, then it is ignored with no diagnostic.

### Other Subtopics

* [Adam Grammar Changes](adam-grammar-changes.html)

### Path Stability

_placeholder_
