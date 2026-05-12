---
layout: post
title: X-Plane 12 Citation-X Checklist
category: simulation
image: /pics/cockpit.jpg
---

<style>
.image-container {
  position: relative;
  width: 100%;
  display: inline-block;
}
#cockpit-image {
  display: block;
}
.image-container, #cockpit-image {
  padding: 0;
  margin: 0;
  border: none;
}
#bounding-box {
  position: absolute;
  border: 3px solid #ffeb3b;
  background-color: rgba(255, 235, 59, 0.2);
  display: none;
  box-sizing: border-box;
  transition: all 0.3s ease;
}
.trigger-link {
  color: #268bd2;
  text-decoration: none;
  user-select: none; 
  -webkit-user-select: none;
  -ms-user-select: none;
  cursor: pointer;
}
.trigger-link:hover,
.trigger-link:focus {
  text-decoration: underline;
}
.scroll-box {
  width: 100%;        /* Define the width of your box */
  height: 8lh;
  overflow-y: auto;    /* Adds a vertical scrollbar only when necessary */
  border: 1px solid #ccc;
  padding: 10px;
}
</style>

If you are trying out the Citation-X with X-Plane 12, I can really recommend the [XChecklist plugin](https://forums.x-plane.org/files/file/20785-xchecklist-linwinmac/) and the [checklist file for the Citation-X](https://forums.x-plane.org/files/file/90301-xchecklist-for-laminar-citation-x/).
To help you with finding all the different controls, I created this interactive page.

<div class="image-container">
    <img id="cockpit-image" src="/pics/citation-x-1.jpg" alt="cockpit view">
    <div id="bounding-box"></div>
</div>

<script>
  const img_element = document.getElementById('cockpit-image');
  const box_element = document.getElementById('bounding-box');

  function cockpit_show(url, orig_width, orig_height, x, y, w, h) {
    img_element.src = url;
    const left = (x / orig_width) * 100;
    const top = (y / orig_height) * 100;
    const width = (w / orig_width) * 100;
    const height = (h / orig_height) * 100;
    box_element.style.display = 'block';
    box_element.style.left = left + '%';
    box_element.style.top = top + '%';
    box_element.style.width = width + '%';
    box_element.style.height = height + '%';
  }
</script>

<div class="scroll-box">
<h4>PRELIMINARY</h4>
<ul>
<li>Aircraft Doors: OPEN</li>
<li>Parking Brake: SET</li>
<li>Landing Gear Handle: DOWN</li>
<li>Speedbrake Handle: CHECK ZERO</li>
<li>Throttles: CUT OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 926, 957, 88, 86)">Battery Switches 1 &amp; 2</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1166, 962, 37, 81)">Eicas Switch</span>: ON</li>
<li>Battery Volts (Elec Page): CHECK 24</li>
<li>Refuel: AS NEEDED</li>
<li>Fuel Balance: CHECK IMBALANCE <200 lbs</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1166, 962, 37, 81)">Eicas Switch</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 926, 957, 88, 86)">Battery Switches 1 &amp; 2</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1112, 1134, 108, 76)">Standby Power</span>: TEST</li>
<li>Passengers & Cargo: BOARDED & CHECK</li>
</ul>

<h4>COCKPIT PREPARATION 1</h4>
<ul>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 926, 957, 88, 86)">Battery Switches 1 &amp; 2</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1061, 958, 43, 85)">External Power</span> if Available: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 880, 959, 45, 81)">Left</span> &amp; <span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1016, 958, 43, 81)">Right Generator</span>: UP</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1166, 962, 37, 81)">Eicas Switch</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1112, 960, 46, 83)">Avionics Switch</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 850, 1057, 101, 75)">Fuel Boost</span>: BOTH NORM or ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 971, 1140, 53, 74)">FADEC Left Switch</span>: RESET &amp; NORMAL</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1038, 1140, 53, 74)">FADEC Right Switch</span>: RESET &amp; NORMAL</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1115, 1048, 111, 81)">Ignition Switches</span>: CHECK NORM</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1109, 1134, 112, 79)">Standby Power</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 569, 1054, 99, 75)">Center Wing Transfer</span>: BOTH NORM</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 361, 961, 78, 80)">Emergency Exit Lights</span>: ON (Red Light On)</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 361, 961, 78, 80)">Emergency Exit Lights</span>: ARM</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 352, 1051, 154, 165)">Instrument Lights</span> (FLOOD LTS, EL, LH, CTR): SET AS NEED</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 16, 1100, 117, 103)">Passenger Oxygen</span>: AUTO</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 728, 258, 89, 71)">Master Warning</span>: RESET</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 817, 258, 89, 71)">Master Caution</span>: RESET</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1144, 1041, 90, 84)">Windshield Heater LH &amp; RH</span>: ON</li>
</ul>

