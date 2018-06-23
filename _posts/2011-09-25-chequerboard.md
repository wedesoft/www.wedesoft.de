---
layout: post
title: Chequerboard Corner Detection
image: /pics/grid.jpg
---
<p style="text-align:center"><iframe id="ytplayer" type="text/html" width="508" height="406" allowfullscreen="allowfullscreen" src="http://www.youtube.com/embed/TmNVErH-yDI" frameborder="0">You need to enable IFrames to display this video.</iframe></p>

Camera calibration is a fundamental problem in computer vision. In the third episode of the **Computer Vision for the Robotic Age podcast** I am demonstrating a robust algorithm for identifying the corners of a chequerboard pattern. Identifying the corners of a calibration grid is the initial step for camera calibration.

{% highlight ruby %}
require 'rubygems'
require 'hornetseye_ffmpeg'
require 'hornetseye_xorg'
include Hornetseye
class Node
  # Non-maxima suppression for corners
  def nms(threshold)
    # compare image with dilated image and threshold
    self >= dilate.major(threshold)
  end
  # Find a component with 'n' corners
  def have(n, corners)
    # compute number of corners for each component
    hist = mask(corners).histogram max + 1
    # compare number of corners with desired number
    msk = hist.eq n
    if msk.inject :or
      # get label of connected component
      id = argmax { |i| msk.to_ubyte[i] }.first
      # get component mask
      eq id
    else
      # return nil
      nil
    end
  end
  def corners(sigma = 5.0, size = 21)
    size2 = size / 2
    # generate chequer pattern
    f1 = finalise(size, size) do |i,j|
      x, y = i - size2, j - size2
      x * y * Math.exp( -(x ** 2 + y ** 2) / sigma ** 2)
    end
    # generate chequer pattern rotated by 45°
    f2 = finalise(size, size) do |i,j|
      x, y = i - size2, j - size2
      0.5 * (x ** 2 - y ** 2) * Math.exp( -(x ** 2 + y ** 2) / sigma ** 2)
    end
    # apply filter pair and compute corner strength
    Math.hypot convolve(f1), convolve(f2)
  end
end
# relative threshold for corner image
CORNERS = 0.6
# absolute threshold for greylevel image
THRESHOLD = 90
# width and height of calibration grid
W, H = 8, 5
W2, H2 = 0.5 * (W - 1), 0.5 * (H - 1)
N = W * H
# dilation constant
GRID = 7
# open input video
input = AVInput.new 'calibration.avi'
width, height = input.width, input.height
X11Display.show do
  # read colour image
  img = input.read_ubytergb
  # convert to greyscale
  grey = img.to_ubyte
  # call method for computing corner image
  corners = grey.corners
  # call method for non-maxima suppression
  nms = corners.nms CORNERS * corners.max
  # threshold image
  binary = grey > THRESHOLD
  # find edges using dilation and erosion
  edges = binary.dilate(GRID).and binary.erode(GRID).not
  # find connected edges
  components = edges.components
  # check for edge region with 'N' corners
  grid = components.have N, nms
  if grid
    # visualise calibration grid
    grid.and(nms).dilate.conditional RGB(255, 255, 0), img
  else
    # visualise detected corners
    nms.dilate.conditional RGB(255, 0, 0), img
  end
end
{% endhighlight %}

You can download a better quality version of the video here
<ul>
  <li><a href="http://dl.dropbox.com/u/49280716/hornetseye-chequerboard.m4v">hornetseye-chequerboard.m4v</a> (39.21 MB)</li>
</ul>

**See also:**

* [Chequerboard corner detector on gist.github.com][6]
* [My earlier article about camera calibration][1]
* [Zhengyou Zhang's camera calibration][2]
* [Forsyth & Ponce: Computer Vision - A Modern Approach][3]

[1]: calibration.html
[2]: http://research.microsoft.com/en-us/um/people/zhang/calib/
[3]: http://www.amazon.co.uk/gp/product/0131911937/ref=as_li_ss_tl?ie=UTF8&tag=wedesoft-21&linkCode=as2&camp=1634&creative=19450&creativeASIN=0131911937
[4]: http://wedesoft.libsyn.com/webpage
[5]: http://wedesoft.libsyn.com/rss
[6]: https://gist.github.com/1241342

