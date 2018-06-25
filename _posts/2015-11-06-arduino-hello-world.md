---
layout: post
title: Arduino hello world
category: arduino
image: /pics/arduino.png
---

<span class="right"><img src="/pics/arduino.png" width="170" alt=""/></span>

<span class="center"><img src="/pics/arduino-led-off.jpg" width="320" alt=""/></span>

<span class="center"><img src="/pics/arduino-led-on.jpg" width="320" alt=""/></span>

Using an [Arduino][1] board with a GNU/Linux personal computer has become [pretty straightforward][3].
The following command will install the required software:

{% highlight shell %}
sudo aptitude install arduino-mk
{% endhighlight %}

In order to get the file-system permissions to use the USB-serial it is necessary to add the current user to the *dialout* group.
It is necessary to log out and back in after this.

{% highlight shell %}
usermod $USER -a -G dialout
{% endhighlight %}

For the [Arduino Diecimila][2] you then create a *Makefile* with the following content
(see */usr/share/arduino/hardware/arduino/boards.txt* for other supported boards).

{% highlight make %}
ARDUINO_DIR = /usr/share/arduino
BOARD_TAG    = diecimila
ARDUINO_PORT = /dev/ttyUSB0
ARDUINO_LIBS =
include /usr/share/arduino/Arduino.mk
{% endhighlight %}

Make sure that */dev/ttyUSB0* is the right device!
You can do this by inspecting the output of *dmesg* after plugging in the Arduino device.

Now you can create a sketch file (*ino* file extension) for programming the micro controller.
*E.g.* a file *Blink.ino* with the following content:

{% highlight c %}
int led = 13;

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  digitalWrite(led, HIGH);
  delay(50);
  digitalWrite(led, LOW);
  delay(50);
}
{% endhighlight %}

Finally compile and upload the code:

{% highlight shell %}
make
make upload
{% endhighlight %}

The source is available on Github: [github.com/wedesoft/arduino-hello-world][6]

**Update:**

It is also possible to switch the LED according to instructions send over the USB serial channel.
The following program facilitates this:

{% highlight c %}
int led = 13;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  char c;
  if (Serial.available()) {
    c = Serial.read();
    if (c == '1') {
      digitalWrite(led, HIGH);
      Serial.write("LED on\r\n");
    } else if (c == '0') {
      digitalWrite(led, LOW);
      Serial.write("LED off\r\n");
    } else
      Serial.write("Switch LED on/off using keys '1' and '0'\r\n");
  };
}
{% endhighlight %}

After opening [*screen* as a serial terminal][7], the LED can be controlled using keys '1' and '0':

{% highlight shell %}
screen /dev/ttyUSB0 9600
{% endhighlight %}

Exit with *Ctrl-A \\*

**See also:**

* [Uploading a Sketch from the command line][3]
* [Control LEDs with lower level IO][4]
* [Three Alternatives to Arduino’s Serial Monitor][7]
* [Arduino ProtoShield][5]

[1]: https://www.arduino.cc/
[2]: https://www.arduino.cc/en/Main/ArduinoBoardDiecimila
[3]: http://www.jamesrobertson.eu/blog/2012/sep/20/uploading-a-sketch-from-the-comman.html
[4]: http://www.mightwerk.com/software/EB62E441-1C1B-43AC-9CEA-FF0AE8753833/index.html
[5]: https://learn.adafruit.com/adafruit-proto-shield-arduino/overview
[6]: https://github.com/wedesoft/arduino-hello-world
[7]: https://www.baldengineer.com/alternatives-to-arduinos-serial-monitor.html
