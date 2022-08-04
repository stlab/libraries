---
title: Layout Terminology
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Layout_Terminology
---
This is the layout behavior specifications for the default set of ASL widgets.

[Originally this document did not cover the attributes related to binding and the property model nor the semantics of the widget, but those are being added in.]

#### Contents
{:.no_toc}
1. contents
{:toc}

## Discussion

### Links vs. Buttons

[1/13/09 - sparent \<at\> adobe \<dot\> com]
The current description of a <code>link</code> in this document refers to the "chain link" indicator in the Ps Image Size dialog. The term <code>link</code> however has come to be used more commonly to refer to a hyper text link. I propose we change that name and add a hyper text link widget. From here forward in the discussion when I refer to a <code>link</code> I mean a hyper text link.

A <code>link</code> widget is similar in nature to a button except the semantics of the action are constrained to "go to location" where a location could be a dialog, sheet, document page, panel, anchor within a document page, or other 'visual' location. Traditionally in dialogs a button with an ellipses, i.e. (Auto...), implied a link to a dialog (later extended to a sheet). The ellipses also implied "more information needed" - however that distinction is not being consistently applied. The ellipses is also used in menu items with the same implication. A link widget should take a URI as an argument and we should settle on a standard URI format to link to application UI constructs (mostly dialogs).

I would like to see our UI designers weigh in with a rule set for when to use a <code>link</code> vs. a button with an ellipsis so we can gain some consistency (perhaps we could simply drop this use case for buttons). If the distinction cannot be made programmatically, then I propose we have an attribute on the <code>link</code> widget gives the button/ellipse appearance without the need to embed the ellipse in the string (giving greater flexibility to the UI designers in the future).

As we are building more of our own UI, some thought might be given to using a link representation in menus as well.

A good <code>display_text</code> widget would allow for embedded links (possibly using an xhtml representation for the display text).

### Subordinate Groups

[1/13/09 - sparent \<at\> adobe \<dot\> com]
A recent question arose regarding adding the ability to add <em>any</em> widget into the header of a <code>group</code> widget. There are cases within our UI where we have a checkbox and a popup and the idea was to generalize this to any widget. However, upon reflection the implied semantics of a widget in the header of a group are that the items within the group are subordinates to the item in the header. The only widgets for which I can envision such a subordinate relationship are those that select from a fixed set of values, specifically, checkbox, radio_button, and popup.

For a checkbox, the implication is that the subordinate items only apply when the checkbox is in it's positive state (and would be disabled otherwise). For a radio_button, the implication is that the subordinate items only apply when the state associated with the radio button is selected. For a popup, the implication is that there are multiple panels of subordinate  items and the items on non-visible panels will be disabled (with the items on the current panel being enabled for the current popup selection). This distinguishes the popup group case from the tab panel case where there is no implication that the tabs are making any "choice" other than selecting which view to display.

My proposed solution is that we allow checkbox, radio_button and popup to be used as containers:

<pre>
checkbox(name: "Use File Size:") {
    edit_number();
}
</pre>

(The proper enabled/disabled behavior would still fall out of the property model - this just allows for a visual clustering). A smart implementation may choose to display such a group that only contains a single element as a row instead of a group box.

## CEL Primer

