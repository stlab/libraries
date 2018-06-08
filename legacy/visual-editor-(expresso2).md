---
title: Visual Editor (Expresso2)
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Visual_Editor_(Expresso2)
---

Sean Parent wrote in mail to adobe-source-devel regarding his thoughts on a visual editor for Eve files:

I have some thoughts - Foster may have some also - he implemented the original Expresso -

* The basic UI that I envision is a two view editor - a tree view showing the UI elements and a preview.

The tree view should be fairly simple - showing an icon for the view type and the name property (after localize is executed) for the item - if no name property then the view type is displayed

```none
 [] My Dialog
  -- row
    () OK
    () Cancel
```

* Drag and drop (including multiple items) - should be allowed within the tree view as well as between the tree view and preview windows.

* The preview window should be live - meaning you can select elements directly in it and drag items around. Items can be dropped into containers (containers should highlight to show you where the item will be dropped.

* An inspector palette should be present to show you the properties of the view selected. I would only show the editable properties and possibly organize them into panels - you might want a debugging palette or panel which shows the calculated properties. This palette should be fairly easy to create with Eve (and Adam) - I've been meaning to construct this part with the current system just to show the properties of each of the widgets types.

* Shortcuts for typical properties should be provided - ctrl-[ - ] for align left, center, right, etc...

* A palette of the available widgets that can be dropped onto either the tree view or the preview would be great.

* Within the inspector palette I would allow expressions - not just values - for the properties.

* From an implementation standpoint - the Eve (and Adam) languages have been designed to support roundtrip editing - this is why the comments are in the syntax - so they can be associated with particular elements and carried through an editor. The way to build such a beast is to replace the eve_evaluate file that captures the information from the parser and populate a structure for the editor (possibly using the forest library).

* One implementation hole that I know of at the moment is that there is no way way to erase an item from a layout - but I believe that could be easily added.

* Bonus points for a live source view
* Double bonus points for alternative syntaxes (especially an XML based one)
* Triple bonus points for a property model view (a table of cells and expressions) - and drag-drop to connect
* 50 bonus points for a property model visualizer (likely using GrafViz)
