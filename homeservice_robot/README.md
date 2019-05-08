# Home Service Robot
![out](https://user-images.githubusercontent.com/1688726/57280362-efdcb880-705d-11e9-9e8c-4750eecf77f4.gif)
![out1](https://user-images.githubusercontent.com/1688726/57378166-f69c2600-7158-11e9-9307-0be57c1015eb.gif)

### Packages Used

 > [turtlebot_gazebo](https://github.com/turtlebot/turtlebot_simulator) 
- Launches the turtlebot inside the specified world file. Turtlebot is equipped with camera and also provides a node to convert camera into laser output. 
- Provides amcl(Monte carlo localization) for localizing the robot which accepts the map file
- It also launches move_base which will launch path planner 

 > [rviz](https://github.com/ros-visualization/rviz)
 - Robot visualization, that can visualize the robot state. It is launched with the predefined config from [view_navigation](https://github.com/turtlebot/turtlebot_interactions)

> rqt_graph
![out](https://github.com/pavankris/gazeboworld/blob/master/homeservice_robot/rosgraph.png)

### Installation Instructions

1. mkdir src; cd src
2. catkin_init_workspace .
3. git clone https://github.com/turtlebot/turtlebot_simulator
4. git clone https://github.com/turtlebot/turtlebot_interactions
6. cd ..;catkin_create_pkg pick_objects move_base_msgs actionlib roscpp
7. cd ..;catkin_create_pkg add_markers visualization_msgs roscpp
8. scripts/home_service.sh

### To generate map

1. sudo apt-get install libignition-math2-dev protobuf-compiler
2. cd src
3. git clone https://github.com/udacity/pgm_map_creator.git
4. cp pavan.world src/pgm_map_creator/worlds/
5. add following to the world file 
  `<plugin filename="libcollision_map_creator.so" name="collision_map_creator"/>`
6. gzserver src/pgm_map_creator/world/pavan.world
7. roslaunch pgm_map_creator request_publisher.launch
