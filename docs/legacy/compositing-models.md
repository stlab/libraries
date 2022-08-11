---
title: Compositing Models
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Compositing_Models
---
A common point of confusion with the property model library (aka Adam) is that it is a mechanism for constructing user interfaces. That isn't the case. The property model library is a library for constructing models of how discrete properties interact. These properties might represent the parameters to a function (as is often the case in a dialog) or may be properties of the document itself.

Viewing the property model library as being part of the UI, leads one to attempt to construct a composite of layered models. Unfortunately, this doesn't work.

The key to the Model-View-Controller pattern is that it is, logically, a simple DAG:

{% include figure.md name='Composite_model_00.png' caption='_Model-View-Controller_' %}

Using the property model library to build the model has many advantages including:

* The property model is able to handle bidirectional relationships and solve based on a priority mechanism.
* The same model can be driven from a script or API with no UI attached.
* The model tracks contributing values which are useful for generating intelligent script which capture the user _intent_.
* The model tracks connected components (components which may contribute) and this information is useful to control the enable/disable state of widgets (a disconnected component cannot contribute so the controller is disabled).

The property model library does have some limitations including:

* Only handles discrete values, no mechanism for modeling sequences or other structures.
* Only handles many-to-one relationships. Currently there is only limited support for many-to-many relationships through self referencing structures.

There are times where the property model library is not the correct answer - before determining that the property model library is not the correct solution for your problem it is important that you understand what you are modeling. There are many cases where people dismiss the property model library because they think it is somehow _only_ a UI mechanism. Also, it is frequently the case the clients believe that they are dealing with a document model, when in fact they are dealing with a model of function parameters (sometimes _function_ is called _command_). Distinguishing the two cases is important. A simple test is to ask if any constraints in the model are in fact invariants on the model or, rather, constraints imposed as a mechanism to construct parameters to a function that will operate on the model.

Treating a property model as part of the UI will lead to attempts to composite the property model with some other model. It is very tempting to bind the UI to a sheet and then have a model treat the view interface on the sheet as a way to connect another model (treating the sheet as a controller) and connect back to the sheet through the controller interface (treating the sheet as a view). This leads to the following structure:

{% include figure.md name='Composite_model_01.png' caption='_Poorly Composited Models_' %}

Here "Model A" is the property model and "Model B" is some other model. The figure-eight links between the two create a problem. A request to change the model comes in from the controller to _A_. The view is notified of the change before _B_ gets a crack at the data - this is akin to an electrical short.

{% include figure.md name='Composite_model_02.png' caption='_Effect of Controller Request_' %}

Once _B_ gets the data, runs it through it's model and then attempts to notify the changes back,  we find that _B_ will be notified of the change it just made.

{% include figure.md name='Composite_model_03.png' caption='_Effect of View Update_' %}

This is the same problem we have with any widget library which has logical state. I'm frequently heard telling people that if their widget has a _get_ function then it is incorrect. The simplest solution to this problem is simply not to use _A_ at all, provide your own model, and don't use the property model library. Another solution, of course, is to only use the property model library. Because of the convenience of binding a layout to the property model library, however, some people still insist on attempting to composite models in this form (one side effect is an assert that fires in sheet_t::update() when it is re-entered. It may appear that the system would work without this assert, however, the first thing update() does is clears a set dirty flags - re-entering update() will give undefined behavior as to which views will be notified of the change that was in progress.

There is a supported way that models can be composited - the property model library allows any function to be plugged in to calculate a derived value, such a function can be arbitrarily complex and work on arbitrary data types. This can be used to effectively embed another model within the property model library:

{% include figure.md name='Composite_model_04.png' caption='_Using Functions to Embed Models_' %}

This structure allows you to keep all of the advantages of the property model library.

A number of clients would like to use the property model library simply as a [bredboard](https://en.wikipedia.org/wiki/Breadboard). This would effectively remove the property model from the system except as a connection point:

{% include figure.md name='Composite_model_05.png' caption='_Breadboard Model_' %}

There is currently no way to do this with the property model library. It would not be difficult to add - likely I would add a new cell type, _connection_, and an input/output API which would let you connect to the two halves of the cell. Input will always refer to information coming from the controller, output information going to the view. So a widget would set the input cell and another model would monitor the input cell, and set the output cell which a view would monitor. This would really just be a conduit and not have much at all to do with the property model library (these cells could not be referenced from the sheet) but would provide a breadboard to connect other models to.
