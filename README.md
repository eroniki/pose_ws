# pose_ws
Pose Estimation with AR Tags using a Pan-Tilt Network Camera

# Installation
## Dependencies
libv4l:

```sudo apt-get install libv4l-dev```

ffmpeg (possibly):

```sudo apt-get install ffmpeg```

wstool:

```sudo apt-get install python-wstool```

## ROS Workspace and the packages
```
cd ~
git clone https://github.com/eroniki/pose_ws
cd pose_ws/src
roslocate info ar_tools | wstool merge -
wstool update
rosdep install -i --from-paths ar_tools
cd ..
catkin_make
```
