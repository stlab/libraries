---
title: Alert Dialog API
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Alert_Dialog_API
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Description

This API is a refinement of the [Modal Dialog Integration Kit](modal-dialog-integration-kit.html) in that the model and layout definitions are already defined, and you need to provide a simple collection of parameters to complete the fleshing out of the layout.

## Goals

The intent of the API is to provide the client with an easy and portable way to set up an alert dialog that can:
* Notify the user of something in a way that requires immediate attention
* Optionally get some basic feedback from the user

## API

### Location

<pre>
<adobe/future/alert.hpp>
</pre>

### ASL Public Interface

<pre>
std::pair<adobe::name_t, bool> alert(const char*                    window_name,
                                     const char*                    message_text,
                                     adobe::name_t*                 first_button_name,
                                     adobe::name_t*                 last_button_name,
                                     const char*                    checkbox_name = 0,
                                     const boost::filesystem::path& icon_path = boost::filesystem::path(),
                                     std::size_t                    default_button_index = 0,
                                     std::size_t                    cancel_button_index = 1);
</pre>

window_name
: points to a C-string to be used to set the dialog title.

message_text
: points to a C-string to be used to set the dialog body text.

first_button_name
: points to the first button in a list of possible buttons from which the user can choose.

last_button_name
: points to one-past-the last button in a list of possible buttons from which the user can choose.

checkbox_name
: optionally points to a C-string to be used to label a checkbox. Common uses of a checkbox in an alert dialog may be "apply to all"- or "don't ask again"-style questions. If the pointer is set to =0= the checkbox is not added to the layout.

icon_path
: specifies the path to an image file that is to be used as the icon of the dialog. If unspecified the icon is not added to the layout.

default_button_index
: is the button offset from <code>first_button_name</code> that is to be the default button -- that is, the button that is selected when the user hits the Enter or Return keys.

cancel_button_index
: is the button offset from <code>first_button_name</code> that is to be the cancel button -- that is, the button that is selected when the user his Escape key.

In the event when <code>default_button_index</code> and <code>cancel_button_index</code> refer to the same button the button is given the default behavior, and the cancel behavior is ignored for this alert.

Should strings be passed in decorated with xstring notation they will be localized before they are displayed to the user.

### Return Value

The <code>adobe::alert</code> API returns a <code>std::pair</code> whose elements are as follows:

name_t
: The name of the button the user selected in order to close the alert.

bool
: The state of the checkbox (if one was desired) for the dialog. In the case when the checkbox was not desired, the value of this <code>bool</code> is undefined.

### Notes

While the <code>adobe::name_t</code> set used to name the buttons are not required to be unique it is recommended, otherwise you will not be able to distinguish between a user clicking on one or the other.

In the alert API the invocation of any button will close the dialog.

## Alert Dialog Helper <code>struct</code>

The <code>adobe::alert_helper_t</code> struct has been supplied to ease in the execution of the alert dialog. It takes into account many default settings for you so you only specify that which is different from the default. Every member in the struct has a one-to-one correspondence to parameters within the <code>alert</code> dialog. Finally there is a `run()` member function that passes off the struct parameters to the <code>adobe::alert</code> procedure.

## Issues and Future Considerations

* The range used to describe the button names should move away from <code>adobe::name_t</code> and to a format that is more easily localizable.
* The default and cancel button indices should be of the same type as type used to specify the button names.
* It should be a requirement that each of the button names is unique.
