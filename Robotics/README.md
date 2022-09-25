# Robotics

[Quaternion](https://en.wikipedia.org/wiki/Quaternion) is used in [calculations involving three-dimensional rotations](https://en.wikipedia.org/wiki/Quaternions_and_spatial_rotation):

> In 3-dimensional space, according to [Euler's rotation theorem](https://en.wikipedia.org/wiki/Euler%27s_rotation_theorem), any rotation or sequence of rotations of a rigid body or coordinate system about a fixed point is equivalent to a single rotation by a given angle θ about a fixed axis (called the Euler axis) that runs through the fixed point. The Euler axis is typically represented by a unit vector `u→` ( `e^` in the picture). Therefore, any rotation in three dimensions can be represented as a combination of a vector `u→` and a scalar θ.

<img alt="3D visualization of a sphere and a rotation about an Euler axis (e^) by an angle of θ" src="https://upload.wikimedia.org/wikipedia/commons/5/51/Euler_AxisAngle.png" width=30% height=30% />

---

[Right-hand rule](https://en.wikipedia.org/wiki/Right-hand_rule):
- <img alt="Cartesian coordinate system handedness" src="https://upload.wikimedia.org/wikipedia/commons/e/e2/Cartesian_coordinate_system_handedness.svg" />
- Note: Left-handed coordinates on the left; right-handed coordinates on the right.
- For **left-handed coordinates** the left thumb points along the z axis in the positive direction and the **curled** fingers of the left hand represent a motion from the first or x axis to the second or y axis. When viewed from the top or z axis the system is clockwise.
- For **right-handed coordinates** the right thumb points along the z axis in the positive direction and the **curl** of the fingers represents a motion from the first or x axis to the second or y axis. When viewed from the top or z axis the system is counter-clockwise.
- Also note that the "left-handedness" or "right-handedness" is about the **curl** of the fingers.
- Another way to look at the handedness of the coordinate systems: **Always use the index finger to point to the positive x-axis, middle finger to the positive y-axis, and the thumb to point to the positive z-axis.** You will find that you have to use the left hand to do it for left-handed coordinate systems and the right hand for the right-handed coordinate systems.
- There is no way to rotate these two coordinate systems so that they align. They represent two **incompatible** ways of representing three-dimensional coordinates. This means that whenever we provide coordinates in three dimensions we must specify whether we are using a left-handed or right-handed system.

---

[Odometry](https://en.wikipedia.org/wiki/Odometry):

> **Odometry** is the **use of data from motion sensors to estimate change in position over time**. It is used in robotics by some legged or wheeled robots to estimate their position relative to a starting location. This method is sensitive to errors due to the integration of velocity measurements over time to give position estimates. Rapid and accurate data collection, instrument calibration, and processing are required in most cases for odometry to be used effectively. 

Examples of odometry:
- Suppose a robot has rotary encoders on its wheels or on its legged joints. It drives forward for some time and then would like to know how far it has traveled. It can measure how far the wheels have rotated, and if it knows the circumference of its wheels, compute the distance.
- Train operations are also frequent users of odometrics. Typically, a train gets an absolute position by passing over stationary sensors in the tracks, while odometry is used to calculate relative position while the train is between the sensors. 

---

[Roll, Pitch, and Yaw](https://howthingsfly.si.edu/flight-dynamics/roll-pitch-and-yaw):
- Rotation around the front-to-back axis is called `roll`.
- Rotation around the side-to-side axis is called `pitch`.
- Rotation around the vertical axis is called `yaw`.

---

The term "**coordinate frame**" looks quite similar to "coordinate system" but in fact they are different. A coordinate frame is more like a [frame of reference](https://en.wikipedia.org/wiki/Frame_of_reference) which is "an abstract coordinate system with an _origin_, _orientation_, and _scale_ specified by a set of reference points - geometric points whose position is identified both mathematically (with numerical coordinate values) and physically (signaled by conventional markers)." Or, as simply put by the post [_Reference frames versus coordinate systems_](https://physics.stackexchange.com/q/497994/226928), it describes the **"state of motion of the observer"**.

A **coordinate system** defines how the points in a space are parameterized. For example, a 3D [Cartesian coordinate system](https://en.wikipedia.org/wiki/Cartesian_coordinate_system) uses _`(x, y, z)`_ to find a point in the 3D space, while the [spherical coordinate system](https://en.wikipedia.org/wiki/Spherical_coordinate_system) uses _`(r, θ, φ)`_ to find a point. For more information, see [`Mathematics/Coordinate-Systems.pdf`](../Mathematics/Coordinate-Systems.pdf).

Therefore, within the same coordinate frame, the same point can be described using different coordinate systems which result in different representations. However, the conversion between the two coordinate systems for the same point is usually a pure mathematical calculation, without the need of considering the "location" of the reference frame.

However, even if using the same coordinate system, if one point is described relative to different coordinate frames, the conversion will have to consider the locations of the coordinate frames themselves. Usually, the locations of the coordinate frames are described relative to a common coordinate frame.

Also note that a coordinate frame is not necessarily "stationary". Consider the case of a moving camera inside a room. The camera can be used as a reference frame, and the corner of the same can be another reference frame. Relative to the room reference frame, the camera reference frame is always moving. Suppose when the camera is at the location 1, it captures an object at the coordinates `(x0, y0, z0)`; when the camera moves to the location 2, it captures a different object at `(x0, y0, z0)`. To the camera, these two points at `(x0, y0, z0)` are the "same" point because their coordinates are the same. However, when we observe from the room reference frame, they are two different points: Here we must also consider the locations of the camera.

---

The document [frames.pdf](./frames.pdf) discusses the coordinate frames and their conversion. This document is downloaded from [this link](https://w3.cs.jmu.edu/spragunr/CS354_F16/handouts/frames.pdf).

---

ROS:
- [REP 103: Standard Units of Measure and Coordinate Conventions](https://www.ros.org/reps/rep-0103.html)
- [REP 105: Coordinate Frames for Mobile Platforms](https://ros.org/reps/rep-0105.html): This article mentions:

> When defining coordinate frames with respect to a global reference like the earth:
> - The default should be to align the x-axis east, y-axis north, and the z-axis up at the origin of the coordinate frame.
> - If there is no other reference the default position of the z-axis should be zero at the height of the WGS84 ellipsoid.

Note that the description of "x-axis east, y-axis north, and the z-axis up at the origin" is a right-handed coordinate system.
