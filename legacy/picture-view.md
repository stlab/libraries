---
title: Picture View
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Picture_View
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Basic Description

The [Generic Image Library (GIL)](https://stlab.adobe.com/gil/) was added to ASL 1.0.12. <code>adobe::image_t</code> is essentially a placeholder for displaying a picture in a dialog.

## Semantics / Rationale

The rationale for the picture widget is a visually rich label, used to enhance the user experience with "metadata" that helps the user accomplish their task within the dialog. This is simply an icon in an alert dialog or a picture. Eventually we would like to consider adding functionality to the point of making it a preview pane that shows realtime updates of the command parameters in action.

## Requirements

A Picture View widget should be able to do the following:
* Bind to and display a GIL image in the Adam sheet.
* Report its best bounds based on the GIL image it's bound to.
    * Trigger a relayout when the image size changes.
* Show an image scaled to fit inside the bounds of the Picture View (stretched and scaled).
* Alpha-blend the image onto the UI if the image has an alpha channel.

## API

### Location

<pre>
<adobe/future/widgets/headers/image_t.hpp>
</pre>

### ASL Public Interface

The picture widget has changed quite a bit over the past several days. Instead of a standalone class with encapsulated functionality, we have decided to make it a platform-specific struct that is accessed with several free functions. The interface for this struct is common for all platforms, however the data contained within the struct varies on a platform basis:

<pre>
struct image_t
{
    image_t(GIL::rgba8_image& image);

    image_t(const image_t& rhs);

    image_t& operator=(const image_t& rhs);

    /* .... member variables are implementation-defined .... */
};
</pre>

The free functions for the platform struct also have the same interface across platform implementations, but their bodies are platform-specific. The signatures are:

<pre>
void initialize(image_t& value);
</pre>

initialize is not meant to be used outside of the platform implementation -- it is used to execute common code between the constructors of the struct

<pre>
extents_t::slice_t measure_horizontal(image_t& value);
extents_t::slice_t measure_vertical(image_t& value, const extents_t::slice_t& horizontal);
</pre>

These two functions form the sum total of the measurement of the widget to be returned to a piece of code requiring them (for instance, a dialog layout engine). This has been split into two functions because we are exploring the concept space ([http://video.google.com/videoplay?docid=-7718210058143075450 cowbell]) of what it means to be a "UI Element".

<pre>
void place(image_t& value, const point_2d_t& position, const extents_t& extents);
</pre>

This is the place call common to all the ASL widgets.

<pre>
void enable(image_t& value, bool make_enabled);
</pre>

Allows for enabling and disabling the widget. We're not quite sure what this "means" for an image_t, but have it here as we have been exploring the UI Element concept. Enablement is definitely a part of being a UI Element, but we're not convinced if it is a requirement or, if it is, whether it belongs in the UI Element concept directly or as part of a refined concept.

<pre>
void set(image_t& value, GIL::rgba8_image& image);
</pre>

This can be used to refresh the image at any point after the widget has been constructed. Currently this does not trigger a relayout of the view system to which the image_t is attached; the new image will be scaled to fit into the original bounds.

<pre>
bool operator==(const adobe::image_t& x, const adobe::image_t& y);
</pre>

Checks for equality. Currently this is componentwise comparison of the struct member variables.

## Related Work

We have work on a [GIL File Format Import/Export Factory](gil-file-format-import-export-factory.html) in place (with budding documentation).