<h4>COCKPIT PREPARATION 2</h4>
<ul>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1464, 957, 47, 82)">Aux Pump A</span>: ON</li>
<li>Parking Brake: OFF</li>
<li>Parking Brake: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1464, 957, 47, 82)">Aux Pump A</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1593, 1052, 67, 78)">Seat Belt Lights</span>: UP</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1495, 1137, 51, 77)">Recognition Lights</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1549, 1137, 51, 77)">Navigation Lights</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1601, 1137, 59, 69)">Tail Flood Lights</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2048, 1147, 269, 87)">Engine Bleed Air</span>: BOTH HP/LP</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2100, 1250, 80, 92)">Righthand Panel Lights</span>: SET</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1355, 855, 62, 99)">APU Master</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1295, 846, 44, 86)">Test Button</span>: CHECK</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1234, 822, 58, 98)">APU Start</span>: HOLD TILL START</li>
<li>APU N1: 100 RPM</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1354, 581, 74, 110)">APU Generator</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1173, 542, 77, 117)">APU Bleed Air</span>: MAX COOL</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2224, 1061, 94, 86)">Cabin</span> and <span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2039, 1059, 95, 88)">Cockpit Packs</span>: ON</li>
</ul>

<h4>PREFLIGHT PROCEDURE</h4>
<ul>
<li>Aircraft Doors: CLOSED</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1061, 958, 43, 85)">External Power</span> if Available: DISCONNECTED</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1442, 523, 170, 178)">Rotary Test</span>: ROTATE TO ALL POSITIONS</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1464, 957, 47, 82)">Aux Pump A</span>: ON</li>
<li>Flaps &amp; Slats: RETRACT</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1464, 957, 47, 82)">Aux Pump A</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 661, 384, 116, 34)">PFD Source Selection</span>: SELECT FMS or NAV</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1578, 108, 79, 179)">Lateral Navigation Mode</span>: NAV</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1744, 112, 67, 44)">Vertical Navigation Mode</span>: FLC</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1827, 99, 67, 188)">Flight Level Change Vertical Speed</span>: 180 KIAS</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1911, 157, 70, 44)">Yaw Damper</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1903, 203, 68, 43)">Mach Trim</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 699, 254, 43, 29)">Transponder Frequency</span>: SET</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 696, 284, 43, 29)">Transponder Mode</span>: STANDBY</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 959, 1254, 126, 110)">Heading</span>: SET TO RUNWAY</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1345, 1250, 97, 105)">Altitude Select</span>: SET TO CLEARED ALT</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 930, 885, 49, 63)">Altimeter Baro</span>: SET LOCAL QNH</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 632, 885, 49, 63)">Minimums BARO PFD</span>: SET TO 1000FT AGL</li>
</ul>

<h4>ENGINE START</h4>
<ul>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1530, 1048, 64, 81)">GND REC/ANTI-COLL Lights</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2224, 1061, 94, 86)">Cabin</span> and <span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2039, 1059, 95, 88)">Cockpit Packs</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1052, 1067, 48, 51)">Engine Start RH Button</span>: PRESS</li>
<li>RH N2%: WAIT >10</li>
<li>Right Power Lever: IDLE</li>
<li>RH N2%: WAIT >60</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 969, 1067, 48, 51)">Engine Start LH Button</span>: PRESS</li>
<li>LH N2%: WAIT >10</li>
<li>Left Power Lever: IDLE</li>
<li>Eicas Fuel Page: CHECK FLOW >500 PPH</li>
<li>Eicas Hyd Page: CHECK PSI>3000</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2048, 1147, 269, 87)">Engine Bleed Air</span>: BOTH LP</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2224, 1061, 94, 86)">Cabin</span> and <span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2039, 1059, 95, 88)">Cockpit Packs</span>: ON</li>
</ul>

