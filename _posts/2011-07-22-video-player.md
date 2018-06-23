---
layout: post
title: Ruby Video Player
image: /pics/sintel.jpg
---

{% youtube "http://www.youtube.com/watch?v=MzgGCjDryXA" %}

This is the second episode of my new **Computer Vision for the Robotic Age podcast**. This episode is about video-I/O. The podcast demonstrates how a video player with proper audio/video synchronisation can be implemented with the Interactive Ruby Shell. The [Sintel][3] short film (Copyright Blender Foundation) was used as a video for testing.

Here's the source code of the [Ruby video player][4] created in the podcast:

{% highlight ruby %}
require 'rubygems'
# load FFMPEG bindings
require 'hornetseye_ffmpeg'
# load X.Org bindings
require 'hornetseye_xorg'
# load ALSA bindings
require 'hornetseye_alsa'
# include the namespace
include Hornetseye
# open a video file
input = AVInput.new 'sintel.mp4'
# open sound output with sampling rate of video
alsa = AlsaOutput.new 'default:0', input.sample_rate, input.channels
# read first audio frame
audio_frame = input.read_audio
# display images using width of 600 pixels and XVideo hardware acceleration
X11Display.show 600, :output => XVideoOutput do |display|
  # read an image
  img = input.read
  # while there is space in the audio output buffer ...   
  while alsa.avail >= audio_frame.shape[1]
    # ... write previous frame to audio buffer
    alsa.write audio_frame
    # read new audio frame
    audio_frame = input.read_audio
  end
  # compute difference of video clock to audio clock
  delay = input.video_pos - input.audio_pos + (alsa.delay + audio_frame.shape[1]).quo(alsa.rate)
  # suspend program in order to synchronise the video with the audio
  display.event_loop [delay, 0].max
  # display image
  img
end
{% endhighlight %}

You can also download the video here
<ul>
  <li><a href="http://dl.dropbox.com/u/49280716/hornetseye-video-player.m4v">hornetseye-video-player.m4v</a> (12.19 MB)</li>
</ul>

**See Also:**

* [Ruby video player on gist.github.com][4]

[1]: http://wedesoft.libsyn.com/webpage
[2]: http://wedesoft.libsyn.com/rss
[3]: http://www.sintel.org/
[4]: https://gist.github.com/1182886

