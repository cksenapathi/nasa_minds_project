# NASA MINDS Project
## Collaboration between UT Austin and ACC

## Competition Description
As a part of their Artemis program, NASA issued a competition for students to do research and build prototypes that would progress some technology to aid the astronauts as they return to the Moon.

Our team chose to focus on the field of autonomous robotics and how they can augment the capabilities of the astronauts as they explore their landing site on the Moon.

This repo contains the software and the models for the autonomous hexapod that was designed by the team, and will grow towards our goal of creating a decentralized swarm of autonomous robots which are able to collaboratively map out a given area.

## TODO
- Add body stl to model once inertia is calculated
- Simulate moon surface using https://github.com/MobileRobots/amr-ros-config
- Add camera plugin to model to simulate cameras
- Write software which stitches pairs of cameras to get depth information
- Stitching stereo images for panoramic depth information
- Adding IMU plugin
- Integrating data sources into http://wiki.ros.org/rtabmap_ros
- Write software to stitch multiple point cloud pieces together; 2 options
  - Pull nodelet from rtabmap
  - Implement ICP algorithm to build a point cloud map in global inertial frame
- Path planning algorithm which takes current map with graph of all previously visited locations and outputs a new location in inertial frame to maximize information gain
