---
title: Tips
layout: page
---

From time-to-time I've posted programming tips (often these have been internal emails), blog posts, or other short form writings. I'm gathering ones that might be of general interest here, updating them in the process.

## Contents

<dl class='posts'>
{% for p in site.pages %}
    {% if p.tags contains 'tips' and p.url != page.url %}
        {% if p.draft %}{% else %}
            <dt><a href="{{ BASE_PATH }}{{ p.url }}">{{ p.title | markdownify }}</a></dt>
        {% endif %}
    {% endif %}
{% endfor %}
</dl>

