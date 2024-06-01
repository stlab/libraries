---
layout: default
---

{% assign home_thumbs = site.pages | where_exp:"p","p.hyde.icon" | sort:"hyde.tab"%}

<div class="home">
  <table class='icon-table'>
  <tr>
    {% for page in home_thumbs %}
      <td>
            <a href='{{page.url}}'>
            <div class='icon-box'>
                <div class='top'><i class="fa fa-inverse fa-{{page.hyde.icon}}"></i></div>
                <div class='bottom'>{{page.hyde.tab}}</div>
            </div>
            </a>
      </td>
    {% endfor %}
  </tr>
  </table>

  <br/>

{% assign releases = site.data.releases | sort: 'published_at' | reverse %}
{% assign release = releases | first %}
{% assign prev_release = releases | shift | first %}
{% assign release_tag = site.data.tags | where_exp:'p','p.name == release.tag_name' | first %}

  <h1>Latest Release: {{release.tag_name}}</h1>

Released: {{release.published_at | date: "%b %-d, %Y" }}
<br/>
Sources: <a href='https://github.com/stlab/libraries/compare/{{prev_release.tag_name}}...{{release.tag_name}}'>changes</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href='{{release.zipball_url}}'>zipball</a>&nbsp;&nbsp;|&nbsp;&nbsp;<a href='{{release.tarball_url}}'>tarball</a>
<br/>
Changes: {{release.body | markdownify}}
For older releases, the full list is available <a href='https://github.com/stlab/libraries/releases'>on GitHub</a>.

  <h1>Posts</h1>

  <ul class="post-list">
    {% for post in site.posts %}
      {% if post.categories contains 'release' %}
        {% continue %}
      {% endif %}
      {% if post.hidden %}
        {% continue %}
      {% endif %}
      <li>
        <span class="post-meta">{{ post.date | date: "%b %-d, %Y" }}</span>

        <h2>
          <a class="post-link" href="{{ post.url | prepend: site.baseurl }}">{{ post.title | markdownify }}</a>
        </h2>
      {{ post.excerpt | markdownify }}
      </li>
    {% endfor %}

  </ul>

  <p class="rss-subscribe">subscribe <a href='{{ "/feed.xml" | prepend: site.baseurl }}'>via RSS</a></p>

</div>
