---
title: Datapumps
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Datapumps
---
## Datapumps

A datapump is an object that you can bind a view to (like you bind a view to a cell in a sheet). A datapump may be a source of data (e.g.: reading a preference in from the registry, or reading a file in from disk) or it may operate on data as a function (e.g.: transforming a floating point number into a string taking the current locale into account, or performing a string substitution).

### Examples

If we made a "preference" datapump then we could directly bind UI widgets to system preferences:

<pre>
checkbox( bind: preference( "com.apple.dock.launchanim" ), on_value: 1, off_value: 0,
          name: "Show bouncing icons in the Mac OS Dock" );
</pre>

The checkbox would also update if the preference was changed by another application (and the platform's preference API supported monitoring). You could also put the name of the preference in the sheet (rather than passing it directly into the "preference" datapump), if you wanted the same checkbox to refer to different preferences.
