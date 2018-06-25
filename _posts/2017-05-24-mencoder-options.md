---
layout: post
title: MEncoder options
category: software
image: /pics/mplayer.png
---
<span class="center"><img src="/pics/mplayer.png" width="240" alt="MPlayer and MEncoder"/></span>

<h2>Encoding DVDs with MEncoder</h2>

Here is an example on how to encode a DVD using *MEncoder*.

{% highlight shell %}
mencoder dvd://1 -o test.avi -ss 120 -endpos 10 -alang en -slang en -aid 130 -psprobe 1000000 -vf crop=720:446:0:66 -aspect 1.78 -af volume=15 -ovc x264 -x264encopts crf=21:threads=2 -oac mp3lame -lameopts q=2
{% endhighlight %}

You can get hints for the audio id, supported languages, aspect ratio, and other values by playing the DVD with *mplayer dvd://1* and inspecting the resulting console output.

* *dvd://1*: read the main track of the DVD. Sometimes the main video is on another track (e.g. dvd://3, dvd://5, dvd://81, ...).
* *-o test.avi*: selects the output file and *AVI* as container format. You could also specify an *MP4* file here.
* *-ss 120 -endpos 10*: this tells the encoder to skip the first 120 seconds and then stop after encoding 10 seconds of video. This is used initially to test on a small representative part of the video that the encoding options are correct. Afterwards you can drop these options.
* *-alang en*: choose the English audio track.
* *-slang en*: add the English subtitles to the video.
* *-aid 130*: choose the audio track with id *130*. Specifying this is not necessary in most cases.
* *-psprobe 1000000*: use this option to force *MEncoder* to extend the search for the audio stream. Only necessary if *MEncoder* says *no audio stream found*.
* *-vf crop=720:446:0:66*: you can use this to crop of black borders.
* *-aspect 1.78*: often it is necessary to set the pixel aspect ratio of the output video.
* *-af volume=15*: with many DVDs it is necessary to boost the audio volume.
* *-ovc x264 -x264encopts crf=21:threads=2*: this selects the H.264 video encoder and configures it.
* *-oac mp3lame -lameopts q=2*: this selects the MP3 audio encoder and configures it. You can also use *-oac faac -faacopts quality=300* if you prefer to use the AAC codec.

Once you have found the right options, rerun the command without the *-ss* and *-endpos* option to encode the full video.

**See also:**

* [MPlayer][mplayer]

[mplayer]: https://www.mplayerhq.hu/design7/dload.html``
