---
layout: post
title: Avoiding alias_method_chain in Ruby
category: ruby
image: /pics/rails-refactor.png
---

I currently use [Ruby][1] a lot but since I am not into developing web applications, I am not knowledgeable about [Ruby on Rails][2]. So when I watched [Yehuda's presentation about refactoring Rails][3] I watched it not so much because of Rails but because of refactoring Ruby software.

<span class="center"><a href="http://confreaks.net/videos/50-mwrc2009-the-great-rails-refactor"><img src="/pics/rails-refactor.png" width="320" alt="Yehuda Katz - The Great Rails Refactor"/></a></span>

Yehuda talks about introducing modules to be able to use _super_ instead of _alias\_method\_chain_. I tried it out on an example and it seems to be a really useful thing to know.

<h4>Using alias_method_chain</h4>

This approach uses the popular definition of _alias\_method\_chain_

{% highlight ruby %}
class Module
  def alias_method_chain( target, feature )
    alias_method "#{target}_without_#{feature}", target
    alias_method target, "#{target}_with_#{feature}"
  end
end
{% endhighlight %}

As an example we have a class _Test_ with a method _x_

{% highlight ruby %}
class Test
  def x
    'x'
  end
end
{% endhighlight %}

Using _alias\_method\_chain_ the method _x_ can be extended without having to introduce a new inheriting class

{% highlight ruby %}
class Test
  def x_with_two
    "#{x_without_two}2"
  end
  alias_method_chain :x, :two
end
{% endhighlight %}

The method now exhibits the new behaviour

{% highlight ruby %}
puts Test.new.x
# "x2"
{% endhighlight %}

<h4>Using modules</h4>

However the method _x_ can be declared in a module which is included by _Test_

{% highlight ruby %}
module X
  def x
    "x"
  end
  module_function :x
end
class Test
  include X
  public :x
end
{% endhighlight %}


Overloading the method now can be done by including another module which has a new definition of _x_. The new method _x_ can invoke the previous one by calling _super_

{% highlight ruby %}
module X2
  def x
    "#{super}2"
  end
  module_function :x
end
class Test
  include X2
end
{% endhighlight %}


The method now exhibits the new behaviour

{% highlight ruby %}
puts Test.new.x
# "x2"
{% endhighlight %}

The issue with this approach is that overloading of the method needs to be anticipated. However at the same time the code indicates it more clearly that the first definition of _x_ is not necessarily the final one in objects of type _Test_. Moreover this approach works without public definitions of methods you are not supposed to call (_e.g._ _x\_without\_two_).

**Update:** If you like to see another talk from MountainWest RubyConf 2009, you should check out Jim Weirich's talk on [The Building Blocks of Modularity][4].

<h2>See also:</h2>

* [Corresponding entry at RubyFlow][5]
* [Yehuda Katz on alias\_method\_chain][6]

**Update:** [More recent post by Yehuda Katz][7]

**Update:** Ruby 2.0 will introduce [Module#prepend][8]

[1]: http://www.ruby-lang.org/
[2]: http://rubyonrails.org/
[3]: http://confreaks.net/videos/50-mwrc2009-the-great-rails-refactor
[4]: http://confreaks.net/videos/77-mwrc2009-the-building-blocks-of-modularity
[5]: http://www.rubyflow.com/items/2047
[6]: http://yehudakatz.com/2009/03/06/alias_method_chain-in-models/
[7]: http://yehudakatz.com/2010/02/15/abstractqueryfactoryfactories-and-alias_method_chain-the-ruby-way/
[8]: http://www.slideshare.net/yukihiro_matz/rubyconf-2010-keynote-by-matz
