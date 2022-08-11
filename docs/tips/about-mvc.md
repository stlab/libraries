---
title: How did MVC get so F’ed up?
layout: page
tags: [tips]
comments: true
---

Smalltalk MVC is defined in Design Pattern as:

> MVC Consists of three kinds of objects. The Model is the application object, the View is its screen presentation, and the Controller defines the way the user interface reacts to user input.[^gof]

However this definition has been abused over the years - Back in 2003 I gave a talk citing how bad Apple's definition was. At the time it stated:

>A view object knows how to display and possibly edit data from the application’s model… A controller object acts as the intermediary between the application’s view objects and its model objects… Controllers are often the least reusable objects in an application, but that’s acceptable…[^sfu]

Of course it isn't _acceptable_ and, over the years, Apple has refined their definition and now acknowledge the distinction between the _traditional_ Smalltalk version of MVC and the Cocoa version.[^objc] But the Cocoa version is still defined much as it was before:

>A view object knows how to display, and might allow users to edit, the data from the application’s model… A controller object acts as the intermediary between the application’s view objects and its model objects…[^objc]

In looking at how iOS applications are written the sentiment that controllers (and now view-controllers) are often the least reusable components in an application still flourishes, even if it is now unstated.

MVC (I'll always use that term to refer to the Smalltalk form) has the following structure:

{% include figure.md name='mvc-structure.png' caption='Smalltalk MVC[^wiki]' %}

Here the solid lines imply a direct association. And the dashed lines an indirect association by an observer. So what we see is that the model is unaware of the view and controller, except indirectly through notifications, and hence the code in the Model is reusable. The controller and view bind to the model, not the other way around.

Often the function of the Controller and View are tightly coupled into a "widget" or "control". When Apple talks about a View-Controller in their model they are talking about a grab-bag of an uber-widget that is a composite of UIView widgets and multiple models. From what I've seen, including in Apple's example code, it is usually a pretty big mess.

The key to getting MVC correct is understanding what models are. A model is simply an object[^eop] which can be _observed_ (a requirement for attaching views). For example, in ObjC an int is an object, but it is not observable. However, an ObjC object with an int property is observable using Key-Value Observing[^kvo].  A model may encapsulate complex relationships between the model’s properties. A trivial model is one where each property is completely independent (think C struct vs. C++ class). From a notification the view should be able to determine, at a minimum:

1. What changed. It may be as simple as "the model bound to the view".
2. The new value to display.

For example, let's say our model is a trivial observable boolean (I can't imagine a simpler model). What we want is a checkbox that binds to the observable boolean. When the controller requests a change in value, the boolean is updated, and the view is notified of the new state of the model. The model is unaware of what UI is attached to it, and in fact there could be multiple UIs, including something like a scripting system, attached to the same instance of the model. This is a form of data binding - though most data binding systems replicate the problems of their underlying widget set by treating the model as if it were observing the view, not the other way around.

Contrast this with most UI frameworks where you have a checkbox widget from which you can query the value and you receive a notification when the value has changed. This is pushing a model into the widget. With MVC you never ask a question like "what is the default state of this checkbox?" - the default state of the view is always the current state of the model. You would also never get the state of the checkbox - the state of the checkbox is simply a reflection of the state of the model. In a system where you get the state of a checkbox you are binding two models together by treating one as a view/controller of the other. Such a pattern doesn't scale beyond trivial models, and even for those it introduces some ambiguity.

I conjecture that one of the reasons why MVC has been so screwed up is because, unlike in Smalltalk, writing something as simple as an observable boolean is a bit of a pain in a language like Pascal or C. You quickly get into object ownership and lifetime issues and how to write bind expressions. If one also assumes that you have a 1:1 mapping from UI to model then there is some inherent inefficiency in the generalization. The Lisa team made some major compromises and the rest of the industry followed along.[^objui]

To support more complex views, the notification may need to specify what parts of the model changed and how those parts changed. For example, "image 58 was removed from the sequence". A complete model is one that can support any view of that model type efficiently (related to the notion of a complete type and a type's efficient basis).

One additional attribute of MVC is that it is a composite pattern. This is hinted at by the direct connection between the Controller and the View. As I said early, the view may contain state, this state is itself an object, and because this state is also displayed within the view it is observable. It is another model. I refers to this as the view's model. This model may include things such as the visibility of a window, the tab the user was last looking at, and the portion of the model being viewed.

Identifying what the models are in your system is important. We usually do pretty good at identifying the major models. Such as "this is an image" - but often fall short of identifying the complete model, i.e. "this is an image with a collection of settings." We end up with our model spread out within the code (an incidental type) and it makes it more difficult to deal with it.

A common model that is often completely overlooked is the model for function arguments. When you have a command, button, gesture, or menu item in your application, these are bound to a function. The function itself is not typically a zeroary function but rather has a set of arguments that are constructed through other parts of the UI. For example, if I have a list of images in my application, I might have a button to delete the selected images. Here the current selection is the argument to my delete command. To create a UI for the selection I must create a model of the arguments to my function. A precondition of delete is that the selection is not empty. This precondition must be observable in the argument model so it can be reflected in the view by disabling or hiding the button and in the controller be disallowing the user to click the button and issue the command. The same argument model can be shared for multiple commands within an application.

[^gof]:
    Gamma, Erich. "1.2 Design Patterns in Smalltalk MVC." Design Patterns: Elements of Reusable Object-Oriented Software. Reading, MA: Addison-Wesley, 1995. N. pag. Print.

[^sfu]:
    <http://smartfriends.com/U/Presenters/untangling_software.pdf> (Don't bother reading, this was an incomprehensible talk.)

[^objc]:
    <https://developer.apple.com/library/content/documentation/General/Conceptual/CocoaEncyclopedia/Model-View-Controller/Model-View-Controller.html>

[^wiki]:
    <https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller>

[^eop]:
    Stepanov, Alexander A., and Paul McJones. "1.3 Objects." Elements of Programming. Upper Saddle River, NJ: Addison-Wesley, 2009. N. pag. Print.

[^kvo]:
    <https://developer.apple.com/library/content/documentation/Cocoa/Conceptual/KeyValueObserving/KeyValueObserving.html>

[^objui]:
    <https://en.wikipedia.org/wiki/Object-oriented_user_interface>
