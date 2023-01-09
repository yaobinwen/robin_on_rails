# ROS

## Overview

This folder has the notes and code that shows how to use ROS.

## `ywen_flir`

This sample code shows how to use nodelets.

What are needed for a nodelet:
- `nodelets.xml`
- The `<export>` section in `package.xml`.
- `PLUGINLIB_EXPORT_CLASS` in `nodelet.cpp`.
- A `.launch` file that launches nodelets.

How to run the nodelet:
- Source `./devel/setup.bash`.
- `rosrun ywen_flir ywen_flir_node`.

References:
- [Clearpath Robotics: Nodelet Everything](https://www.clearpathrobotics.com/assets/guides/melodic/ros/Nodelet%20Everything.html)