<h4>BEFORE TAXI</h4>
<ul>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1782, 1124, 159, 85)">Pressurization Switches</span>: ALL THREE NORM</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1839, 944, 180, 151)">Pressurization Altitude</span>: SET</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1145, 955, 90, 85)">Pitot Static Switches</span>: BOTH UP</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1475, 752, 61, 55)">Flaps 5 or 15</span>: SET</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1475, 671, 63, 78)">Pitch Trim</span>: SET GREEN</li>
<li>Rudder &amp; Aileron Trim: CHECK NEUTRAL</li>
<li>Yoke Roll Control: FREE MOVEMENT</li>
<li>Yoke Pitch Control: FREE MOVEMENT</li>
<li>Rudder Control: FREE MOVEMENT</li>
<li>Speedbrake Handle: CHECK MOVEMENT THEN ZERO</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1212, 1052, 68, 117)">Taxi Light</span>: ON</li>
</ul>

<h4>TAXI</h4>
<ul>
<li>Parking Brake: RELEASED</li>
<li>Parking Brake: SET</li>
</ul>

<h4>BEFORE TAKEOFF</h4>
<ul>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1530, 1048, 64, 81)">GND REC/ANTI-COLL Lights</span>: UP</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1281, 1052, 116, 121)">Landing Lights</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1212, 1052, 68, 117)">Taxi Light</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 696, 284, 43, 29)">Transponder</span>: ALT</li>
<li>Parking Brake: RELEASED</li>
<li>Pedal Brakes: FULL PRESS</li>
</ul>

<h4>TAKEOFF</h4>
<ul>
<li>Throttle Lever: SET 40% N1</li>
<li>Both Engines Simultaneous: CHECK</li>
<li>Pedal Brakes: RELEASE</li>
<li>Throttle Lever: MAX POSITION T/O</li>
<li>At Vr Speed: ROTATE</li>
</ul>

<h4>AFTER TAKEOFF</h4>
<ul>
<li>==>: Positive Rating Climb</li>
<li>Landing Gear: UP</li>
<li>==>: Above 1000ft AGl</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1924, 113, 64, 45)">Autopilot</span>: ENGAGE</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1744, 112, 67, 44)">Vertical Navigation Mode</span>: FLC or VNAV</li>
<li>Flaps &amp; Slats: UP AS REQUIRED</li>
<li>Throttle Lever: CLB POSITION</li>
<li>Cabin Pressurization: CHECK POSITIVE FPM</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 930, 885, 49, 63)">Altimeter Baro</span>: STD AT TRANS ALT</li>
<li>==>: Above 10000ft</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1281, 1052, 116, 121)">Landing Lights</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1593, 1052, 67, 78)">Seat Belt Lights</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1173, 542, 77, 117)">APU Bleed Air</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1354, 581, 74, 110)">APU Generator</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1234, 822, 58, 98)">APU Start</span>: HOLD DOWN</li>
<li>==>: APU RPM ZERO</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1355, 855, 62, 99)">APU Master</span>: OFF</li>
<li>==>: Above 18000ft</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1041, 1055, 182, 120)">ENG SYNC</span>: FAN</li>
</ul>

<h4>CRUISE</h4>
<ul>
<li>BANK: AS REQUIRED</li>
<li>Throttle Lever: CRU POSITION</li>
</ul>

