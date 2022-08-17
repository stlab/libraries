---
title: Tips
layout: page
tab: Tips
icon: thumb-tack
---

From time to time I've posted programming tips (often these have been internal emails), blog posts, or other short form writings. I'm gathering ones that might be of general interest here, updating them in the process.

<ul class='definition-list'>
    {% assign tips = site.pages | where_exp:"p","p.tags contains 'tips'" | where_exp:"p","p.draft == nil" | sort:"title"%}
    {% for p in tips %}
        <li><a href="{{ BASE_PATH }}{{ p.url }}">{{ p.title | markdownify }}</a></li>
    {% endfor %}
</ul>
