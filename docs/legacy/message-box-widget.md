---
title: Message Box Widget
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Message_Box_Widget
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Basic Description

A message box widget can be bound to the error handling or other message display mechanism to provide the user with runtime information about the state of the dialog.

## Semantics / Rationale

The rationale for the widget is that there needs to be a feature rich static text field that is very flexible about its size, content, and visibility. These are not features we want to add to the static_text_t widget currently, as that goes beyond the semantic intent of that widget. This is more or less a warning indicator with text that comes and goes as warnings should be presented to the user.

## Requirements

* It must be able to change content dynamically
* When there is no message to be displayed the widget should not only be invisible, but it should have no "placeholder" space in the layout. That is, when it does become visible it should "grow" into the location that it is to occupy.
* It must be able to respond to user intervention, in case there is a desired behavior as a result of the user acknowledging the warning.

## API

<pre>
struct message_box_t
{
public:
  message_box_t(adobe::theme_t theme);

  adobe::extents_t measure();
  void set_bounds(const point2d_t&, const extents_t&);

  void set_field_text(const std::string& text); // set_name?

  // should there be an activate call as well? I don't think there should
  // be as there isn't a meaning to a deactivated, yet visible, message box

  void set_visible(bool make_visible);

  // used to call back to the client when the message box is clicked on
  // possible behaviors might include resetting the value that triggered
  // the warning to a last known good value.
  void set_hit_proc(adobe::implementation::hit_callback_proc_t proc);

private:
  implementation_t* object_m;
};
</pre>

### API Considerations

* Do we want to have some kind of icon support, like 3 or 4 different types of icons one could attach to the message box as well as a piece of text?
  * RalphThomas: Icon is a specialization of [Picture View](picture-view.html).
* How do we trigger a re-layout of the view? Should we have a callback set within message_box_t that is called whenever the visibility changes, or should the relayout be an issue handled by the same client that set the visibility? I'm guessing it'll be the latter.
* How do we handle the fact that the metrics of the widget change at runtime? Eve doesn't call calculate upon relayout, does it?
  * RalphThomas: it does for evaluate, but not for adjust.

## Other Issues

RalphThomas thinks that this current proposal combines the optional panel, a dynamic_text and a way to bind to an invariant violation. Would the following be more useful?

* some kind of [dynamic_text](https://web.archive.org/web/20100929073310/http://www.missioncode.org/) widget.
* allow binding to an invariant, flag it when it's violated (is this already possible?)
* a way to determine which cell caused the violation if the invariant is complicated; or just have simple invariants like in the example below.

It might look like this:
<pre>
sheet errortest {
  interface:
    width  : 800 ;
    height : 600 ;
  output:
    result <== { w: @width, h: @height } ;
  invariant:
    width_max  <== width  <= 300000 ;
    height_max <== height <= 300000 ;
}

dialog( name: "error test", placement: place_column ) {
  row() {
    optional( bind: @width_max ) { picture( image: "error.png" ); }
    edit_number( name: "Width:", bind: @width );
  }
  row() {
    optional( bind: @height_max ) { picture( image: "error.png" ); }
    edit_number( name: "Height:", bind: @height );
  }
  button( action: @ok, name: "OK", bind: @result );
}
</pre>

Note that you could do the above right now if there was a way to make the optional panel show when the thing it was bound to went inactive...

If we had a way to style a widget (color, etc) then we could make the offending edit_number turn red instead of showing an image next to it.

_What other issues will need consideration?_

## Related Work

_What additional code must be written to support this widget? What platform implementations / APIs can be leveraged when implementing this widget?_
