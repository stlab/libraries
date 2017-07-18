---
title: Handling of Limits in Property Model Library
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Handling_of_Limits_in_Property_Model_Library
---
One of our current problems with the New Document dialog is the handling of value limits. Enforcing limits is not a function of the UI but a function of the model.

Let's take the current state of affairs and a specific example - we want to limit the number of pixels to a range of [1-300,000] if I understand correctly. We can do that easily enough in the UI for pixels but what happens when we violate the range through inches? In this case what to limit the range to becomes dependent on resolution and things get much more complicated.

We can express the limit by adding a couple of lines to the property model like so:

invariant:
	valid_width <== (pixel_width >= 1) && (pixel_width <= 300000);

Currently the only thing that would happen when this triggered is it will dim the okay button. Even with the current limitations though we could do a bit better - we could have a message area appear at the bottom keyed off this invariant which could even have a "Repair" button which would correctly pin the value (model and layout at the end of this message demonstrates this).

The available options that I see for handling an invariant violation are:

* limit - prevent input which causes the violation in the first place. When the value being edited is directly limited, this is the preferred UI.
* auto-repair - repair the bad value as soon as it occurs in the model.
* auto-revert - return the model to the last known valid state prior to the violation.

* repair - correct the violation by adjusting values in the model through a user action (such as pressing a repair button)
* revert - return to the last valid state through a user action (such as pressing a revert button)
* accept - allow the system to be in an invalid state - but disable the OK button until the user corrects the problem - message could describe what the problem is and Cancel would always be available.

* limit is straight forward for a controller with a range (such as a slider) when the value being edited is the value with the invariant.
	It is not so easy to do when the controller doesn't have a natural way to limit the range (such as an edit text field) or when the invariant is derived from the value 	One example, if we are editing inches then the limit on the number of inches is a function of the pixels and resolution - this is non-trivial to calculate correctly because it requires an accurate reversal of a floating point equation - one way to do this is to be conservative and overly constrain the number of inches. We would need to research how to get such reversals correct.

An edit text field causes additional problems - by limiting the range on the value entered we prohibit you from going through a bad value to get to a good one. Imagine a lower limit of 0.1 - but you can't type zero or a limit of 10 but you can't type 1. There are techniques to mitigate this such as requiring the number be "entered" through some additional keystroke, such as tab which isn't very discoverable (the enter key is a bad choice because that usually dismisses the dialog) - or simply letting some small time elapse which has problems with accessibility.

Units also pose a problem for limits on edit number fields - the easiest way to enter 3 inches is to type "3 inches" - but if the current unit is something other than inches and 3 is invalid, and rejected, you have a problem. Photoshop currently attempts to constrain the keyboard focus to the widget contributing to the violation until the problem is corrected. There are several places in the application where this allows you to save an invalid value (by toggling a field off, saving a preset, or other similar operations), or prohibit you from taking an action which would correct the problem without changing the number - which brings us to the next issue.

When the limited value is derived from more than one number then the limit also inhibits you from going through a bad value to get to a good one (for example - you want a document 10 inches wide at 300 dpi but the resolution is currently set to be 300000dpi - you can't enter 10 inches until you first fix the resolution). This is a cumbersome sequence even if you know what you have to do (and many customers don't).

* auto-repair - this is very similar to providing a limit except in this case the controller places the model in an invalid state which then takes some action to correct the mistake. Care has to be taken in defining the corrections or you can get into a situation where failures cascade (it would likely be a good idea to limit the number of failures that can occur - typically to 1). There are some subtle difference in the UI compared to limits: There is no need with auto-repair to determine the limit in advance - or to constrain the input in advance - this eliminates the problem of having to try and calculate a range for a derived value - but it also means you can't easily have a slider interface with proper limits since the limits aren't known in advance. The other problems of a limit still apply. Scrip recording is also impacted as the correction changes what the contributing value is to the system - so in our example pixels will get recorded instead of inches.

* auto-revert - again, similar to providing a limit except in this case a change to an invalid state is rejected by the model. There is an implementation problem here in that most OS controllers currently don't request changes - they notify of a change. So if the change is rejected - the model will not notify the view (because there was no change) and the widget is left in an invalid state. Except for cascading failure and script recording the problems of auto-repair also apply.

* repair - here we avoid all of the problems of limiting the user input by making the repair operation specific - likely this is done (as described above) - by having a warning appear in the view with a button to repair the problem. The same care must be taken in defining the repair operation or you risk cascading failures (fairly embarrassing to have a user tell you to repair something only to present them with another problem). Because the user isn't limited to what they can do within the dialog while in an invalid state (except they cannot OK the action) they can cause additional failures. I can see one of two ways to handle this - either display all the warnings, each with a separate fix operation or join them into a single warning with a revert option. A combined "fix" would be especially tricky to avoid a cascading failure. As an example of how a user could get into this situation - imagine creating a new document where you specify the width (in inches) and height (in inches) causing the pixel values to both exceed max (two messages) - but the user planned to then set the resolution to something reasonable to bring pixels back into bounds.

* revert - would be handled like repair (and could be an option alongside repair in the UI) - here the model would simply be restored to the last known good state - a single revert button would suffice even with cascaded failures.

* accept - this is what we currently do (simply dimming the OK button) - I view this as a requirement for any combination of repair and revert.

My current recommendation is that -

* We limit when the value being changed is the value which is directly constrained. This requires us to come up with some reasonable solution for edit number fields - but we I'm hopeful we can figure out a decent UI here.
* When a derived value is constrained we use a combination of repair, revert and accept presented nicely in a message box which slides into the bottom of the window.

We might be able to think up other options - for example, we know which items are currently contributing to the invalid state and which item would be fixed. I haven't come up with any way to present this information to the user which is particularly helpful (other than wording the message right for the repair button(s) ).

```
sheet my_sheet
{
interface:
   pixel_width: 42;
output:
   result <== pixel_width;
   valid_width_min_t <== pixel_width >= 1;
invariant:
    valid_width_min <== valid_width_min_t;
}
```
```
layout my_dialog
{
    view dialog(name: localize("<xstr id='my_dialog_name'>My Dialog</xstr>"))
    {
	column()
	{
        	edit_number(bind: @pixel_width, format: '#', alt: 'Alters the value of the slider');
		optional(bind: @valid_width_min_t, value: false, placement: place_row)
		{
			static_text(name: "Pixel width must be 1 or more.");
			button(name: "Repair", bind_output: @pixel_width, value: 1);
		}
	}
        button (items: [
                           { name: localize("<xstr id='ok'>OK</xstr>"), action: @ok, bind: @result, alt: 'Perform the command with the current settings' },
                           { name: localize("<xstr id='reset'>Reset</xstr>"), action: @reset, modifiers: @opt, alt: 'Reset the dialog settings' }
                       ]);
    }
}
```