Values expressed in this document are specified using the Common Expression Language (CEL). For a more thorough introduction to the common expression language please visit [http://stlab.adobe.com/group__expression__reference.html the CEL expression reference documentation]. Here is a quick overview of the types available in the CEL and how they are formatted:

{:auto_ids}
empty
: a nil-like value. Intended to mean _nothing_.
: e.g., `{ my_brain: empty }`

integer
: see [number](#number).

float
: see [number](#number).

number
: Stored as double. Format is simply the number
: e.g., 42 or 19.2

string
: Stored as an adobe::string_t. Format is a sequence of characters encase in single- or double-quotes.
: e.g., "string" or 'string'.
: Strings placed together concat into a single string.
: e.g., "this" 'is' "one" "'string'"
: is the same as "this is one 'string'"

name
: Stored as an adobe::name_t. Format is a sequence of letters, digits, and '_' prepended with '@'.
e.g., @width or @forty_two

boolean
: Stored as a bool. Either <code>true</code> or <code>false</code>.

array
: Stored as an adobe::array_t. A homogeneous, sequential collection of values wrapped in braces.
e.g. [ 1, 2, 3 ] or [ @first_item, "second item", 3.14, 'fourth item' ]

dictionary
: Stored as an adobe::dictionary_t. A collection of associated key-value pairs wrapped in brackets. The keys are always of type Identifier (adobe::name_t), though they are _not_ prepended with the '@' (it is implied).
: e.g, { name: @value, name2: "second value" } or { spacing: 42, placement: place_overlay, name: "My Dictionary" }

enumeration
: [ no description ]

regular
: [ no description ]

### Other Types
The type system for CEL is open. Values are stored in an adobe::any_regular_t which can hold any regular type. Client functions can be introduced that return values of types other than those listed and other functions may process those value.

### Predefined Constants
There are a number of predefined constants in the layout library - unless otherwise specified the value of the constant is not defined:
* align_left
* align_right
* align_center
* place_row
* place_column
* place_overview

The following constants should be defined with values based upon their context within a layout. For example, the value of space_normal should be less inside of a palette or message box then inside of a dialog.

* space_grouped
* space_normal
* space_unrelated

The value of space_grouped should match the spacing defined within the widget set between a widget label and field. For example:

<pre>
row(spacing: space_grouped) {
    static_text(name: "Label:"); edit_text();
}
</pre>

should appear identical to:

<pre>
edit_text(name: "Label:");
</pre>

[ The space_ variables are not currently defined in the Begin application. ]

## Associated Structures

These data structures are used by some of the widgets described below. They are available as utility structures to any widget that would like to use them. Note that a widget is not required to use all portions of a structure; it may use just what it requires.

### unit_t

unit_t stores attributes pertaining to a specific numerical unit. An example of a numerical unit may be _inches_ or _degrees Celsius_. Each of the values in this dictionary can be interpreted by the implementation of a widget as the widget sees fit. unit_t is described in CEL as a dictionary, and can have the following keys:

#### Attributes

name
: type: [string](#string)
: default: ""
: The visually displayed name of the unit

bind
: type: [name](#name)
: default: name_t()
: Distinguishes the common underlying unit for this unit. An example of this would be lengths displayed in inches, centimeters, mm, picas, etc. Though they are different units they all, by linear transformation (see scale, below), devolve to a base unit which the user can choose. This identifier is used to group different units together that have the same base unit. This allows for a single collection of units to describe values in different bases (For example, bases of unit length and unit percent.)

format
: type: [string](#string)
: default: "#"
: Specifies the numeric formatting for this unit. Allows for printf-like formatting for the result of the transformation. Examples "#" (integer values), "#.00" (double-precision floating point values).

increment
: type: [float](#float)
: default: 1
: The amount this value is to change when the user 'bumps' it (e.g., by scrolling the scroll wheel or using the arrow keys on the keyboard.)

min_value
: type: [float](#float)
: default: <code>std::numeric_limits<double>::min()</code>
: The lower bound that numbers in this particular unit are allowed to attain. The default value is intended to imply ignoring the value.

max_value
: type: [float](#float)
: default: <code>std::numeric_limits<double>::max()</code>
: The upper bound that numbers in this particular unit are allowed to attain. The default value is intended to imply ignoring the value.

scale
: type: [array](#array) of two [float](#float)s
: default: [ 1, 0 ] (identity transformation)
: This is the linear transformation used to go from this-unit to the base-unit. The first number is the slope of the linear transformation (m). The second number is the y-intercept of the transformation (b). A default of [ 1, 0 ] results in an identity transformation for the unit values.

#### Examples in CEL

<pre>
units: [
    { name: "fahrenheit", bind: @celsius, scale: [ 1.8, 32 ] },
    { name: "celsius", bind: @celsius },
    { name: "kelvin", bind: @celsius, scale: [ 1, 273.15 ]  }
]

units: [
    { name: 'percent', bind: @width_percent, format: "#.00" },
    { name: 'inches', bind: @width_inches, format: "#.000" },
    { name: 'cm', bind: @width_inches, scale: [ 2.54, 0 ], format: "#.00" },
    { name: 'mm', bind: @width_inches, scale: [ 25.4, 0 ], format: "#.0" },
    { name: 'points', bind: @width_inches, scale: [ 72, 0 ], format: "#.0" },
    { name: 'picas', bind: @width_inches, scale: [ 6, 0 ], format: "#.00" },
    { name: 'columns', bind: @width_inches, scale: [ 1, 0 ], format: "#.000" }
]
</pre>

### value_range_format_t

This is a simple filter for limiting a value to one within a range of values. The general notion of a filter should describe a set of valid values (potentially infinite - such as all positive numbers).

#### Attributes

first
: type: [float](#float)
: default: 0
: Specifies the first value this range can have

last
: type: [float](#float)
: default: 0
: Specifies the last value this range can have

interval
: type: [float](#float)
: default: 0
: Specifies the increment/decrement count to get between first and last. In ASL widget implementations if this value is omitted it will default to <code>last - first</code> (which is actually pretty useless).

#### Examples in CEL

<pre>
format: { first: 0, last: 100, interval_count: 50 }
</pre>

## General Layout Attributes

The layout engine ("Eve") does not know the difference between a widget destined to be a container (e.g., a window or a panel) versus one that is a leaf node (e.g., a button or a label). To the layout engine everything is a "box" with various attributes. In theory, then, all of the general attributes below could pertain to all of the widgets. Of course, this is nonsensical in some cases (e.g., spacing for a label), but the layout engine does not care, because it does not know that a "label" is a leaf widget any more than it knows a "group" is not (though it actually may be if it has no children). From an implementation standpoint, _all_ nodes in the layout hierarchy have _all_ these attributes, even if some are never used (or make no sense).

Default values in this section are suggestions of sorts. It is possible (and entirely likely) that the layout semantics of various widgets will require a different default for an attribute. For example, the default horizontal alignment of a container is align_default, but that value is changed to align_fill in the case of a panel widget. Overrides of this nature are explicitly iterated in the section on [Widget-Specific Layout Attributes](#widget-specific-layout-attributes) under "Layout Attribute Defaults".

* Code for the layout attributes can be found in <adobe/layout_attributes.hpp>.
* Code for the extents can be found in <adobe/extents.hpp>.

### Layout Attributes

{:auto_ids}
indent
: type: [integer](#integer)
: default: 0
: The amount of space between the leftmost (or topmost) widget and the end of the left (or top) margin, in pixels.

create
: type: [boolean](#boolean)
: default: true
: Determines whether or not this node actually creates a widget using a platform widget implementation. Examples of noncreating containers are row, column, and overlay. This attribute is not settable from within the layout description.

spacing
: type: [integer](#integer) or [array](#array)
: default: 10
: The amount of space to be put between multiple children of the container. This value can also contain an array, which will be iterated in turn to determine the spacing between two child widgets. Example: spacing: [ 5, 10, 15 ] will yield a container with 5 pixels between child widget 1 and 2, 10 pixels between child widget 2 and 3, and so forth.

placement
: type: [enumeration](#enumeration)
: default: place_leaf
: The placement of a container's children. Options are:

    place_row
    : Align the children in a row

    place_column
    : Align the children in a column

    place_overlay
    : Align the children in an overlay. An overlay will cause all children to have horizontal and vertical alignments of align_fill. Each child will share the same layout real estate with its siblings. The idea for an container placing its children using place_overlay is so that only one will be visible at a time.

: _question_ : What does place_leaf do?

margin
: type: [integer](#integer) or [array](#array)
: default: [ 0, 0, 0, 0 ]
: margin is the amount of space from the frame of the container to the children within. The margins of a container can only be encroached upon by the outset of a child widget. A child widget's outset is not allowed to be larger than its container's margin (implying the outset of the child would extend into and possibly beyond the frame of the container.) If the container has no margin, then the outset of the child widgets will propagate to become the outsets of the parent container. If this attribute is specified using a single value (e.g., margin: 20) it implies the value should be used for all the container's margins.

horizontal
: type: [enumeration](#enumeration)
: default: align_default
: Specifies horizontal alignment of the widget. Options are:

    align_forward
    : Forward alignment

    align_reverse
    : Reverse alignment

    align_center
    : Center alignment

    align_proportional
    : Distributes leftover container space equally among widgets with this alignment specified (_todo_: verify)

    align_forward_fill
    : Distributes leftover container space completely to widgets with this alignment specified (_todo_: verify)

    align_reverse_fill
    : Distributes leftover container space completely to widgets with this alignment specified (_todo_: verify)

    align_default
    : Aligns items from left-to-right in the case of a container with placement place_row, and top-to-bottom in the case of a container with placement place_column.

    align_fill
    : same as align_forward_fill

    align_left_fill
    : same as align_forward_fill

    align_right_fill
    : same as align_reverse_fill

    align_top_fill
    : same as align_forward_fill

    align_bottom_fill
    : same as align_reverse_fill

    align_left
    : same as align_forward

    align_right
    : same as align_reverse

    align_top
    : same as align_forward

    align_bottom
    : same as align_reverse

vertical
: type: [enumeration](#enumeration)
: default: align_default
: Specifies vertical alignment of the widget. Options are the same as the horizontal attribute.

child_horizontal
: type: [enumeration](#enumeration)
: default: align_default
: Unless otherwise specified by the child widget itself, this specifies the horizontal alignment of the children of this widget. Options are the same as the horizontal attribute.

child_vertical
: type: [enumeration](#enumeration)
: default: align_default
: Unless otherwise specified by the child widget itself, this specifies the vetical alignment of the children of this widget. Options are the same as the horizontal attribute.

{:#guide_mask} guide_mask <!-- auto_ids not working? -->
: type: [array](#array) of [enumeration](#enumeration)s
: default: [ ]
: guides to suppress - current valid values are:

    guide_baseline
    :

    guide_label
    :

size
: type: [enumeration](#enumeration)
: default: size_normal
: Specifies the size of the widget. Valid values are:

    size_mini
    : For palettes

    size_small
    : For palettes and dialogs (in rare cases)

    size_normal
    : For dialogs

### Widget Extents

Generally the widget extents are not settable through a layout description - but instead are obtained by a call to the widget with the attributes supplied by the description. The terminology is documented here so that in cases where it does make sense to expose through a layout description it is done so consistently.

outset
: type: [array](#array)
: default: [ 0, 0, 0, 0 ]

frame
: type: [array](#array)
: default: [ 0, 0, 0, 0 ]
[top, left, bottom, right] in coordinate space of parent where top left of parent is 0, 0.

inset
: type: [array](#array)
: default: [ 0, 0, 0, 0 ]

guide_set
: type: [array](#array)
: default: [ ]

width
: type: [integer](#integer)
: default: 0
: Specifies the minimum width requirement for this widget

height
: type: [integer](#integer)
: default: 0
: Specifies the minimum height requirement for this widget

## Widget-Specific Layout Attributes

### Containers

A _container_ is a widget-context term: as previously mentioned, it really has no meaning as far as the layout engine is concerned. That being said, it is very helpful as a user interface designer to separate those widgets intending to contain child widgets from those that are meant to be leaf nodes. No container should be required to have any children, though for some of them it would be a poor UI decision to do so (e.g., a tab_group with no child panels would not be much of a tab group.)

#### group

##### Layout Attribute Defaults

* [placement](#placement): place_column
* [margin](#margin): 10

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: Specifies the label to be put at the top of the group frame

alt
: type: [string](#string)
: default : ""
: Specifies the tooltip text for the widget

##### Examples in CEL

<pre>
group(placement: place_row, name: "Horizontal Checkboxes", child_horizontal: align_fill)
{
    // ...
}
</pre>

#### optional

##### Layout Attribute Defaults

* [placement](#placement): place_column
* [horizontal](#horizontal): align_fill
* [vertical](#vertical): align_fill
* [guide_mask](#guide_mask): [ guide_baseline ]

##### Additional Attributes

value
: type: [regular](#regular)
: default: [empty](#empty)
: When the bound cell is set to this value, the optional panel becomes visible

##### Examples in CEL

<pre>
optional(bind: @valid_bounds, value: false)
{
    static_text(name: "Error: The image dimensions you have specified are invalid. Please correct
                them", characters: 10);
}
</pre>

#### panel

##### Layout Attribute Defaults

* [placement](#placement): place_column
* [horizontal](#horizontal): align_fill
* [vertical](#vertical): align_fill
* [guide_mask](#guide_mask): [ guide_baseline ]

##### Additional Attributes

value
: type: [regular](#regular)
: default: [empty](#empty)
: When the bound cell is set to this value, the optional panel becomes visible

##### Examples in CEL

<pre>
panel(bind: @colorMode, value: @labColorMode, horizontal: align_fill)
{
    // ...
}
</pre>

#### tab_group

##### Layout Attribute Defaults

* [placement](#placement): place_overlay
* [guide_mask](#guide_mask): [ ]
* [margin](#margin): 10

##### Additional Attributes

items
: type: [array](#array) of [dictionaries](#dictionary)
: default: [ ]
: Describes the tabs of the tab group. Each tab is described using a dictionary with the following keys:

    name
    : type: [string](#string)
    : default: ""
    : The visually displayed name of the tab

    value
    : type: [regular](#regular)
    : default: [empty](#empty)
    : When the bound cell to the tab group is set to this value, this particular tab becomes selected. Note that this does not hide/show the children of the tab group explicitly. Rather, in order to hide/show children properly, they must be contained within a panel bound to the same cell, and revealing itself by the same regular value.

##### Examples in CEL

<pre>
tab_group(vertical: align_fill, horizontal: align_fill,
          items: [
              { name: "Sources", value: @sources },
              { name: "Property Model", value: @model_inspector },
              { name: "Layout Sheet", value: @layout_inspector },
              { name: "Results", value: @results },
              { name: "Errors", value: @errors }
          ])
{
    // ...
}
</pre>

#### dialog

##### Layout Attribute Defaults

* [placement](#placement): place_row
* [margin](#margin): 10
* [spacing](#spacing): 20

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the window to be shown in the its title bar

grow
: [boolean](#boolean)
: default: false
: Whether or not this window should have a grow box and is resizeable

metal
: [boolean](#boolean)
: default: false
: Whether or not this window shows up in the 'brushed metal' look
: _Note_: This is for Mac OS X only; other OSes ignore this value.

##### Examples in CEL

<pre>
dialog(name: "Editor", size: @size_small, placement: place_column,
       grow: true, horizontal: align_fill, vertical: align_fill)
{
    // ...
}
</pre>

### Non-Creating Containers

Non-creating containers are unique from regular containers in that they produce no platform-specific widget upon time of creation. They are legitimate nodes in the layout engine tree, and can contain other nodes and be contained themselves. However, when the time comes to create the widgets within the layout, these elements propagate information up and down the layout, but do nothing in and of themselves.

By implication, calling adobe::measure and adobe::place for these "widgets" are no-ops.

#### row

##### Layout Attribute Defaults

* [placement](#placement): place_row
* [create](#create): false
* [guide_mask](#guide_mask) : [ guide_baseline ]

##### Additional Attributes

none.

#### column

##### Layout Attribute Defaults

* [placement](#placement): place_column
* [create](#create): false
* [guide_mask](#guide_mask) : [ guide_label ]

##### Additional Attributes

none.

#### overlay

##### Layout Attribute Defaults

* [placement](#placement): place_overlay
* [create](#create): false

##### Additional Attributes

none.

### Leaf Widgets

#### button

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the widget.

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

default
: type: [boolean](#boolean)
: default: false
: Specifies whether or not this button should behave as the default button (including any necessary visual cues.)

cancel
: type: [boolean](#boolean)
: default: false
: Specifies whether or not this button should behave as the cancel button (including any necessary visual cues.)

items
: type: [array](#array) of [dictionaries](#dictionary)
: default: [ ]
: Describes the various visual states this button can turn to, based on the modifier keys that are held down by the user. Each item is described using a dictionary with the following keys:

    name
    : type: [string](#string)
    : default: ""
    : The name of the widget.

    alt
    : type: [string](#string)
    : default: ""
    : The text to be used for the widget's tooltip.

    modifiers
    : type: [name](#name) or [array](#array) of [name](#name)s
    : valid values: @option, @command, @control
    : The necessary modifiers must be depressed by the user (either one or many, specified using an [array](#array)) in order for this state to be shown.

##### Examples in CEL

<pre>
button(items: [
           { name: "Evaluate" },
           { name: "Dump", modifiers: @option },
           { name: "Version", modifiers: @command }
       ]);
</pre>

#### checkbox

_(Note: move this to the semantics doc)_ Checkboxes are _not_ boolean (flag) widgets. There is a third, undetermined state for a checkbox, which is commonly seen as a dash running through the middle of the checkbox itself. When the value of the cell to which the checkbox is bound matches neither value_on or value_off, the checkbox should be seen in this third, undetermined state.

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the widget.

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

value_on
: type: [regular](#regular)
: default: true
: When the value of the cell to which this widget is bound changes to this value, the checkbox is checked.

value_off
: type: [regular](#regular)
: default: false
: When the value of the cell to which this widget is bound changes to this value, the checkbox is unchecked.

##### Examples in CEL

<pre>
checkbox(name: "Whizzy Checkbox", alt: "This is my tooltip info");
checkbox(name: "Make Mixed Control", value_on: @mixed, value_off: false);
</pre>

#### display_number

_Note:_ There is a design flaw with the display_number implementation. It would be nice if the user were not required to specify a unit array for the display_number, but a problem arises in that the one and only unit thus created will use the name parameter for the display_number as the unit's name. This will cause a duplication of the name of the widget as the unit's label itself, for example: <code>Inches: 5 Inches:</code>

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the widget.

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

characters
: type: [integer](#integer)
: default: 5
: Used to specify the approximate width of the numeric value of the display_number widget. Added to this value are the length of the widget label and the width of the longest unit label.

units
: type: [array](#array) of [dictionaries](#dictionary)
: default: [ ]
: Describes the units to be used when formatting the number coming in from the property model. This array should contain a collection of unit_t dictionaries; please see the [unit_t](#unit_t) documentation for parameter information. Note that display_number collects defaults for all unit values from the parameter set for the widget itself. For example, if <code>format</code> is defined as a parameter to this widget and is not otherwise specified by a unit, the unit will use the value found in the <code>format</code> parameter as its <code>format</code> value. This avoids unnecessary description duplication.

##### Examples in CEL

<pre>
display_number(name: "File Size:", bind: @bytes, format: "#.##",
               units: [
                   { name: "bytes", format: "#", scale: [ 1, 0 ] },
                   { name: "KB", format: "#.###", scale: [ 1024, 0 ] },
                   { name: "MB", scale: [ 1024 * 1024, 0 ] },
                   { name: "GB", scale: [ 1024 * 1024 * 1024, 0 ]  },
                   { name: "TB", scale: [ 1024 * 1024 * 1024 * 1024, 0 ]  }
               ]);
</pre>

#### edit_number

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the widget.

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

digits
: type: [integer](#integer)
: default: 10
: Used to specify the approximate width of the numeric value (the edit text portion) of this widget.

max_digits
: type: [integer](#integer)
: default: 0
: Used to specify the maximum digits allowed for the numeric value (the edit text portion) of this widget. The default of 0 implies unlimited length.

units
: type: [array](#array) of [dictionaries](#dictionary)
: default: [ ]
: Describes the units to be used when formatting the number coming in from the property model. This array should contain a collection of unit_t dictionaries; please see the [unit_t](#unit_t) documentation for parameter information. Note that edit_number collects defaults for all unit values from the parameter set for the widget itself. For example, if <code>format</code> is defined as a parameter to this widget and is not otherwise specified by a unit, the unit will use the value found in the <code>format</code> parameter as its <code>format</code> value. This avoids unnecessary description duplication.

##### Examples in CEL

<pre>
edit_number(name: "Temperature:", format: "#.##", min_value: -273.15,
            units: [
                { name: "fahrenheit", bind: @celsius, scale: [ 1.8, 32 ] },
                { name: "celsius", bind: @celsius },
                { name: "kelvin", bind: @celsius, scale: [ 1, 273.15 ]  }
            ]);
</pre>

#### edit_text

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the widget.

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

characters
: type: [integer](#integer)
: default: 10
: Used to specify the approximate width of the edit text portion of this widget. Added to this length is the length of the name of the widget to get the minimum width required for this widget.

max_characters
: type: [integer](#integer)
: default: 0
: Used to specify the maximum width of the edit text portion of this widget. The default of 0 implies unlimited length. Only relevant when lines is 1.

lines
: type: [integer](#integer)
: default: 1
: Specifies the height of the widget in number of lines available in the edit text field.

monospaced
: type: [boolean](#boolean)
: default: false
: Specifies the use of a monospaced font for this edit text field. Deprecated.

scrollable
: type: [boolean](#boolean)
: default: false
: Specifies whether or not the edit text field should be scrollable. Only relevant when lines is greater than 1.

password
: type: [boolean](#boolean)
: default: false
: Specifies whether or not the edit text field should hide the actual text within the widget.

##### Examples in CEL

<pre>
edit_text(name: "Expression:", characters: 10, horizontal: align_fill);
</pre>

#### image

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

image
: type: [string](#string)
: default: ""
: File name of the image to be placed in the widget. The path(s) used for finding this file vary based on how the client specifies resource paths with <code>adobe/future/resources.hpp</code>.

##### Examples in CEL

<pre>
image(image: "stop.tga");
</pre>

#### label

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the widget.

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

characters
: type: [integer](#integer)
: default: 0
: Specifies the maximum width of the label (in approximate characters). When the label's content exceeds this width, the label will wrap, automatically increasing its height, so the entire label's contents are visible.

##### Examples in CEL

<pre>
label(name: "Unfortunately, something drastic has happened. If you would like we can try to
            continue with the operation, but there is a chance you will blow up your computer. Would
            you like to try?", characters: 25);
</pre>

#### link

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

count
: type: [integer](#integer)
: default: 0
: The number of horizontal 'link lines' to be displayed by this widget. Each link line will automatically attach itself to the next available baseline guide that can be found.

value_on
: type: [regular](#regular)
: default: true
: When the value of the cell to which this widget is bound changes to this value, the checkbox is checked.

value_off
: type: [regular](#regular)
: default: false
: When the value of the cell to which this widget is bound changes to this value, the checkbox is unchecked.

##### Examples in CEL

<pre>
link(count: 3, value_on: false, value_off: true);
</pre>

#### list

_Note:_ The list widget is still in its infancy. Expect this documentation to change.

##### Layout Attribute Defaults

* [horizontal](#horizontal): align_fill
* [vertical](#vertical): align_fill

##### Additional Attributes

column_set
: type: [array](#array) of [dictionaries](#dictionary)
: default: [ ]
: Describes the columns to be used in the list. Each column is described using a dictionary with the following keys:

:; name
:: type: [string](#string)
:: default: ""
:: The visually displayed name of the column

:; id
:: type: [name](#name)
:: default: name_t()
:: Specifies the key to be used to extract data from the model for this particular column

:; selectable
:: type: [boolean](#boolean)
:: default: false
:: Specifies whether or not the data in this column can be selected by the user

:; mutable
:: type: [boolean](#boolean)
:: default: false
:: Specifies whether or not the data in this column can be edited by the user

##### Examples in CEL

<pre>
list(column_set: [
         { id: @name, name: "Name", selectable: true, mutable: true },
         { id: @value, name: "Value", selectable: true, mutable: true }
     ]);
</pre>

#### popup

The custom meta-item is never explicitly selectable by the user. The only way it can be shown is if the user modifies the value bound to the popup by some other means for which the popup has no corresponding menu item.

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the widget.

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

custom_item_name
: type: [string](#string)
: default: "Custom"
: When the value to which this popup is bound does not match any of the possible values in the popup, the 'Custom' meta-item is shown. This string is the actual text used in the display of the custom meta-item.

items
: type: [name](#name) or [array](#array) of [dictionaries](#dictionary)
: default: [ ]
: Describes the items to be listed in the popup. If a [name](#name) is specified then the popup items are bound to an output of the data model. The value bound to should contain an [array](#array) of [dictionaries](#dictionary) as described here. [Note: binding items is currently not implemented in the APL widget set.] Each [dictionary](#dictionary) in the [array](#array) has the following keys:

:; name
:: type: [string](#string)
:: default: ""
:: The visually displayed name of the item

:; value
:: type: [regular](#regular)
:: default: [empty](#empty)
:: When the bound cell to the popup is set to this value, this particular item becomes selected.

##### Examples in CEL

<pre>
popup(name: "Select One:", custom_item_name: "Not Applicable",
      items: [
            { name: "Eeny", value: "Eeny" },
            { name: "Meeny", value: "Meeny" },
            { name: "Meiny", value: "Meiny" },
            { name: "Moe", value: "Moe" }
         ]);
</pre>

#### presets

_Note:_ This is one nasty widget. The reason it is so is because it does not follow the conventions of a widget (typically) being a single view/controller into the model. In the case of a preset widget, it is a view/controller into an arbitrary number of values in the property model. As such, extra levels of setup and hook points are required.

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the widget. Note that this name is never seen visually; rather, it is used as the preset file name when presets are saved/loaded from disk.

domain
: type: [string](#string)
: default: ""
: The domain of the widget, typically the name of the application in which this preset widget resides. Note that this name is never seen visually; rather, it is used as the directory that will contain all the preset widget data files for this application.

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

additional_preset_set
: type: [array](#array) of [dictionaries](#dictionary)
: default: [ ]
: Sometimes the client application will want to add runtime 'presets' that are not really presets but give the user the ability to copy state already present within the application. Examples of this may be a current list of the open documents and their dimensions. Each item is described using a dictionary with the following keys:

:; name
:: type: [string](#string)
:: default: ""
:: The visually displayed category name of the preset set (e.g., "Open Documents")

:; items
:: type: [array](#array) of [dictionaries](#dictionary)
:: default: [ ]
:: Describes the actual presets to be listed in this preset category. Each item is described using a dictionary with the following keys:

::; name
::: type: [string](#string)
::: default: ""
::: The visually displayed name of the preset (e.g., "4x6 Portrait")

::; value
::: type: [dictionary](#dictionary)
::: default: { }
::: Describes the actual values contained within the preset. The keys in the preset's value must have a one-to-one correspondence to keys found in the current property model. The value associated with those keys are the values to be imbued upon the property model at the time this preset is selected by the user.

bind_output
: type: [name](#name)
: default: name_t()
: Specifies the feedback cell to go from the property model to the preset. This cell should contain an aggregation of property model cell names and their values as key/value pairs of a dictionary. When a value changes in the model and the preset widget wants to know about the change, the cell named here should have an updated dictionary with the new value for the modified cell.

bind
: type: [array](#array) of [array](#array)s
: default: [ ]
: One of the functions available through the preset widget is the ability to add new presets. When this is done a dialog is presented to the user where they can specify a name for the preset. The user, in that dialog, will also be able to specify exactly which of the property model values they would like to have included in the preset. This parameter allows for the specification of which property model values are allowed in a preset. Specifying an array of identifiers under a single label allows for related values to be added/omitted as a group from the preset.
: Please see the CEL example for an example of how this parameter is specified.

localization_set
: type: [dictionary](#dictionary)
: default: { }
: The preset widget has subdialogs of its own. In order to allow for the client application to override the string values within these widgets, this dictionary can be supplied to the widget. There are a set number of keys for which strings are expected; if the key/value pair is not supplied in the dictionary, a default (English) string will be used.
: Please see the CEL example for a list of keys (and possible string values).

##### Examples in CEL

<pre>
preset(name: "new_document",
       domain: 'MyApplication',
       alt: "Modify the presets available for this command",
       bind_additional: @additional_preset_set,
       bind_output: @preset_feedback,
       bind: [
                [ [ @resolution, @resolution_indirect ], "Resolution" ],
                [ @background, "Background Contents" ],
                [ @colorMode, "Color Mode" ],
                [ [ @gray_profile, @rgb_profile, @cmyk_profile ], "Color Profile" ],
                [ @bitDepth, "Bit Depth" ],
                [ @aspectRatio, "Pixel Aspect Ratio" ],
                [ [ @pixelWidth, @width_inches, @pixelHeight, @height_inches ], "Dimensions" ]
             ],
       localization_set: {
            add_dialog_group_name: "Include in Preset",
            add_dialog_message: "Choose which values in the dialog you would like to add to the preset you are creating.",
            add_dialog_name: "Add A Preset",
            add_subdialog_cancel_button_alt_text: "Do not add a preset",
            add_subdialog_default_preset_name: "My Preset",
            add_subdialog_ok_button_alt_text: "Add a preset with the current settings",
            category_popup_alt_text: "Select a category of presets from the list",
            category_popup_name: "Category:",
            custom_category_name: "Custom",
            delete_dialog_message: "Choose a preset to delete. This action cannnot be undone.",
            delete_dialog_name: "Delete A Preset",
            delete_subdialog_cancel_button_alt_text: "Do not delete a preset",
            delete_subdialog_ok_button_alt_text: "Delete the currently selected preset",
            file_extension: ".presets.cel",
            menu_item_add_preset: "Add Preset...",
            menu_item_append_preset: "Append Presets...",
            menu_item_delete_preset: "Delete Preset...",
            preset_popup_alt_text: "Select a preset from the list to populate the dialog",
            preset_popup_name: "Preset:",
            subdialog_cancel_button_name: "Cancel",
            subdialog_ok_button_name: "OK",
            top_folder_name: "Adobe",
            user_preset_list_empty_warning: "The user preset list is empty",
            user_presets_category_name: "User Presets"
         });
</pre>

#### progress_bar

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

is_indeterminate
: type: [boolean](#boolean)
: default: false
: Specifies whether or not this progress bar has the "barber pole" spinning style.

is_relevance
: type: [boolean](#boolean)
: default: false
: Specifies whether or not this progress bar is a relevance bar.
: _Note_: This is option is ignored on every platform but Mac OS X.

orientation
: type: [name](#name)
: valid values: @vertical, @horizontal
: default: @horizontal
: Specifies the orientation of the progress bar

format
: type: [dictionary](#dictionary)
: A range of possible values for this progress bar to have. See [value_range_format_t](#value_range_format_t) for more information on the dictionary parameters.

##### Examples in CEL

<pre>
progress_bar(orientation: @vertical, format: {first: 0, last: 100, interval: 1});
</pre>

#### radio_button

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the widget.

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

value
: type: [regular](#regular)
: default: [empty](#empty)
: When the value of the cell to which this widget is bound changes to this value, the radio button is selected.

##### Examples in CEL

<pre>
radio_button(name: "Radio Button C", value: @c, alt: "Tooltip info!");
radio_button(value: @none);
</pre>

#### reveal

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

name
: type: [string](#string)
: default: ""
: The name of the widget.

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

value_on
: type: [regular](#regular)
: default: [empty](#empty)
: When the value of the cell to which this widget is bound changes to this value, the reveal widget is 'turned down' or 'on'.
: [ value_on should be renamed "value" for consistency with other widgets. ]

##### Examples in CEL

<pre>
reveal(name: "Advanced Settings", alt: "Click to hide or show advanced options");
</pre>

#### separator

##### Layout Attribute Defaults

* [horizontal](#horizontal): align_fill

##### Additional Attributes

orientation
: type: [name](#name)
: valid values: @vertical, @horizontal
: default: @horizontal
: Specifies the orientation of the separator. Note that if you set the separator to have a vertical orientation, you will also have to set its <code>vertical</code> value to align_fill.

##### Examples in CEL

<pre>
separator(vertical: align_center);
</pre>

#### slider

##### Layout Attribute Defaults

no changes.

##### Additional Attributes

alt
: type: [string](#string)
: default: ""
: The text to be used for the widget's tooltip.

orientation
: type: [name](#name)
: valid values: @vertical, @horizontal
: default: @horizontal
: Specifies the orientation of the slider.

slider_ticks
: type: [integer](#integer)
: default: 0
: Specifies the number of tick marks to place along the slider's edge.

slider_point
: type: [name](#name)
: valid values: @none, @up, @down, @left, @right
: default: @none
: Specifies if the slider thumb has a point and if so which way it points.

format
: type: [dictionary](#dictionary)
: A range of possible values for this slider to have. See [value_range_format_t](#value_range_format_t) for more information on the dictionary parameters.

##### Examples in CEL

<pre>
slider(bind: @progress_value, format: {first: 0, last: 100, interval: 1});
</pre>
