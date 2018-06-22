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
list += [ [ 'wakeup'     , 'At what time did you get up' ],
          [ 'sleep'      , 'How long did you sleep' ],
          [ 'wellness'   , 'Do you feel well' ],
          [ 'objective'  , 'What\'s the objective for today' ] ]
if time.hour >= 14
  list += [ [ 'description', 'What did you do today' ],
            [ 'food', 'What did you eat' ],
            [ 'positive', 'What did I do right to move me towards my goals' ],
            [ 'toimprove', 'What would I do differently if I had the day to live over' ] ]
end
list.each { |tag,query| track log, previous, tag, query }
File.open 'track.xml.part', 'w' do |f|
  doc.write f
end
FileUtils.mv 'track.xml.part', 'track.xml'

