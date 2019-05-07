#!/bin/sh

rosdep -i install turtlebot_gazebo

catkin_make

xterm  -e  " source devel/setup.bash;roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(pwd)/src/myrobot/worlds/pavan.world " &
sleep 5

sudo apt-get install ros-kinetic-amcl
xterm -e " source devel/setup.bash;roslaunch turtlebot_gazebo amcl_demo.launch map_file:=$(pwd)/src/myrobot/maps/pavanworldmap.yaml initial_pose_a:=4.71 " &
sleep 5

xterm -e " source devel/setup.bash;roslaunch myrobot rviz.launch " &
sleep 5