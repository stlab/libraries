---
title: Preset Widget
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Preset_Widget
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Description

The basis of a dialog is the collection of valid parameters in order to execute a command. Many times the client would like to store these parameters (or a subset of them) for use in a later execution of the same command. The Preset widget is a general-use solution to save, manage and apply these collections of reusable parameters for a command.

## The Eve Syntax

A preset widget in the Eve syntax would look as follows. Only parameters directly pertaining to the semantics of this widget are presented:

<pre>
preset(name: "new_document",
       domain: "Photoshop",
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

name
: This is the "name" of the command for which this preset is being made available. A combination of this value and the domain combine to form a path under which the user's saved preset values will be stored. The path is a unique identifier for the presets of a command, which will be stored in a file on the user's machine. For the widget above, the location will be: <code>~/Library/Application Support/Adobe/Photoshop/new_document.preset</code> for Mac OS X, and <code>C:\Documents and Settings\{username}\Application Data\Adobe\Photoshop\new_document.preset</code> for Win32 (where {username} is replaced by the name of the user running the app)

domain
: The domain of the preset is typically the application under which this command is commonly run. This allows one heirarchical level under which a group of command presets may be stored.

bind_additional
: Oftentimes the application will want to supply additional presets to the dialog given its current state. For example, the application might want to supply a list of presets that correlate to the documents currently open. The bind_additional attribute specifies a cell in the property model (or layout sheet) that will contain these extra presets from the application.

bind_output
: Presets perform reverse-lookup of the state of the model to determine if the state matches one of the presets present in the listing. If a preset is found, the preset widget displays that particular preset. This visual feedback notifies the user that, given the current values in the model, they may also use a particular preset to achieve the same state. bind_output specifies a cell which will contain a dictionary of the current "snapshot" of the dialog state so that the preset dialog may accomplish this behavior.

bind
: This array is a set of pairs representing the potential parameters that can be pulled from the current state of the model and saved as a preset. For each pair, the first element is either an adobe::name_t that identifies a cell, or an adobe::array_t of adobe::name_t elements identifying a set of cells combined under a single identifier. The second element in the pair is a name to be used when saving a preset in the Add Preset dialog.

localization_set
: The preset widget has several built-in dialogs to assist the user in managing their presets. The strings to be displayed in these dialogs are internationalized through this dictionary. Each of the keys corresponds to a displayable string in one of the built-in dialogs in the preset widget. The user, though this optional dictionary, can specify localized values for these displayable strings.

## Semantic Notes

The categories in the preset widget are a composition of several sources:
* dynamic preset set: whatever the app wants
    * specified by the client via preset_t::display
* defaults: these are the application defaults.
    * loaded from load_default_preset_set
    * the name of the file should be "<name>.presets"
    * the file should be in the resources directory
* user_prefs: custom user presets
    * loaded from load_user_preset_set
    * the name of the file should be "<name>.presets"
    * the location is derived based on OS preference file location rules

Each of the above are adobe::array_t structures that are combined in the order specified above. None are required. Should the user preferences file be empty, an empty category will be auto-created by the widget.

### On-Disk Format

Currently the on-disk format for the preset widget leverages the Adobe Common Expression Language, the same language used for the foundation of the Adam/Eve description languages. The format of the file on-disk is the following:
<pre>
    [ {
        items: [ {
            name: 'This is the name of the preset SINGLETON',
            value: { // this is the actual property set to be sent to the property model
                height_inches: 5,
                width_inches: 7
            }
        },
        ... more singletons if necessary ...
        ],
        name: 'This is the name of the preset CATEGORY'
    }, ... more categories if necessary ...
    ]
</pre>

## Visual Representation

The "widget" itself is actually a self-contained suite of functionality for managing and selecting presets. Visually there is a popup labelled "Presets" which contains the names of all the presets currently saved for this command. There is also an icon to the right of the popup. When the user clicks on the icon, they get a manu containing (in this order):
* Append Presets...
* Add Preset...
* Delete Preset...

### Append Presets
This allows you to append a file full of presets from an external location into your current preset list. See the comment regarding the on-disk format of this file for more information on what this file should look like internally.

### Add Presets

This lets you save the currently contributing set of values to the command model as a preset, allowing you to selectively include/discard parameters from the preset. When this option is selected a dialog box will show. In it will be an edit text field where the user can give a name to the preset. Also available to the user will be a series of checkboxes. Each checkbox will have a name (from the localizeable list of names described in the Eve definition for this preset) and a set of model cells to which they are bound. When the user hits OK to this dialog, the cells in the model that have their respective checkboxes checked will be added to the preset, and the preset as a whole will be appended to the list on disk. Likewise, the preset widget in the main dialog will be updated.

### Delete Preset

This lets you select a preset from the list of saved presets to have it removed permanently. When this option is a selected a dialog box will show with a popup filled with the names of the currently saved presets. The user then selects a preset from the popup and hits the OK button in the dialog, and that preset is removed from the list of saved presets. The preset widget in the main dialog is then updated to reflect this change. Likewise, the preset file on-disk is updated with the preset deleted.

## Future Work

* An interesting issue here (which I'm not quite sure how to solve). Luckily I don't think we need a solution for this dialog.

  When I click that I want to add something to my preset - my expectation would be that it is added even if it doesn't currently contribute. But doing this would allow us to potentially "over constrain" the preset by capturing more than you could.

  One potential solution would be to bring the dialog up with only items which currently contribute set. If you turn a checkbox on which isn't currently contributing, we touch the cell and anything which is no longer contributing gets disabled.

  For this dialog none of the presets are "optionally" contributing (except within the width/height sets) so we can ignore it for now. But something to note and think about.
