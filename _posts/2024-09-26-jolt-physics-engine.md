---
layout: post
title: Getting started with the Jolt Physics Engine
category: simulation
image: /pics/joltphysics.jpg
---

#### Motivation

In the past I have experimented with sequential impulses to implement constraints (see
<a href="/simulation/2019/10/24/rigid-body-game-physics/">part 1</a>,
<a href="/simulation/2019/11/13/rigid-body-game-physics-2/">part 2</a>,
<a href="/simulation/2019/11/25/rigid-body-game-physics-3/">part 3</a>,
<a href="/simulation/2019/11/29/rigid-body-game-physics-4/">part 4</a>,
<a href="/simulation/2019/12/01/rigid-body-game-physics-5/">part 5</a>,
<a href="/simulation/2019/12/03/rigid-body-game-physics-6/">part 6</a> of my rigid body physics series).
I tried to integrate Runge-Kutta integration with sequential impulses.
However it was difficult to prevent interpenetration of objects.
Also implementing a vehicle with wheels and suspension, where the weight ratio between the vehicle and the wheels was high, required a high number of iterations to stabilise.
Finally stacking of boxes turned out to be unstable.

In a GDC 2014 talk, [Erin Catto showed sequential impulses and stable box stacking][4] in the Box2D engine.
Stacking of 2D boxes was made stable by solving for multiple impulses at the same time.

{% youtube "https://www.youtube.com/watch?v=SHinxAhv1ZE" %}

In 2022 Jorrit Rouwe released [JoltPhysics][1] which is a physics engine for 3D rigid objects also using sequential impulses.
His GDC 2022 talk [Architecting Jolt Physics for Horizon Forbidden West][10] refers to Erin Catto's talk and discusses various performance optimisations developed in Jolt Physics.

In the following I have provided a few **Jolt physics example programs** to demonstrate some capabilities of the physics engine.

#### Installing Jolt

Jolt Physics is a C++ library built using CMake.
To compile with double precision, I changed the cmake call in *JoltPhysics/Build/cmake_linux_clang_gcc.sh* as follows:
{% highlight shell %}
cmake -S . -B $BUILD_DIR -G "Unix Makefiles" -DDOUBLE_PRECISION=ON -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CXX_COMPILER=$COMPILER "${@}"
{% endhighlight %}

A release build with g++ and installation is done as follows:
{% highlight shell %}
./cmake_linux_clang_gcc.sh Release g++
cd Linux_Release
make
sudo make install
{% endhighlight %}

Next you can have a look at [JoltPhysics/HelloWorld/HelloWorld.cpp][2] which is a simple example of a sphere bouncing on a floor.
The example shows how to implement the required layers and collision filters (e.g. stationary objects cannot collide with each other).
Make sure to define the `Trace` variable so you get useful warnings if something goes wrong.

#### Tumbling object in space

In this section we test the tumbling motion of a cuboid in space.

To compile a C++ program using Jolt, you need to use the same preprocessor definitions which were used to compile Jolt.
If you have set up the `Trace` function, you will get a warning if the preprocessor definitions do not match.

Here is an example *Makefile* to compile and link a program with the release build of the Jolt library, GLFW, and GLEW.
{% highlight makefile %}
CCFLAGS = -g -fPIC -Wall -Werror -DNDEBUG -DJPH_PROFILE_ENABLED -DJPH_DEBUG_RENDERER -DJPH_OBJECT_STREAM -DJPH_DOUBLE_PRECISION $(shell pkg-config --cflags glfw3 glew)
LDFLAGS = -flto=auto $(shell pkg-config --libs glfw3 glew) -lJolt

all: tumble

tumble: tumble.o
	g++ -o $@ $^ $(LDFLAGS)

clean:
	rm -f tumble *.o

.cc.o:
	g++ -c $(CCFLAGS) -o $@ $<
{% endhighlight %}

See [Makefile][9] for complete build code.

The core of the example creates a shape of dimension a×b×c and sets the density to 1000.0.
Furthermore the convex radius used for approximating collision shapes needs to be much smaller than the object dimensions.
The limit for the linear velocity is lifted and most importantly the solution for gyroscopic forces is enabled.
Furthermore linear and angular damping are set to zero.
Finally the body is created, added to the physics system, and the angular velocity is set to an interesting value.
The code snippet is shown below:

