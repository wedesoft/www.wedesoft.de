---
layout: post
title: X-Plane 12 Citation-X Checklist
category: simulation
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
  height: 5lh;
  overflow-y: auto;    /* Adds a vertical scrollbar only when necessary */
  border: 1px solid #ccc;
  padding: 10px;
}
</style>

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

If you are trying out the Citation-X with X-Plane 12, I can really recommend the [XChecklist plugin](https://forums.x-plane.org/files/file/20785-xchecklist-linwinmac/) and the [checklist file for the Citation-X](https://forums.x-plane.org/files/file/90301-xchecklist-for-laminar-citation-x/).
To help you with finding all the different controls, I created this interactive page.

(Work in progress)

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
<li>Aux Pump A: ON</li>
<li>Parking Brake: OFF</li>
<li>Parking Brake: ON</li>
<li>Aux Pump A: OFF</li>
<li>Seat Belt Lights: UP</li>
<li>Recognition Light: ON</li>
<li>Navigation Light: ON</li>
<li>Tail Flood Light: ON</li>
<li>Engine Bleed Air: BOTH HP/LP</li>
<li>Righthand Panel Lights: SET</li>
<li>APU Master: ON</li>
<li>Test Button: CHECK</li>
<li>APU Start: HOLD TILL START</li>
<li>APU N1: 100 RPM</li>
<li>APU Generator: ON</li>
<li>APU Bleed Air: MAX COOL</li>
<li>Cabin and Cockpit Packs: ON</li>
</ul>

<h4>PREFLIGHT PROCEDURE</h4>
<ul>
<li>Aircraft Doors: CLOSED</li>
<li>External Power: DISCONNECTED</li>
<li>Rotary Test: ROTATE TO ALL POSITIONS</li>
<li>Aux Pump A: ON</li>
<li>Flaps &amp; Slats: RETRACT</li>
<li>Aux Pump A: OFF</li>
<li>PFD Source Selection: SELECT FMS or NAV</li>
<li>Lateral Navigation Mode: NAV</li>
<li>Vertical Navigation Mode: FLC</li>
<li>Flight Level Change Vertical Speed: 180 KIAS</li>
<li>Yaw Damper: ON</li>
<li>Mach Trim: ON</li>
<li>Transponder Frecuency: SET</li>
<li>Transponder Mode: STANDBY</li>
<li>Heading: SET TO RUNWAY</li>
<li>Altitude Select: SET TO CLEARED ALT</li>
<li>Altimeter Baro: SET LOCAL QNH</li>
<li>Minimums BARO PFD: SET TO 1000FT AGL</li>
</ul>

<h4>ENGINE START</h4>
<ul>
<li>GND REC/ANTI-COLL Lights: ON</li>
<li>Cabin and Cockpit Packs: OFF</li>
<li>Engine Start RH Button: PRESS</li>
<li>RH N2%: WAIT >10</li>
<li>Right Power Lever: IDLE</li>
<li>RH N2%: WAIT >60</li>
<li>Engine Start LH Button: PRESS</li>
<li>LH N2%: WAIT >10</li>
<li>Left Power Lever: IDLE</li>
<li>Eicas Fuel Page: CHECK FLOW >500 PPH</li>
<li>Eicas Hyd Page: CHECK PSI>3000</li>
<li>Engine Bleed Air: BOTH LP</li>
<li>Cabin and Cockpit Packs: On</li>
</ul>

<h4>BEFORE TAXI</h4>
<ul>
<li>Pressurization Switches: ALL THREE NORM</li>
<li>Pressurization Altitude: SET</li>
<li>Pitot Static Switches: BOTH UP</li>
<li>Flaps 5 or 15: SET</li>
<li>Pitch Trim: SET GREEN</li>
<li>Rudder &amp; Aileron Trim: CHECK NEUTRAL</li>
<li>Yoke Roll Control: FREE MOVEMENT</li>
<li>Yoke Pitch Control: FREE MOVEMENT</li>
<li>Rudder Control: FREE MOVEMENT</li>
<li>Speedbrake Handle: CHECK MOVEMENT THEN ZERO</li>
<li>Taxi Light: ON</li>
</ul>

<h4>TAXI</h4>
<ul>
<li>Parking Brake: RELEASED</li>
<li>Parking Brake: SET</li>
</ul>

<h4>BEFORE TAKEOFF</h4>
<ul>
<li>GND REC/ANTI-COLL Lights: UP</li>
<li>Landing Lights: ON</li>
<li>Taxi Light: OFF</li>
<li>Transponder: ALT</li>
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
<li>Autopilot: ENGAGE</li>
<li>Vertical Navigation Mode: FLC or VNAV</li>
<li>Flaps &amp; Slats: UP AS REQUIRED</li>
<li>Throttle Lever: CLB POSITION</li>
<li>Cabin Pressurization: CHECK POSITIVE FPM</li>
<li>Altimeter Baro: STD AT TRANS ALT</li>
<li>==>: Above 10000ft</li>
<li>Landing Lights: OFF</li>
<li>Seat Belts: OFF</li>
<li>APU Bleed Air: OFF</li>
<li>APU Generator: OFF</li>
<li>APU Start: HOLD DOWN</li>
<li>==>: APU RPM ZERO</li>
<li>APU Master: OFF</li>
<li>==>: Above 18000ft</li>
<li>ENG SYNC: FAN</li>
</ul>

<h4>CRUISE</h4>
<ul>
<li>BANK: AS REQUIRED</li>
<li>Throttle Lever: CRU POSITION</li>
</ul>

<h4>DESCENT</h4>
<ul>
<li>Altitude Knob: SET TO CLEARED ALT</li>
<li>Vertical Navigation Mode: SET VNAV, FLC or VS</li>
<li>==>: Below 18000ft</li>
<li>ENG SYNC: OFF</li>
<li>BANK: AS REQUIRED</li>
<li>APU Master: ON</li>
<li>APU Start: HOLD TILL START</li>
<li>APU N1 100%: READY</li>
<li>APU Generator: ON</li>
<li>APU Bleed Air: MAX COOL</li>
<li>==>: Below 10000ft</li>
<li>Landing Lights: ON</li>
<li>Seat Belts Lights: ON</li>
<li>Altimeter Baro: SET LOCAL QNH</li>
<li>RA/BARO Minimums: SET</li>
</ul>

<h4>APPROACH &amp; LANDING</h4>
<ul>
<li>==>: SPEED <250 KIAS</li>
<li>Slats &amp; Flaps: FLAPS 5</li>
<li>Autopilot Approach Mode: ARM</li>
<li>==>: SPEED <210 KIAS</li>
<li>Flaps: FLAPS 15</li>
<li>Landing Gear: DOWN 3 GREEN</li>
<li>==>: SPEED 180 KIAS</li>
<li>Flaps: FLAPS 35</li>
<li>==>: AGL <500 ft</li>
<li>AP &amp; YD: DISCONNECT</li>
<li>Throttle Lever: IDLE</li>
<li>Reverse Throttle: IF NECESSARY</li>
<li>==>: GND SPEED <50 KIAS</li>
<li>Reverse Throttle: OFF</li>
</ul>

<h4>AFTER LANDING</h4>
<ul>
<li>Pitot Static Heat: BOTH OFF</li>
<li>GND REC/ANTI-COLL Lights: GND REC</li>
<li>Transponder Mode: STANDBY</li>
<li>Taxi Light: ON</li>
<li>Landing Lights: OFF</li>
<li>Speedbrakes: RETRACTED</li>
<li>Flaps: UP</li>
</ul>

<h4>SHUTDOWN</h4>
<ul>
<li>Taxi Light: OFF</li>
<li>Parking Brake: SET</li>
<li>Throttles: CUT OFF</li>
<li>Engines N2%: Below 10%</li>
<li>GND REC/ANTI-COLL Lights: OFF</li>
<li>Aircraft Doors: OPEN</li>
<li>Seat Belts: OFF</li>
<li>APU Bleed Air: OFF</li>
<li>APU Generator: OFF</li>
<li>APU Start: HOLD DOWN</li>
<li>==>: APU RPM ZERO</li>
<li>APU Master: OFF</li>
<li>Engine Bleed Air: BOTH OFF</li>
<li>Cabin and Cockpit Packs: OFF</li>
<li>Recognition Light: OFF</li>
<li>Navigation Light: OFF</li>
<li>Tail Flood Light: OFF</li>
<li>Windshiel Heat LH &amp; RH: OFF</li>
<li>Passenger Oxygen: OFF</li>
<li>Emergency Exit Lights: OFF</li>
<li>Center Wing Transfer: BOTH OFF</li>
<li>Fuel Boost: BOTH OFF</li>
<li>Standby Power: OFF</li>
<li>Avionics Switch: OFF</li>
<li>Eicas Switc^Mh: OFF</li>
<li>Generators 1 &amp; 2: OFF</li>
<li>Battery Switches: BOTH OFF</li>
</ul>

</div>
