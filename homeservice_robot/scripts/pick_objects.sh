#!/bin/sh

rosdep -i install turtlebot_gazebo

catkin_make

xterm  -e  " source devel/setup.bash;roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=/home/workspace/myrobot/homeservice_robot/src/myrobot/worlds/pavan.world " &
sleep 5

sudo apt-get install ros-kinetic-amcl
xterm -e " source devel/setup.bash;roslaunch turtlebot_gazebo amcl_demo.launch map_file:=/home/workspace/myrobot/homeservice_robot/src/myrobot/maps/pavanworldmap.yaml " &
sleep 5

xterm -e " source devel/setup.bash;roslaunch turtlebot_rviz_launchers view_navigation.launch " &
sleep 5

xterm -e " source devel/setup.bash;roslaunch pick_objects pick_objects.launch " &
sleep 5