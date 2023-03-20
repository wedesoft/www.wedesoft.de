---
layout: post
title: Procedural generation of global cloud cover
category: software
---

Recently I started working on procedural generation of a cloud map.
Initially I applied [random rotation fields to a sphere with Worley noise and posted it on Reddit][1].
A helpful comment by [mr\_bitshift][3] pointed out the publication [Bridson et al. "Curl noise for procedural fluid flow"][4].
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
To obtain prevailing winds and vortices with different direction of rotation depending on the latitude one can use the function (1+sin(2.5\*latitute))/2 to mix positive and negative Worley noise.

![Mixing positive and negative Worley noise to obtain prevailing winds](/pics/prevailing-winds.png)

Below is a result obtained using the method described in this article.

![Example of resulting cloud cover](/pics/cloud-cover.jpg)

Also see [here](https://www.youtube.com/watch?v=dzGjDgvapfs) for a video.

See [cover.clj](https://github.com/wedesoft/sfsim25/blob/c26adc1a1bb04d2193885b88353375fa45d0d41f/etc/cover.clj) for source code.

Enjoy!

[1]: https://www.reddit.com/r/proceduralgeneration/comments/1150e4f/how_can_i_generate_realistic_planetary_cloud_cover/
[2]: https://www.reddit.com/r/proceduralgeneration/comments/118gbqq/how_to_generate_planetary_cloud_cover_using_curl/
[3]: https://www.reddit.com/user/mr_bitshift/
[4]: https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph2007-curlnoise.pdf
[5]: https://www.bit-101.com/blog/2021/07/curl-noise-demystified/
[6]: https://en.wikipedia.org/wiki/Worley_noise
[7]: https://en.wikipedia.org/wiki/Vector_projection
[8]: https://learnopengl.com/Advanced-OpenGL/Cubemaps
[9]: https://www.wedesoft.de/software/2022/07/01/tdd-with-opengl/
