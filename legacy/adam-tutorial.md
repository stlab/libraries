---
title: Adam Tutorial
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Adam_Tutorial
---
The following question came in Alan Shouls on the adobe-source forum:

<blockquote>
I am implementing one of our dialogs in A&E and have some problems defining
a relationship between one popup and two numeric fields.

The particular cluster that I am stuck on has a single popup with pre-set page
sizes - these sizes represent a particular height/width combination. The two
numeric fields can be populated by the popup but also allow the user to enter
custom values. The combination of UI elements should work so that if an pre-set
value is entered into the numeric fields the popup "magically" shows the correct
setting - otherwise it should show "custom".
</blockquote>

Foster Brereton correctly answered that this was a good use case for the new preset widget [include link here]. The design choice of when some logic belongs as part of the model and when as part of the UI is frequently difficult to answer - with the preset widget we are currently biasing the solution to this problem to the UI side (but that has some ramifications). The way the preset widget works is by effectively having a single widget which binds to multiple cells.

I thought it would be somewhat instructive to try and look at the structure of the problem from the view of the property model (aka Adam). The property model library provides a relatively simple but powerful computational structure. This small example actually covers nearly all aspects of the computational model. For this example, we'll ignore the fact that a widget could bind to multiple cells and assume that only single bindings were allowed. We know we are going to need to be able to bind the popup to a single cell so we'll start with a sheet with three cells:

<pre>
sheet preset_example
{
  interface:
    width : 5;
    height : 7;
    preset : [width, height];
}
</pre>

I filled in some initial values just to make it obvious. For this example we can see what is going on in the system by constructing a simple layout:

<pre>
layout preset_example
{
    view dialog()
    {
        edit_number(name: "Width:", bind: @width);
        edit_number(name: "Height:", bind: @height);
        popup(name: "Preset:", bind: @preset, items: [ { name: "5 x 7", value: [5, 7] }, { name: "4 x 6", value: [4, 6] } ] );
    }
}
</pre>

This will generate the following dialog:

{% include figure.md name='Adam_tutorial_01.png' caption='Initial Dialog' %}

At this point our preset cell is not connected to our width and height cells. We know that when we set the preset popup we want the value to propagate to width and height. We could express this as:

<pre>
sheet preset_example
{
  interface:
    width <== preset[0];
    height <== preset[1];
    preset : [5, 7];
}
</pre>

Here our edit_number fields in the view become disabled, this is because the value width and height are always being derived:

{% include figure.md name='Adam_tutorial_02.png' caption='Disabled Fields' %}

In spreadsheet terminology these are now equation cells and preset is our only value cell. We can draw the structure of this system as a DAG:

{% include figure.md name='Adam_tutorial_03.png' caption='DAG Structure' %}

The notion of a expressing computations as a DAG is central to property models. A property model is conceptually a set or predicates and the DAG gives us a deterministic way to satisfy those predicates. Here the predicates are "width is equal to preset[0] and height is equal to preset[1]".

Now we need a way to also say that width and height can effect the value of preset. We'll start with just width. We want to be able to write something like "width <=> preset[0]" but the property model doesn't (yet) have a notion of reversible logic, instead we have to manually unwrap this with a relate clause. The property model also can't handle a relationship to an expression (with good reason, in order to determine which direction this goes we need information which is only present in a cell) - so we need to name a cell. This leads to a dilemma:

<pre>
sheet preset_example
{
  interface:
    width;
    height <== preset[1];
    preset : [5, 7];
  logic:
    relate {
        width <== preset[0];
        preset <== [width, ????];
    }
}
</pre>

The relate clause binds a set of cells together. The rule for a relate clause is that only one term is executed for any state of the system. The property model library cannot distribute across multiple values to satisfy a predicate.

There are actually two choices for what to fill in for ????. Let's start with what appears to be the obvious choice:

<pre>
sheet preset_example
{
  interface:
    width;
    height <== preset[1];
    preset : [5, 7];
  logic:
    relate {
        width <== preset[0];
        preset <== [width, height];
    }
}
</pre>

If you try this, the system will "break" - if you look in the console after typing a value for width you will see the following diagnostic:

<pre>
(warning) relation unnecessary and ignored
00008: relate {
             ^
</pre>

It is instructive to look at the diagram for this system:

{% include figure.md name='Adam_tutorial_04.png' caption='Potential Cycle' %}

Here the circle represents the relationship and the lines without arrows are unresolved - they can flow one way or the other based on cell priority. What we've done is introduced a sort of cycle into the system. If we make preset a source, then the system resolves like so:

{% include figure.md name='Adam_tutorial_05.png' caption='No Cycle Here' %}

We use the double arrows to denote a resolved flow. However, if we try to make width be a source then the system will proceed as follows:

<pre>
width <== <source>;
preset <== [width, height];
   height <== preset[1];
       preset <== // FAIL - Preset is part of an unresolved relate clause.

// Select an additional source and retry.

width <== <source>;
preset <== <source>;
height <== preset[1];

// Success - but relate clause was ignored.
</pre>

The cycle failure can be viewed like this:

{% include figure.md name='Adam_tutorial_05a.png' caption='Bad Cycle Here' %}

Now, I had said that there were two possible answers for what to put into ????. To understand the second choice we need to understand a little bit about interface cells. To use spreadsheet terms, an interface cell can be either a value cell, or an equation cell depending on the state of the system (when it is attached to a relate clause). Normally, within a given state it cannot be used as both. This means that if I have a sequence of expressions like:

<pre>
a : 5;
b <== c;
c : 6 <== a;
</pre>

The is no ambiguity about the value of b; it is the value of c, which is the value of a which is 5. When b reads the value of c it always gets the "final" value of c, not the initial value. This is necessary to guarantee that the entire system is idempotent - that is, no matter how many times it is calculated it will return the same value. There is a single exception to this rule, when an expression on an interface cell is self referential then the self reference is assumed to refer to the initial state of that cell and the expression itself must be idempotent.

<pre>
a <== b;
b : 5.2 <== round(b);
</pre>

Here if round(b) referred to the final value of b, then we'd have cycle. So it must refer to the initial value of b and so the final value of b becomes 5 and a gets the final value of b, also 5. If in the relation "a <== b" we read the initial value of b, then a would be 5.2 the first time we updated, and 5 the second. The system would fail to be idempotent. So this rule only applies to self reference.

We can use this knowledge in our problem:

<pre>
sheet preset_example
{
  interface:
    width;
    height <== preset[1];
    preset : [5, 7];
  logic:
    relate {
        width <== preset[0];
        preset <== [width, preset[1]];
    }
}
</pre>

This gives us the following nice structure:

{% include figure.md name='Adam_tutorial_06.png' caption='Almost Done' %}

Finally, we can apply the same logic to height:

<pre>
sheet preset_example
{
  interface:
    width;
    height;
    preset : [5, 7];
  logic:
    relate {
        width <== preset[0];
        preset <== [width, preset[1]];
    }
    relate {
        height <== preset[1];
        preset <== [preset[0], height];
    }
}
</pre>

This system works as desired and the final structure is shown here:

{% include figure.md name='Adam_tutorial_07.png' caption='Complete System' %}

This system has essentially 3 states, either everything is derived from width, or from height, or from preset. The "tricky" bit is that as values flow through preset they pick up additional state. I continue to search for a more straightforward solution to such a problem (this is one of those problems that is a bit simpler to describe imperatively) but the property model solution is informative and hints at the capabilities of even such a simple computation model. I will also point out that the solution is remarkably similar to the inequality example.
