---
layout: post
title: Ruby 1.9 was released
image: /pics/ruby-logo.png
category: ruby
---

<span class="right"><img src="/pics/ruby-logo.png" width="50" alt=""/></span>

Yesterday [Ruby 1.9.1 was released][1]. Here is a small [presentation about changes in Ruby 1.9][2]. There are some new features in the syntax and the semantics of local variables was improved. For example Array.map now returns an enumerator if you don't give a code block as argument

{% highlight ruby %}
e1 = [ 1, 2, 3, 4 ].collect
e2 = [ 5, 6, 7, 8 ].collect
loop { puts "#{e1.next}, #{e2.next}" }
{% endhighlight %}

[Ruby 1.9][3] (formerly known as [YARV][8]) also has native threads. Now it is not necessary any more to use [Distributed Ruby (DRb)][4] to make use of multicore processors. Of course this means that a Ruby extension has to be thread-safe depending on how it is being used.

There also is a lot of work on making the handling of strings aware of the current character encoding

{% highlight shell %}
ruby -e "puts 'Hä?'.size"
# 4
ruby1.9 -e "puts 'Hä?'.size"
# 3
ruby -e "puts 'Hä?'[1]"
# 195
ruby1.9 -e "puts 'Hä?'[1]"
# ä
{% endhighlight %}

Another new feature is the implementation of [fibers][5]. Fibers are like threads but they are not preemptive. This means that a fiber has to suspend itself first before execution of the calling thread can continue. Fibers allow you to implement parallel contexts with implicit locking.

<span class="center"><a href="http://rubyconf2007.confreaks.com/d2t1p8_keynote.html"><img src="/pics/d2t1p8_keynote_preview.jpg" alt="Yukihiro Matsumoto's keynote at Rubyconf 2007" width="400"/></a></span>

You can install Ruby 1.9 alongside 1.8 as follows (under GNU/Linux)

{% highlight shell %}
./configure --prefix=/usr/local --program-suffix=1.9
make
sudo make install
{% endhighlight %}

The performance of the Ruby VM was improved and the memory footprint was reduced. String operations in general will be slower though since UTF-8 now is the default character encoding. But then again Yukihiro Matsumoto's primary goal in designing Ruby always has been to [minimize the effort in programming][6].

Note that it will take some time until the existing Ruby-extensions get ported to 1.9.

**Update:** I submitted [a summary of this][7] to Slashdot and it was accepted

[1]: http://www.rubyinside.com/ruby-191-released-first-production-release-of-the-fastest-ruby-ever-1480.html
[2]: http://www.scribd.com/doc/2589469/Migrating-to-Ruby-19
[3]: http://www.ruby-lang.org/en/news/2009/01/30/ruby-1-9-1-released/
[4]: http://www.ruby-doc.org/core/classes/DRb.html
[5]: http://en.wikipedia.org/wiki/Fiber_(computer_science)
[6]: http://www.artima.com/intv/ruby4.html
[7]: http://developers.slashdot.org/article.pl?sid=09/01/31/230216
[8]: http://www.atdot.net/yarv/

