#!/bin/sh

rosdep -i install turtlebot_gazebo
rosdep -i install turtlebot_teleop
rosdep install gmapping
catkin_make

xterm  -e  " source devel/setup.bash;roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=$(pwd)/src/myrobot/worlds/pavan.world " &
sleep 5

xterm  -e  " source devel/setup.bash;rosrun gmapping slam_gmapping " &
sleep 5

xterm -e " source devel/setup.bash;roslaunch myrobot rviz.launch " &
sleep 5

xterm -e " source devel/setup.bash;roslaunch turtlebot_teleop keyboard_teleop.launch " &
