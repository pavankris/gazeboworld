git clone https://github.com/turtlebot/turtlebot_simulator
git clone https://github.com/turtlebot/turtlebot

rosdep -i install turtlebot_gazebo
rosdep -i install turtlebot_teleop

git clone https://github.com/ros-perception/slam_gmapping
rosdep install gmapping

catkin_make

roslaunch turtlebot_gazebo turtlebot_world.launch world_file:=worlds/willowgarage.world


roslaunch turtlebot_teleop keyboard_teleop.launch

rosrun gmapping slam_gmapping

rosrun rviz rviz

# To save the map
rosrun map_server map_saver -f myMap

# Grid based Slam
![GridSlam](https://user-images.githubusercontent.com/1688726/55355539-9d284380-5496-11e9-9d80-4e8b257e6563.png)
