---
layout: post
title: Time Tracking
category: life
image: /pics/clock.jpg
---
I recently read an [interesting article about time tracking by Sebastian Marshall][trackpost]. There are also [two videos about time tracking][trackvideo] by the same author. To quote from the website:

<blockquote>
<p>To put it simply – I now realize it’s impossible to understand how your life is going without some careful observation. There’s a lot of time each day, and knowing where that time goes, what you ate, what you did and didn’t do… it’s almost impossible to get a good picture of your life without some kind of measuring.</p>
</blockquote>

I realised that one could develop a small time tracker in Ruby quite easily. The tracker shown below will ask you questions after showing you the answers of the previous 7 days. In the morning it asks questions of your plans and in the evening it asks you questions in review. You can also run the script multiple times and skip through the questions where you don’t want to modify the answer. I have tried to keep it simple. Quoting from [another article by Sebastian Marshall][trackdev]:

<blockquote>
<p>Second, remember to start simple. This is to build up momentum and make a workable system you actually use. Do it every day. If you miss a day or two or three, fill in from memory as best as you can. If you fell off a cliff for a while, just reboot. Don’t beat yourself up too much – it solves nothing. We all fall off a cliff sometimes. Also, remember the gains made from living more purposefully are forever – the time you’ve spent well will remains well-spent even if you fall off for a while sometimes. Most people don’t even try, which is why most people don’t succeed.</p>
</blockquote>

{% highlight ruby %}
#!/usr/bin/env ruby
require 'readline'
require 'fileutils'
require 'rexml/document'
include REXML
def track( log, previous, tag, query )
  previous.each_with_index do |prev,i|
    if prev
      if prev.elements[ tag ]
        puts "#{previous.size-i} day(s) ago: #{prev.elements[ tag ].text}"
      end
    end
  end
  item = log.elements[ tag ]
  item = log.add_element tag unless item
  if item.text
    line = "-> #{query} (#{item.text})? "
  else
    line = "-> #{query}? "
  end
  # print line; STDOUT.flush
  # text = STDIN.readline.gsub /[ \r\n]*$/, ''
  text = Readline.readline line
  item.text = text unless text.empty?
end
if File.exist? 'track.xml'
  doc = File.open( 'track.xml', 'r' ) { |f| Document.new f }
else
  doc = Document.new
  doc.add_element 'track'
end
time = Time.new
t = "%4d/%2d/%2d" % [ time.year, time.month, time.day ]
log = doc.root.elements[ "log[@date='#{t}']" ]
previous = ( 1 .. 7 ).collect do |i|
  timep = time - 86400 * i
  tp = "%4d/%2d/%2d" % [ timep.year, timep.month, timep.day ]
  doc.root.elements[ "log[@date='#{tp}']" ]
end.reverse
unless log
  log = Element.new 'log'
  log.add_attribute 'date', t
  doc.root.add_element log
end
list = []
list +=
  [ [ 'wakeup'     , 'At what time did you get up' ],
    [ 'sleep'      , 'How long did you sleep' ],
    [ 'wellness'   , 'Do you feel well' ],
    [ 'objective'  , 'What\'s the objective for today' ] ]
if time.hour >= 14
  list +=
    [ [ 'description', 'What did you do today' ],
      [ 'food', 'What did you eat' ],
      [ 'positive', 'What did I do right to move me towards my goals' ],
      [ 'toimprove', 'What would I do differently if I had the day to live over' ] ]
end
list.each { |tag,query| track log, previous, tag, query }
File.open 'track.xml.part', 'w' do |f|
  doc.write f
end
FileUtils.mv 'track.xml.part', 'track.xml'
{% endhighlight %}

I will try to play with this for some time and see whether it has any benefits.

**See also:**

* [Leo Babauta - Focus][focus]
* [Brain Workshop (software)][brainworkshop]
* [Alan Watts - Work as Play (video)][workasplay]
* [Arnold Bennett - How to Live on 24 Hours a Day (free ebook)][livefullday]
* [Nigel Marsh - How to make work-life balance work (video)][worklifebalance]
* <a href="http://www.amazon.co.uk/gp/product/0385528752?ie=UTF8&amp;tag=wedesoft-21&amp;linkCode=as2&amp;camp=1634&amp;creative=19450&amp;creativeASIN=0385528752">Chip Heath, Dan Heath - Switch: How to Change Things When Change Is Hard (book)</a>

[trackpost]: http://www.sebastianmarshall.com/detailed-analysis-of-my-current-timehabitlife-tracking
[trackvideo]: http://www.sebastianmarshall.com/time-tracking-video
[trackdev]: http://www.sebastianmarshall.com/the-evolution-of-my-timehabitlife-tracking
[brainworkshop]: http://brainworkshop.sourceforge.net/
[workasplay]: http://www.veoh.com/browse/videos/category/entertainment/watch/v3297411Z7k6sfZA
[livefullday]: http://www.fullbooks.com/How-to-Live-on-24-Hours-a-Day.html
[worklifebalance]: http://www.ted.com/talks/nigel_marsh_how_to_make_work_life_balance_work.html
[focus]: http://focusmanifesto.com/