{% highlight c++ %}
float a = 1.0;
float b = 0.1;
float c = 0.5;
// ...
BoxShapeSettings body_shape_settings(Vec3(a, b, c));
body_shape_settings.mConvexRadius = 0.01;
body_shape_settings.SetDensity(1000.0);
body_shape_settings.SetEmbedded();
ShapeSettings::ShapeResult body_shape_result = body_shape_settings.Create();
ShapeRefC body_shape = body_shape_result.Get();
BodyCreationSettings body_settings(body_shape, RVec3(0.0, 0.0, 0.0), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
body_settings.mMaxLinearVelocity = 10000.0;
body_settings.mApplyGyroscopicForce = true;
body_settings.mLinearDamping = 0.0;
body_settings.mAngularDamping = 0.0;
Body *body = body_interface.CreateBody(body_settings);
body_interface.AddBody(body->GetID(), EActivation::Activate);
body_interface.SetLinearVelocity(body->GetID(), Vec3(0.0, 0.0, 0.0));
body_interface.SetAngularVelocity(body->GetID(), Vec3(0.3, 0.0, 5.0));
{% endhighlight %}

Here is a video showing the result of the simulation.
As one can see, Jolt is able to simulate a tumbling motion without deterioation.

{% youtube "https://www.youtube.com/watch?v=kZoc2nsGFH4" %}

See [tumble.cc][3] for full source code.

#### Stack of cuboids

In this section we test the falling motion of a stack of cuboids.
Three cuboids are created and the initial positions are staggered in the x direction to get a more interesting result.
Using `i = 0, 1, 2` the cuboids are created in the following way:

{% highlight c++ %}
BoxShapeSettings body_shape_settings(Vec3(0.5 * a, 0.5 * b, 0.5 * c));
body_shape_settings.mConvexRadius = 0.01;
body_shape_settings.SetDensity(1000.0);
body_shape_settings.SetEmbedded();
ShapeSettings::ShapeResult body_shape_result = body_shape_settings.Create();
ShapeRefC body_shape = body_shape_result.Get();
BodyCreationSettings body_settings(body_shape, RVec3(i * 0.4, 0.2 + i * 0.2, -i * 0.3), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
body_settings.mMaxLinearVelocity = 10000.0;
body_settings.mApplyGyroscopicForce = true;
body_settings.mLinearDamping = 0.0;
body_settings.mAngularDamping = 0.0;
Body *body = body_interface.CreateBody(body_settings);
body->SetFriction(0.5);
body->SetRestitution(0.3f);
body_interface.AddBody(body->GetID(), EActivation::Activate);
{% endhighlight %}

Furthermore a ground shape is created.
Note that for simplicity I only created one layer.
If the ground was composed of multiple convex objects, a static layer should be created and used.

{% highlight c++ %}
BoxShapeSettings ground_shape_settings(Vec3(3.0, 0.1, 3.0));
ground_shape_settings.mConvexRadius = 0.01;
ground_shape_settings.SetEmbedded();
ShapeSettings::ShapeResult ground_shape_result = ground_shape_settings.Create();
ShapeRefC ground_shape = ground_shape_result.Get();
BodyCreationSettings ground_settings(ground_shape, RVec3(0.0, -0.5, 0.0), Quat::sIdentity(), EMotionType::Static, Layers::MOVING);
Body *ground = body_interface.CreateBody(ground_settings);
ground->SetFriction(0.5);
body_interface.AddBody(ground->GetID(), EActivation::DontActivate);
{% endhighlight %}

Note that the bodies need to be activated for the simulation to take place.

{% highlight c++ %}
body_interface.ActivateBody(body->GetID());
{% endhighlight %}

The simulation is run by repeatedly calling the `Update` method on the physics system.

{% highlight c++ %}
const int cCollisionSteps = 1;
physics_system.Update(dt, cCollisionSteps, &temp_allocator, &job_system);
{% endhighlight %}

The following video shows the result of the simulation.

{% youtube "https://www.youtube.com/watch?v=vo4-9reTK78" %}

See [stack.cc][5] for full source code.

For a more challenging demo of this type, see the [Stable Box Stacking][11] demo video by Jorrit Rouwe.

#### Double pendulum

The double pendulum is created using the `HingeConstraintSettings` class.
There are two hinges.
One between the base and the upper arm of the pendulum and one between the upper arm and the lower arm.
The physics library also requires initialisation of a vector normal to the hinge axis.

{% highlight c++ %}
HingeConstraintSettings hinge1;
hinge1.mPoint1 = hinge1.mPoint2 = RVec3(0.0, 0.5, 0);
hinge1.mHingeAxis1 = hinge1.mHingeAxis2 = Vec3::sAxisZ();
hinge1.mNormalAxis1 = hinge1.mNormalAxis2 = Vec3::sAxisY();
physics_system.AddConstraint(hinge1.Create(*base, *upper));

HingeConstraintSettings hinge2;
hinge2.mPoint1 = hinge2.mPoint2 = RVec3(a, 0.5, 0);
hinge2.mHingeAxis1 = hinge2.mHingeAxis2 = Vec3::sAxisZ();
hinge2.mNormalAxis1 = hinge2.mNormalAxis2 = Vec3::sAxisY();
physics_system.AddConstraint(hinge2.Create(*upper, *lower));
{% endhighlight %}

The following video shows the result.

{% youtube "https://www.youtube.com/watch?v=ITSNDQgw13U" %}

See [pendulum.cc][6] for full source code.

#### Suspension

Another test case is a prismatic joint with a suspension constraint.
The prismatic joint is created using the `SliderConstraintSettings` class.
The suspension is created using a soft distance constraint.
The code snippet is shown below:

{% highlight c++ %}
SliderConstraintSettings slider_settings;
slider_settings.mAutoDetectPoint = true;
slider_settings.SetSliderAxis(Vec3::sAxisY());
physics_system.AddConstraint(slider_settings.Create(*boxes[0], *boxes[1]));

DistanceConstraintSettings distance_settings;
distance_settings.mPoint1 = RVec3(0.0, 0.0, 0.0);
distance_settings.mPoint2 = RVec3(0.0, 0.4, 0.0);
distance_settings.mLimitsSpringSettings.mDamping = 0.1f;
distance_settings.mLimitsSpringSettings.mStiffness = 1.0f;
physics_system.AddConstraint(distance_settings.Create(*boxes[0], *boxes[1]));
{% endhighlight %}

The video shows the result of running this sumulation.

{% youtube "https://www.youtube.com/watch?v=f2Rcfzaxo9I" %}

See [suspension.cc][7] for full source code.

#### Wheeled vehicle

Jolt comes with a specialised implementation for simulating wheeled vehicles (there is also even one for tracked vehicles).
The vehicle API allows placing the wheels and adjusting the suspension minimum and maximum length.
One can set the angular damping of the wheels to zero.
Furthermore there are longitudinal and lateral friction curves of the wheels which I haven't modified.
Finally there is a vehicle controller object for setting motor, steering angle, brakes, and hand brake.

{% highlight c++ %}
RefConst<Shape> car_shape = new BoxShape(Vec3(half_vehicle_width, half_vehicle_height, half_vehicle_length));
BodyCreationSettings car_body_settings(car_shape, RVec3::sZero(), Quat::sIdentity(), EMotionType::Dynamic, Layers::MOVING);
car_body_settings.mOverrideMassProperties = EOverrideMassProperties::CalculateInertia;
car_body_settings.mMassPropertiesOverride.mMass = 1500.0f;
car_body_settings.mLinearDamping = 0.0;
car_body_settings.mAngularDamping = 0.0;

VehicleConstraintSettings vehicle;

WheelSettingsWV *w1 = new WheelSettingsWV;
w1->mPosition = Vec3(0.0f, -0.9f * half_vehicle_height, half_vehicle_length - 1.0f * wheel_radius);
w1->mSuspensionMinLength = wheel_radius;
w1->mSuspensionMaxLength = 2 * wheel_radius;
w1->mAngularDamping = 0.0f;
w1->mMaxSteerAngle = 0.0f; // max_steering_angle;
w1->mMaxHandBrakeTorque = 0.0f;
w1->mRadius = wheel_radius;
w1->mWidth = wheel_width;

WheelSettingsWV *w2 = new WheelSettingsWV;
w2->mPosition = Vec3(half_vehicle_width, -0.9f * half_vehicle_height, -half_vehicle_length + 1.0f * wheel_radius);
// ...

WheelSettingsWV *w3 = new WheelSettingsWV;
w3->mPosition = Vec3(-half_vehicle_width, -0.9f * half_vehicle_height, -half_vehicle_length + 1.0f * wheel_radius);
// ...

vehicle.mWheels = {w1, w2, w3};

WheeledVehicleControllerSettings *controller = new WheeledVehicleControllerSettings;
vehicle.mController = controller;

Body *car_body = body_interface.CreateBody(car_body_settings);
body_interface.AddBody(car_body->GetID(), EActivation::Activate);
VehicleConstraint *constraint = new VehicleConstraint(*car_body, vehicle);
VehicleCollisionTester *tester = new VehicleCollisionTesterRay(Layers::MOVING);
constraint->SetVehicleCollisionTester(tester);
physics_system.AddConstraint(constraint);
physics_system.AddStepListener(constraint);

vehicle_controller->SetDriverInput(0.0f, 0.0f, 0.0f, 0.0f);
{% endhighlight %}

A vehicle dropping on the ground with horizontal speed is shown in the following video.

{% youtube "https://www.youtube.com/watch?v=LWSXWqWFKmQ" %}

Note that the inertia of the wheels was high in this video.
One can correct this by reducing the inertia of the wheels as follows.

{% highlight c++ %}
w1->mInertia = 0.01;
w2->mInertia = 0.01;
w3->mInertia = 0.01;
{% endhighlight %}

See [vehicle.cc][8] for full source code.

Enjoy!

[1]: https://jrouwe.github.io/JoltPhysics/
[2]: https://github.com/jrouwe/JoltPhysics/blob/master/HelloWorld/HelloWorld.cpp
[3]: https://github.com/wedesoft/jolttest/blob/main/tumble.cc
[4]: https://www.youtube.com/watch?v=SHinxAhv1ZE
[5]: https://github.com/wedesoft/jolttest/blob/main/stack.cc
[6]: https://github.com/wedesoft/jolttest/blob/main/pendulum.cc
[7]: https://github.com/wedesoft/jolttest/blob/main/suspension.cc
[8]: https://github.com/wedesoft/jolttest/blob/main/vehicle.cc
[9]: https://github.com/wedesoft/jolttest/blob/main/Makefile
[10]: https://gdcvault.com/play/1027891/Architecting-Jolt-Physics-for-Horizon
[11]: https://www.youtube.com/watch?v=fTtjBLYBxco
