---
layout: post
title: Creating harmonica tabs with Lilypond
category: software
image: /pics/lilypond.png
---

<span class="center"><img src="/pics/lilypond.png" width="254"></span>

I play a bit of music using a [Hohner Special 20][2] bluesharp.
However I am not quick enough at reading sheet music, so I need to add tabs.
Tabs basically tell you the number of the hole to play and whether to blow or draw (-).
[Lilypond][1] is a concise programming language to generate sheet music.
It turns out, that it is very easy to add lyrics using Lilypond.
By putting the tabs in double quotes it is possible to add the tabs to the music.
I found the tune [Hard Times][3] on the Internet and I used Lilypond to create a version with tabs.
Here is the Lilypond code:

```
\version "2.22.0"
\header {
  title = "Hard Times"
  composer = "Stephen Foster"
  instrument = "Bluesharp"
}
\relative {
  r2 c'4 d
  e2 e4 d
  e8 g4.~ g4 e
  d4 c c d8 c
  e2 c'4 a
  g2 e8 c4.
  d4. c8 e4 d
  c1~
  c2 c4 d
  e2 e4 d
  e8 g4.~ g4 e
  d4 c c d8 c
  e2 c'4 a
  g2 e8 c4.
  d4. c8 e4 d
  c1~
  c2 e4 f4
  g2. g4
  g2 f4 g4
  a1
  g2 r2
  c2 a8 g4.
  e2 d8 c4.
  d4. c8 d4 e
  d2 c4 d
  e2 e4 d
  e8 g4.~ g4 e
  d4 c c d8 c
  e2 c'4 a
  g2 e8 c4.
  d4. c8 e4 d
  c1~
  c4 r4 r2
}

\addlyrics {
  "4" "-4" "5" "5" "-4" "5" "6" "5" "-4" "4" "4" "-4" "4" "5" "7" "-6" "6" "5" "4" "-4" "4" "5" "-4" "4"
  "4" "-4" "5" "5" "-4" "5" "6" "5" "-4" "4" "4" "-4" "4" "5" "7" "-6" "6" "5" "4" "-4" "4" "5" "-4" "4"
  "5" "-5" "6" "6" "6" "-5" "6" "-6" "6" "7" "-6" "6" "5" "-4" "4" "-4" "4" "-4" "5" "-4"
  "4" "-4" "5" "5" "-4" "5" "6" "5" "-4" "4" "4" "-4" "4" "5" "7" "-6" "6" "5" "4" "-4" "4" "5" "-4" "4"
}
```

Here is the output generated by Lilypond.
Notice how the tabs and the notes are aligned to each other.
You can click on the image to open the PDF file.

[![Hard Times music sheet](/pics/hardtimes.png)](/downloads/hardtimes.pdf)

<center><audio controls src="/downloads/hardtimes.mp3"></audio></center>

By the way, I can really recommend to sign up for [David Barrett's harmonica videos][4] to learn tongue blocking which is essential to play single notes as in this example.

Enjoy!

**Update:**

I added harmonica tabs for Silent Night:

[![Silent Night](/pics/silentnight.png)](/downloads/silentnight.pdf)

<center><audio controls src="/downloads/silentnight.mp3"></audio></center>

[1]: http://lilypond.org/
[2]: https://www.hohner.de/en/instruments/harmonicas/diatonic/progressive/special-20
[3]: https://www.harmonicaacademy.com/categories/20080411_25
[4]: https://www.bluesharmonica.com/