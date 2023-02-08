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


The ROS parameter server is designed to persist between node executions such that you can manually set them, then launch the node, and have the new instance use those settings.

I'm not aware of any use case where we'd want to adjust the roi dynamically. If some day we had to go with slightly mismatched lenses and imagers that resulted in the imager having unexposed surface area (e..g. the corners of images from SL units) then we may want to set roi to some values to crop it out ... but that would be a calibration step, not something we'd want to dynamically adjust.
