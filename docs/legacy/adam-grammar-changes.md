---
title: Adam Grammar Changes
layout: page
tags: [legacy]
comments: true
redirect_from: /legacy/index.php/Adam_Grammar_Changes
---
#### Contents
{:.no_toc}
1. contents
{:toc}

## Allow the <code>when</code> clause to contain multiple relations

Instead of:

<pre>
   when (p) relate { ... }
   when (p) relate { ... }
   when (!p) relate { ... }
   when (!p) relate { ... }
</pre>

we have:

<pre>
   when (p)
   {
      relate { ... }
      relate { ... }
   }

   when (!p)
   {
      relate { ... }
      relate { ... }
   }
</pre>

or even:

<pre>
   when (p)
   {
      relate { ... }
      relate { ... }
   }
   otherwise // 'else' seems too if-like; some word for "when not"
   {
      relate { ... }
      relate { ... }
   }
</pre>

## Shorthand for direct relationships between two cells

Instead of:

<pre>
   relate {
      y <== x;
      x <== y;
   }
</pre>

we have:

<pre>
   relate {
      x <=> y;
   }
</pre>

or even:

<pre>
   relate x <=> y;
</pre>
