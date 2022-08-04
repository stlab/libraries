---
title: All About Binding
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/All_About_Binding
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Introduction
Within a layout description, <code>bind</code> arguments are used to specify cells within the sheet to bind the widget to. The value of a bind argument must be of type <code>name</code>. A common misconception with the <code>@cell</code> notation is that <code>@</code> is some kind of reference operator - it is not. The <code>@</code> operator is a quote operator which quotes a keyword or identifier and generate a <code>name</code>.

{% include figure.md name='Bind_01.png' caption='Model-View-Controller' %}

The sheet within ASL acts as a model in a Model-View-Controller pattern. A typical widget, such as a checkbox,  acts as a View-Controller.

{% include figure.md name='Bind_02.png' caption='`checkbox(bind: @cell)`' %}

In this configuration the widget is displaying the current value in the cell to which it is bound, and when clicked on, requests a change of the model to a new value. A control will be disabled if changing the value of the cell to which it is bound will have no effect on the output of the sheet.

Not all widgets comprise both a controller and view. A widget which simply displays a value is only a view, a piece of static text may be neither.

The usual configuration for a button is a bit different. A button binds to a cell as a view, even though it doesn't display the value. The value it binds to are typically the arguments to an action. Clicking on the button will invoke the action with the value from the cell to which the button is bound. A button will be disabled if the value in the cell to which it is bound is <i>invalid</i>. A cells value is invalid when it is derived from information contributing to an invariant in the sheet.

{% include figure.md name='Bind_03.png' caption='`button(bind: @cell, action: @do_it)`' %}

A button may also act as a controller. This is the equivalent of a button with an action that simply sets a cell within the sheet. The cell to set is specified with the <code>bind_output</code> argument. For example, let's say that you wanted a button which added 11 to the <code>value</code> cell. That could be expressed as:

<pre>
// -- in the sheet
interface:
    value: 1;
output:
    next_value <== value + 11;
// -- in the layout
    button(name: "Next Value", bind: @next_value, bind_output: value);
</pre>

{% include figure.md name='Bind_04.png' caption='`button(bind: @cell, bind_output: @another)`' %}

The button acts as the latch in the system to prevent this configuration from being an infinite loop.

The value of a button can also be specified with the <code>value</code> argument, rather than <code>bind</code>. For example, a button which reset the above value to 1 would look like:
<pre>
   button(name: "Reset", value: 1, bind_output: @value);
</pre>

Such a button acts strictly as a controller.
