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

<ul>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 926, 957, 88, 86)">Battery Switches 1 &amp; 2</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1166, 962, 37, 81)">Eicas Switch</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1112, 1134, 108, 76)">Standby Power</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1061, 958, 43, 85)">External Power</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 880, 959, 45, 81)">Left Generator</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1016, 958, 43, 81)">Right Generator</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1112, 960, 46, 83)">Avionics Switch</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 850, 1057, 101, 75)">Fuel Boost</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 971, 1140, 53, 74)">FADEC Left Switch</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1038, 1140, 53, 74)">FADEC Right Switch</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1115, 1048, 111, 81)">Ignition Switches</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 1109, 1134, 112, 79)">Standby Power</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 569, 1054, 99, 75)">Center Wing Transfer</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 361, 961, 78, 80)">Emergency Exit Lights</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 352, 1051, 154, 165)">Instrument Lights</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 16, 1100, 117, 103)">Passenger Oxygen</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 728, 258, 89, 71)">Master Warning</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-1.jpg', 2560, 1440, 817, 258, 89, 71)">Master Caution</span></li>
  <li><span class="trigger-link" onclick="cockpit_show('/pics/citation-x-2.jpg', 2560, 1440, 1144, 1041, 90, 84)">Windshield Heater LH &amp; RH</span></li>
</ul>
