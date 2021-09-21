---
layout: post
title: Reversed-Z Rendering in OpenGL
category: software
image: /pics/glxgears.png
---

By default the OpenGL 3D normalized device coordinates (NDC) are between -1 and +1.
x is from left to right and y is from bottom to top of the screen while the z-axis is pointing into the screen.

![normalized device coordinates](/pics/ndc.svg)

Usually the 3D camera coordinates of the object are mapped such that near coordinates are mapped to -1 and far values are mapped to +1.
The near and far plane define the nearest and farthest point visualized by the rendering pipeline.

The absolute accuracy of floating point values is highest when the values are near zero.
This leads to the depth accuracy to be [highest somewhere between the near and far plane][1] which is suboptimal.

A better solution is to map near coordinates to 1 and far coordinates to 0.
In this case the higher precision near zero is used to compensate for the large range of distances covered towards the far plane.

![zero to one mapping](/pics/zerotoone.svg)

Fortunately [OpenGL 4.5 provides a way to perform reversed-z rendering][2]:

* set glDepthFunc to GL\_GREATER or GL\_GEQUAL
* set glClipControl to GL\_LOWER\_LEFT and GL\_ZERO\_TO\_ONE
* set glClearDepth to 0.0
* use a suitable projection matrix

![using 0 to 1 z values](/pics/ndcltd.svg)

Let x, y, z, 1 be the homogeneous coordinate of the 3D point and x', y', z', w' the homogeneous NDC.
Normally the 3D point uses a right-handed coordinate system where the z-axis points out of the screen.

![frustum](/pics/frustum.svg)

This means that the x- and y-coordinate are projected using negative z, i.e. w' = -z.

The camera projection equations are x'/w' = fw x/-z and y'/w' = fh y/-z.

Substituting w' = -z we get x' = fw x and y' = fh y.

Setting z' = a + b z we get the following matrix equation:

{% latex usepackages=amsmath %}
$\begin{pmatrix} x^\prime \\ y^\prime \\ z^\prime \\ w^\prime \end{pmatrix} =
\begin{pmatrix} f_w & 0 & 0 & 0 \\ 0 & f_h & 0 & 0 \\ 0 & 0 & b & a \\ 0 & 0 & -1 & 0\end{pmatrix}\,
\begin{pmatrix}x \\ y \\ z \\ 1 \end{pmatrix}$
{% endlatex %}

When z = -near, we want z'/w' = 1.

When z = -far, we want z'/w' = 0.

For z = -near we get: z' = w' => a + b z = -z => a - b near = near.

For z = -far we get: z' = 0 => a + b z = 0 => a - b far = 0 <=> b = a / far.

Substituting b we get: a - a / far = near <=> a = near far / (far - near)

This means b = a / far = near / (far - near).

I.e. we get:

{% latex %}
$a = \frac{far \cdot near}{far - near}$
{% endlatex %}

and

{% latex %}
$b = \frac{near}{far - near}$
{% endlatex %}

Finally we set fw = 1 / tan(fov / 2) where fov is the horizontal field of view angle (e.g. 60 degrees in radians).

fh then needs to be chosen such that fh / fw = w / h, where w and h are the width and height of the screen.

[1]: https://developer.nvidia.com/content/depth-precision-visualized
[2]: https://nlguillemot.wordpress.com/2016/12/07/reversed-z-in-opengl/
