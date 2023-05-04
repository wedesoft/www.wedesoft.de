---
layout: post
title: Procedural Volumetric Clouds
category: software
image: /pics/bluenoiseoffsets.jpg
---

Volumetric clouds use 3D density functions to represent clouds in a realistic way.
Ray marching is used to generate photorealistic rendering.
With modern graphics cards it is possible to do this in realtime.

[Sebastian Lague's video on cloud rendering][1] shows how to generate Worley noise which can be used to generate realistic looking clouds.
Worley noise basically is a function which for each location returns the distance to the nearest point of a random set of points.
Usually the space is divided into cells with each cell containing one random point.
This improves the performance of determining the distance to the nearest point.
The following image shows a slice through inverted 3D Worley noise.

![Worley slice](/pics/worley.png)

Ray marching works by starting a ray for each render pixel and sampling the cloud volume which is a cube in this example.
This ray tracing program can be implemented in OpenGL by rendering a dummy background quad.

![Sampling](/pics/sampling.png)

The transmittance for a small segment of the cloud is basically the exponent of negative density times step size:

{% highlight glsl %}
vec3 cloud_scatter = vec3(0, 0, 0);
float transparency = 1.0;
for (int i=0; i<cloud_samples; i++) {
  vec3 c = origin + (i * stepsize + 0.5) * direction;
  float density = cloud_density(c);
  float transmittance_cloud = exp(-density * stepsize);
  float scatter_amount = 1.0;
  cloud_scatter += transparency * (1 - transmittance_cloud) * scatter_amount;
  transparency = transparency * transmittance_cloud;
};
incoming = incoming * transparency + cloud_scatter;
{% endhighlight %}

The resulting sampled cube of Worley noise looks like this:

![Worley cube](/pics/worleycube.jpg)

The amount of scattered light can be changed by using a mix of isotropic scattering and a phase function for approximating Mie scattering.
I.e. the amount of scattered light is computed as follows:

{% highlight glsl %}
  float scatter_amount = anisotropic * phase(0.76, dot(direction, light_direction)) + 1 - anisotropic;
{% endhighlight %}

The resulting rendering of the Worley noise now shows a bright halo around the sun:

![Anisotropic scattering](/pics/anisotropic.jpg)

The rendering does not include self-shadowing.
Shadows are usually computed by sampling rays towards the light source for each existing sample of the cloud cube.
However a more efficient way is to use [deep opacity maps][5].
In a similar fashion to shadow maps, a depth map of the start of the cloud is computed as seen from the light source.
While rendering the depth map, several samples of the opacity (or transmittance) behind the depth map are taken with a constant stepsize.
I.e. the opacity map consists of a depth (or offset) image and a 3D array of opacity (or transmittance) images.

![Deep opacity map](/pics/deepopacity.png)

Similar as when performing shadow mapping, one can perform lookups in the opacity map to determine the amount of shading at each sample in the cloud.

![Clouds with self-shading](/pics/cloudshadow.jpg)

To make the cloud look more realistic, one can add multiple octaves of Worley noise with decreasing amplitude.
This is also sometimes called fractal Brownian motion.

![Octaves of Worley noise](/pics/octaves.jpg)

To reduce sampling artifacts without loss of performance, one can use [blue noise][7] offsets for the sample positions when computing shadows as well as when creating the final rendering.

![Blue noise sampling offsets](/pics/bluenoiseoffsets.jpg)

In a previous article I have demonstrated how to [generate global cloud cover using curl noise][4].
One can add the global cloud cover with octaves of mixed [Perlin][6] and Worley noise and subtract a threshold.
Clamping the resulting value creates 2D cloud patterns on a spherical surface.

![Scattered global clouds](/pics/scatteredclouds.jpg)

By restricting the clouds to be between a bottom and top height, one obtains prism-like objects as shown below:

![Cloud blocks](/pics/cloudblocks.jpg)

Note that at this point it is recommended to use cascaded deep opacity maps instead of a single opacity map.

One can additionally multiply the clouds with a [vertical density profile][9].

![Vertical density profile](/pics/cloudprofile.jpg)

Guerilla Games uses a [remapping function][3] to introduce high frequency noise on the surfaces of the clouds.
The high frequency noise value is remapped using a range defined using the low frequency noise value.

{% highlight glsl %}
float density = clamp(remap(noise, 1 - base, 1.0, 0.0, cap), 0.0, cap);
{% endhighlight %}

The remapping function is defined as follows:

{% highlight glsl %}
float remap(float value, float original_min, float original_max, float new_min, float new_max)
{
  return new_min + (value - original_min) / (original_max - original_min) * (new_max - new_min);
}
{% endhighlight %}

An example obtained using these techniques is shown below:

![Remapping of noise](/pics/remapping.jpg)

Please let me know any suggestions and improvements!
<img src="/pics/bounce.gif" width="19" alt=""/>

Enjoy!

## Future work

* add atmospheric scattering with cloud shadows
* add planet surface and shadow map
* sampling with adaptive step size
* [Powder sugar effect][2]
* problems with shadow map at large distances
* problem with long shadows at dawn

[1]: https://www.youtube.com/watch?v=4QOcCGI6xOU
[2]: https://www.youtube.com/watch?v=Qj_tK_mdRcA
[3]: https://www.guerrilla-games.com/read/the-real-time-volumetric-cloudscapes-of-horizon-zero-dawn
[4]: https://www.wedesoft.de/software/2023/03/20/procedural-global-cloud-cover/
[5]: http://www.cemyuksel.com/research/deepopacity/
[6]: https://adrianb.io/2014/08/09/perlinnoise.html
[7]: https://www.wedesoft.de/software/2022/09/21/blue-noise-dithering/
[8]: https://inria.hal.science/inria-00288758/document
[9]: https://www.youtube.com/watch?v=p48rNiJBFG0
