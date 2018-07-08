var image = document.getElementById("flip");
var currentPos = 0;
var images = ["/pics/arduino-led-off.jpg","/pics/arduino-led-on.jpg"]

function flip_image() {
  currentPos = 1 - currentPos;
  image.src = images[currentPos];
}

setInterval(flip_image, 1000);
