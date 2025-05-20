---
layout: post
title: Create Blender bones and animate and import with Assimp
category: graphics
image: /pics/animate.jpg
---

This article is about
* creating objects and armatures consisting of bones
* adding inverse kinematics
* creating animations
* exporting to glTF
* importing the animations with Assimp

Here is the Youtube video showing the steps involved:

{% youtube "https://www.youtube.com/watch?v=J83S_EFRB6A" %}

Here is a corresponding step-by-step procedure to create a proof-of-concept kinematic chain in Blender

* create armature
  * first clear scene (in layout mode)
  * in Object mode add an armature which is going to be named "Armature"
  * rotate the armature around the y-axis by 90 degrees so that the bone points in the direction of the x-axis
  * in Edit mode move the tip of the bone to enlarge it
  * press E and extrude the bone to add a second bone to the chain
  * in Edit mode add an additional bone to the armature by pressing Shift-A and move it to the end of the previous chain
  * in the tree view rename the bones to "upper bone", "lower bone", and "stay IK"
* set up inverse kinematic constraint
  * in Pose mode select the stay IK bone first (!) and then shift-click on the lower bone.
  * go to bone constraints (not constraints) and add an Inverse Kinematics constraint
  * set target to "Armature" and bone to "stay IK"
  * set the chain length to 2
  * note that the bone constraint should appear under the lower bone in the tree view
  * moving the stay IK bone in Pose mode should now update the kinematic chain of upper and lower bone
* add objects and connect them to armature
  * in Object mode select the armature, go to Object properties -> Viewport display, and check name and in front
  * in Object mode create a cuboid and move it to the position of the upper bone
  * click on the cube and shift-click on the armature
  * switch to Pose mode, click on the cube, and shift-click on the upper bone
  * press Ctrl-P and set the parent to bone
  * moving the stay IK bone should now move the cuboid
  * add another cube and set the parent to the lower bone (similar to the last 5 steps but for the lower bone)
  * add a small sphere and move it to the stay IK bone position
  * select the armature in Object mode
  * switch to Pose mode and select the stay IK bone
  * add the "Child Of" bone constraint and set the target to the sphere
  * moving the sphere in Object mode should move the cuboids now
* add top-level armatures
  * add another armature in Object mode and set the name to "Control" in the tree view
  * click the upper arm bone and shift click the control armature, use Ctrl-P and set it as parent object
  * set the parent object of the sphere to the control armature as well (object, keep transform)
  * moving the control bone now should move everything else
  * create a root armature
  * shift drag and drop control armature under root armature (an extra node in the tree is useful if you want to overwrite the top-level transform programmatically)
* create animations and export to glTF
  * go to the animation editor
  * add two keyframes for 90 degree rotation of the control bone around the y-axis
  * switch from Dope sheet to Nonlinear animation editor and push down the action
  * select the control bone in the tree view
  * in the 3D viewport, go to the Object menu, then Animation, and select Bake Action.
  * bake only the selected bones and Object data
  * delete the original action, rename the current action to "ControlAction", and push it down
  * uncheck the control action
  * add two keyframes moving the sphere
  * select the armature and the sphere and bake another action
  * push down the generated actions and the sphere action and rename to the same name (e.g. "CubeAction") so that they will later become one animation in glTF
  * check the control action back on so that all actions are checked now
  * export the glTF with the animations with "Animation Mode" set to "NLA Tracks"

You can use the LWJGL Assimp Java library to import the animations into Clojure as follows:

{% highlight clojure %}
(import '[org.lwjgl.assimp Assimp AIAnimation AINodeAnim])
(def scene (Assimp/aiImportFile "test.gltf" (bit-or Assimp/aiProcess_Triangulate Assimp/aiProcess_CalcTangentSpace)))
(.mNumAnimations scene)
; 2
(def animation (AIAnimation/create ^long (.get (.mAnimations scene) 1)))
(/ (.mDuration animation) (.mTicksPerSecond animation))
; 10.4166669921875
(.mNumChannels animation)
; 5
(map #(.dataString (.mNodeName (AINodeAnim/create ^long (.get (.mChannels animation) %)))) (range (.mNumChannels animation)))
; ("Sphere" "Armature" "Stay IK" "Lowe Bone" "Upper Bone")
(def node-anim (AINodeAnim/create ^long (.get (.mChannels animation) 4)))
(.dataString (.mNodeName node-anim))
; "Upper Bone"
(.mNumPositionKeys node-anim)
; 2
(.mNumRotationKeys node-anim)
; 250
(.mNumScalingKeys node-anim)
; 2
(def quaternion (.mValue (.get (.mRotationKeys node-anim) 249)))
[(.w quaternion) (.x quaternion) (.y quaternion) (.z quaternion)]
; [0.99631643 -1.20924955E-17 -1.5678631E-17 0.08575298]
(/ (.mTime (.get (.mRotationKeys node-anim) 249)) (.mTicksPerSecond animation))
; 10.4166669921875
{% endhighlight %}

I used tmux, Clojure with rebel-readline, and vim with vim-slime to do the coding.

For an in-depth introduction to rigging I can recommend [Mark Alloway's video on how to rig and animate a landing gear][1].

Enjoy!

[1]: https://www.youtube.com/watch?v=tV2MhgnnvOg
