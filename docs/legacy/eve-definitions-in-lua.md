---
title: Eve definitions in Lua
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Eve_definitions_in_Lua
---
Often many dialogs in an application have very similar Eve definitions. For example, all of the "Properties" dialogs on Windows contain a tab group and "OK/Cancel/Apply" buttons. It would be nice to be able to define functions that return commonly used combinations of widgets.

One solution to this problem is to write the Eve definitions in an embeddable language such as [Lua](http://www.lua.org/).

My current thinking is that a datastructure would be built in Lua that defines all of the views, and that there would be a function exposed that would take this datastructure and build a UI from it. So an Eve definition might look like this:

<pre>
view = dialog{ name = "Test" }
{
 row{ horizontal = "align_fill", vertical = "align_fill" }
 {
   static_text{ name = "This is a test." },
   button{ name = "Click me", action: cancel }
 }
}
--
-- Now build it.
--
asl_factory(view)
</pre>

The various view functions (dialog, row, static_text, button, etc) can be defined like this:

<pre>
function leaf_factory(name)
	return function(params)
		local t = {}
		t.name = name;
		t.params = params;
		return t;
	end
end

function container_factory(name)
	return function(params)
		return function(children)
			local t = {}
			t.name = name;
			t.params = params;
			t.children = children;
			return t;
		end
	end
end

local dialog = container_factory("dialog")
local row = container_factory("row")
local column = container_factory("column")
local static_text = leaf_factory("static_text")
local button = leaf_factory("button")
</pre>
