---
layout: post
title: Camera Calibration
category: ai
image: /pics/calibration.png
---

{% youtube "http://www.youtube.com/watch?v=rJVEvBDyVsE" %}

I am currently working on camera calibration. Many implementations require the user to manuallly point out corners. Here is an idea on how to detect and label corners automatically.

1. Apply [Otsu Thresholding][2] to input image.
2. Take difference of [dilated and eroded][3] image to get edge regions.
3. Label [connected components][4].
4. Compute [corners][5] of input image (and use non-maxima suppression).
5. Count corners in each component.
6. Look for a component which contains exactly 40 corners.
7. Get largest component of inverse of grid (i.e. the surroundings).
8. Grow that component and find all corners on it (*i.e.* corners on the boundary of the grid).
9. Find centre of gravity of all corners and compute vectors from centre to each boundary corner.
10. Sort boundary corners by angle of those vectors.
11. Use non-maxima suppression on list of length of vectors to get the 4 "corner corners" (convexity).
12. Use the locations of the 4 "corner corners" to compute a [planar homography][6] mapping the image coordinates of the 8 times 5 grid to the ranges 0..7 and 0..4 respectively.
13. Use the homography to transform the 40 corners and round the coordinates.
14. Order the points using the rounded coordinates.

Further work is about taking several images to perform the actual camera calibration.

Thanks to [Manuel Boissenin][8] for suggesting convexity for finding the "corner corners".

**Update:**

{% youtube "http://www.youtube.com/watch?v=YFKULC0efHc" %}

After calibrating the camera the ratio of focal length to pixel size is known (also see [Zhengyou Zhang's camera calibration][10]). Once the camera is calibrated, it is possible to estimate the 3D pose of the calibration grid in every frame.

I have created a [screencast on how to locate the chequerboard calibration pattern][11].

**See also:**

* [HornetsEye][1]
* [Zhengyou Zhang camera calibration][6]
* [Computer Vision: A Modern Approach][9]

[1]: https://www.wedesoft.de/hornetseye-api/
[2]: http://en.wikipedia.org/wiki/Otsu%27s_method
[3]: http://en.wikipedia.org/wiki/Mathematical_morphology
[4]: http://en.wikipedia.org/wiki/Connected_Component_Labeling
[5]: http://en.wikipedia.org/wiki/Corner_detection
[6]: http://research.microsoft.com/en-us/um/people/zhang/calib/
[8]: http://fr.linkedin.com/pub/manuel-boissenin/a/444/69
[9]: http://www.amazon.co.uk/gp/product/0131911937/ref=as_li_ss_tl?ie=UTF8&tag=wedesoft-21&linkCode=as2&camp=1634&creative=19450&creativeASIN=0131911937
[10]: http://research.microsoft.com/en-us/um/people/zhang/calib/
[11]: https://www.wedesoft.de/ai/2011/09/25/chequerboard/
