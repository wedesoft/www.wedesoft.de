---
layout: post
title: Keyestudio Smart Home
category: hardware
image: /pics/keyestudio-smarthome.jpg
---

![Keyestudio Smarthome](/pics/keyestudio-smarthome.jpg)

A few months ago I bought a Keyestudio Smart Home, assembled it and tried to program it using the Arduino IDE.
However I kept getting the following error when trying to upload a sketch to the board.


     avrdude: stk500_getsync() attempt 1 of 10: not in sync: resp=0x2e
     avrdude: stk500_getsync() attempt 2 of 10: not in sync: resp=0x2e
     avrdude: stk500_getsync() attempt 3 of 10: not in sync: resp=0x2e


Initially I thought it was an issue with the QinHeng Electronics CH340 serial converter driver software.
After exchanging a few emails with [keyestudio support][2] however I was pointed out that the board type of my smart home version was not "Arduino Uno".
The box of the control board says "Keyestudio Control Board for ESP-32" and I had to [install version 3.1.3 of the esp32 board software][3] for being able to program the board.
The documentation for the Keyestudio smart home using ESP-32 is [here][1].
Also the correct version of the [smart home sketches are here][4].
I.e. the Keyestudio IoT Smart Home Kit for ESP32 is not to be confused with the Keyestudio Smart Home Kit for Arduino.

Many thanks to Keyestudio support for helping me to get it working.

[1]: https://docs.keyestudio.com/projects/KS5009/en/latest/index.html
[2]: mailto:service@keyestudio.com
[3]: https://docs.keyestudio.com/projects/KS5009/en/latest/docs/Arduino/arduino.html#add-the-esp32-environment-add-version-3-1-0
[4]: https://docs.keyestudio.com/projects/KS5009/en/latest/docs/Arduino/arduino.html#resource-compression-package