<h4>DESCENT</h4>
<ul>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1345, 1250, 97, 105)">Altitude Knob</span>: SET TO CLEARED ALT</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1744, 112, 67, 44)">Vertical Navigation Mode</span>: SET VNAV, FLC or VS</li>
<li>==>: Below 18000ft</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1041, 1055, 182, 120)">ENG SYNC</span>: OFF</li>
<li>BANK: AS REQUIRED</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1355, 855, 62, 99)">APU Master</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1234, 822, 58, 98)">APU Start</span>: HOLD TILL START</li>
<li>APU N1 100%: READY</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1354, 581, 74, 110)">APU Generator</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1173, 542, 77, 117)">APU Bleed Air</span>: MAX COOL</li>
<li>==>: Below 10000ft</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1281, 1052, 116, 121)">Landing Lights</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1593, 1052, 67, 78)">Seat Belt Lights</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 930, 885, 49, 63)">Altimeter Baro</span>: SET LOCAL QNH</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 632, 885, 49, 63)">RA/BARO Minimums</span>: SET</li>
</ul>

<h4>APPROACH &amp; LANDING</h4>
<ul>
<li>==>: SPEED <250 KIAS</li>
<li>Slats &amp; Flaps: FLAPS 5</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1584, 204, 66, 41)">Autopilot Approach Mode</span>: ARM</li>
<li>==>: SPEED <210 KIAS</li>
<li>Flaps: FLAPS 15</li>
<li>Landing Gear: DOWN 3 GREEN</li>
<li>==>: SPEED 180 KIAS</li>
<li>Flaps: FLAPS 35</li>
<li>==>: AGL <500 ft</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1924, 113, 64, 45)">AP</span> &amp; <span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1911, 157, 70, 44)">YD</span>: DISCONNECT</li>
<li>Throttle Lever: IDLE</li>
<li>Reverse Throttle: IF NECESSARY</li>
<li>==>: GND SPEED <50 KIAS</li>
<li>Reverse Throttle: OFF</li>
</ul>

<h4>AFTER LANDING</h4>
<ul>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1145, 955, 90, 85)">Pitot Static Heat</span>: BOTH OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1530, 1048, 64, 81)">GND REC/ANTI-COLL Lights</span>: GND REC</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 696, 284, 43, 29)">Transponder Mode</span>: STANDBY</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1212, 1052, 68, 117)">Taxi Light</span>: ON</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1281, 1052, 116, 121)">Landing Lights</span>: OFF</li>
<li>Speedbrakes: RETRACTED</li>
<li>Flaps: UP</li>
</ul>

<h4>SHUTDOWN</h4>
<ul>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-3.jpg', 2560, 1440, 1212, 1052, 68, 117)">Taxi Light</span>: OFF</li>
<li>Parking Brake: SET</li>
<li>Throttles: CUT OFF</li>
<li>Engines N2%: Below 10%</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1530, 1048, 64, 81)">GND REC/ANTI-COLL Lights</span>: OFF</li>
<li>Aircraft Doors: OPEN</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1593, 1052, 67, 78)">Seat Belts</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1173, 542, 77, 117)">APU Bleed Air</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1354, 581, 74, 110)">APU Generator</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1234, 822, 58, 98)">APU Start</span>: HOLD DOWN</li>
<li>==>: APU RPM ZERO</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-4.jpg', 2560, 1440, 1355, 855, 62, 99)">APU Master</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2048, 1147, 269, 87)">Engine Bleed Air</span>: BOTH OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2224, 1061, 94, 86)">Cabin</span> and <span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 2039, 1059, 95, 88)">Cockpit Packs</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1495, 1137, 51, 77)">Recognition Lights</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1549, 1137, 51, 77)">Navigation Lights</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1601, 1137, 59, 69)">Tail Flood Lights</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1144, 1041, 90, 84)">Windshield Heater LH &amp; RH</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 16, 1100, 117, 103)">Passenger Oxygen</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 361, 961, 78, 80)">Emergency Exit Lights</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 569, 1054, 99, 75)">Center Wing Transfer</span>: BOTH OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 850, 1057, 101, 75)">Fuel Boost</span>: BOTH OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1109, 1134, 112, 79)">Standby Power</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1112, 960, 46, 83)">Avionics Switch</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1166, 962, 37, 81)">Eicas Switch</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 880, 959, 45, 81)">Left</span> &amp; <span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1016, 958, 43, 81)">Right Generator</span>: OFF</li>
<li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 926, 957, 88, 86)">Battery Switches 1 &amp; 2</span>: BOTH OFF</li>
</ul>

</div>
