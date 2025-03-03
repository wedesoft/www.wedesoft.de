---
layout: post
title: Procedural generation of global cloud cover
category: software
image: /pics/cloud-cover.jpg
---

This article is about generating realistic planetary cloud cover.
In an initial attempt I applied [random rotation fields to a sphere with Worley noise and posted it on Reddit][1].
However the results did not look convincing.
A helpful comment by [mr\_bitshift][3] pointed out the publication [Bridson et al. "Curl noise for procedural fluid flow"][4].
Also in a response to a later [Reddit post][2] [smcameron][11] shared an impressive result of a [gas giant generated using curl noise][10].

In two dimensions curl noise is fairly easy to understand and implement.
For a thorough description of 2D curl noise see [Keith Peters' article "Curl noise, demystified"][5].
Basically one starts with a potential field such as multiple octaves of [Worley noise][6].
One then extracts the 2D gradient vectors and rotates them by 90°.

![Curl vectors](/pics/curl-vectors.png)

To generate curl vectors for a spherical surface one can use 3D Worley noise and sample the gradients on the surface of the sphere.
The gradient vectors then need to be projected onto the sphere.
This can be achieved by projecting the gradient vector onto the local normal vector of the sphere using [vector projection][7].
By subtracting the projected vector from the gradient vector one obtains the tangential component of the gradient vector.

![Project vector onto sphere](/pics/project-vector.png)

{% latex %}
$\vec{p} = \vec{v}-\displaystyle\frac{\vec{v}^\top\vec{n}}{\vec{n}^\top\vec{n}}\vec{n}$
{% endlatex %}

The resulting vector *p* needs to be rotated around the normal *n* by 90°.
This can be achieved by rotating the vector *p* into a TBN system, rotating by 90° around N and then transforming back.
The GLSL functions for the rotation (without [OpenGL tests][9]) are shown below:

{% highlight glsl %}
#version 410 core

vec3 orthogonal_vector(vec3 n)
{
  vec3 b;
  if (abs(n.x) <= abs(n.y)) {
    if (abs(n.x) <= abs(n.z))
      b = vec3(1, 0, 0);
    else
      b = vec3(0, 0, 1);
  } else {
    if (abs(n.y) <= abs(n.z))
      b = vec3(0, 1, 0);
    else
      b = vec3(0, 0, 1);
  };
  return normalize(cross(n, b));
}

mat3 oriented_matrix(vec3 n)
{
  vec3 o1 = orthogonal_vector(n);
  vec3 o2 = cross(n, o1);
  return transpose(mat3(n, o1, o2));
}

// note that axis needs to be a unit vector
vec3 rotate_vector(vec3 axis, vec3 v, float cos_angle, float sin_angle)
{
  mat3 orientation = oriented_matrix(axis);
  vec3 oriented = orientation * v;
  mat2 rotation = mat2(cos_angle, sin_angle, -sin_angle, cos_angle);
  vec3 rotated = vec3(oriented.x, rotation * oriented.yz);
  return transpose(orientation) * rotated;
}
{% endhighlight %}


In OpenGL one can create a [cubemap][8] where each pixel on each surface contains a 3D warp vector.

* Using a fragment shader the cubemap is initialised to be an identity transform for unit vectors.
* A second fragment shader is used to initialise a cubemap with the curl vectors which are tangential to the sphere.
* A third fragment shader is called multiple times to renormalize and increment the identity transform to become a warp field.
* A final fragment shader uses the cubemap warp field to perform lookups in a 3D Worley noise field to generate a cubemap of the global cloud cover.

![Update warp vectors](/pics/warp-vectors.png)

If one uses octaves of Worley noise one obtains vortices rotating in one direction.
To obtain prevailing winds and vortices with different direction of rotation depending on the latitude one can use the function (1+sin(2.5\*latitude))/2 to mix positive and negative Worley noise.

![Mixing positive and negative Worley noise to obtain prevailing winds](/pics/prevailing-winds.png)

Below is a result obtained using the method described in this article.

![Example of resulting cloud cover](/pics/cloud-cover.jpg)

Also see [here](https://www.youtube.com/watch?v=dzGjDgvapfs) for a video.

See [cover.clj](https://github.com/wedesoft/sfsim25/blob/dd0ddd7e8f0b218664232ce8e228e2c1b8b54596/etc/cover.clj) for source code.

Enjoy!

## Update

Another detail I forgot to mention is that the fragment shaders and the cubemap texture lookups use modified vectors to avoid performing lookups in the texture clamping regions which would lead to seams in the cloud cover.
I.e. when converting fragment coordinates, one increases the range of the index by half a pixel on both ends:

{% highlight glsl %}
vec2 x = (gl_FragCoord.xy - 0.5) / (size - 1);
{% endhighlight %}

Furthermore when performing lookups, two coordinates of the lookup vector are scaled down by half a pixel:

{% highlight glsl %}
vec3 convert_cubemap_index(vec3 idx, int size)
{
  vec3 scale;
  if (abs(idx.x) >= abs(idx.y)) {
    if (abs(idx.x) >= abs(idx.z))
      scale = vec3(size, size - 1, size - 1);
    else
      scale = vec3(size - 1, size - 1, size);
  } else {
    if (abs(idx.y) >= abs(idx.z))
      scale = vec3(size - 1, size, size - 1);
    else
      scale = vec3(size - 1, size - 1, size);
  };
  return idx * scale / size;
}
{% endhighlight %}

The following picture illustrates the two related conversions.

![Index conversions for cubemaps](/pics/cubemap-index.png)

[1]: https://www.reddit.com/r/proceduralgeneration/comments/1150e4f/how_can_i_generate_realistic_planetary_cloud_cover/
[2]: https://www.reddit.com/r/proceduralgeneration/comments/118gbqq/how_to_generate_planetary_cloud_cover_using_curl/
[3]: https://www.reddit.com/user/mr_bitshift/
[4]: https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph2007-curlnoise.pdf
[5]: https://www.bit-101.com/blog/2021/07/curl-noise-demystified/
[6]: https://en.wikipedia.org/wiki/Worley_noise
[7]: https://en.wikipedia.org/wiki/Vector_projection
[8]: https://learnopengl.com/Advanced-OpenGL/Cubemaps
[9]: https://www.wedesoft.de/software/2022/07/01/tdd-with-opengl/
[10]: https://smcameron.github.io/space-nerds-in-space/gaseous-giganticus-slides/slideshow.html
[11]: https://www.reddit.com/user/smcameron/
