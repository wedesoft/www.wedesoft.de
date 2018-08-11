---
layout: post
title: This website is now based on Jekyll
category: software
---

Hi, I decided to update the design of my homepage to a responsive HTML design.
This makes the web page look nicer on mobile phones.
I ended up using the [Hyde][1] theme which is based on [Poole][2] which in turn is based on [Jekyll][3].
I hope you will enjoy the new layout.
Here is a demonstration of what *Jekyll* can do (also to remind myself).
See [Hyde example][4] for more.

# Heading 1
## Heading 2
### Heading 3
#### Heading 4

<div class="message">
  This shows the message style.
</div>

The text can be *italic* or **bold**.

> Quoted text looks like this.

<del>strike through text</del>

<ins>inserted text</ins>

Text with <sub>subscript</sub> and <sup>superscript</sup>

Horizontal line

---

![image](/pics/floppy.png)

Inline code `looks like this`.

{% highlight ruby %}
def highlight
  # This is Ruby code
  some.source code
end
{% endhighlight %}

Here is a Gist from Github:

{% gist c449c3625505c44579544b6c440f9d67 graph-coloring.scm %}

* Bullet
* list
  * of
  * things

1. Enumerated
1. List
  1. of
  1. things



<table>
  <thead>
    <tr>
      <th>Name</th>
      <th>Upvotes</th>
      <th>Downvotes</th>
    </tr>
  </thead>
  <tfoot>
    <tr>
      <td>Totals</td>
      <td>21</td>
      <td>23</td>
    </tr>
  </tfoot>
  <tbody>
    <tr>
      <td>Alice</td>
      <td>10</td>
      <td>11</td>
    </tr>
    <tr>
      <td>Bob</td>
      <td>4</td>
      <td>3</td>
    </tr>
    <tr>
      <td>Charlie</td>
      <td>7</td>
      <td>9</td>
    </tr>
  </tbody>
</table>

 {% latex %}
$\displaystyle\int_{x=0}^\infty x^2 \mathrm{d}x$
 {% endlatex %}

[1]: http://hyde.getpoole.com/
[2]: http://getpoole.com/
[3]: https://jekyllrb.com/
[4]: https://github.com/poole/hyde/blob/master/_posts/2012-02-07-example-content.md
