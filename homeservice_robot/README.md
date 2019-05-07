![out](https://user-images.githubusercontent.com/1688726/57280362-efdcb880-705d-11e9-9e8c-4750eecf77f4.gif)

## Installation Instructions

1. mkdir src; cd src
2. catkin_init_workspace .
3. git clone https://github.com/turtlebot/turtlebot_simulator
4. git clone https://github.com/turtlebot/turtlebot_interactions
5. git clone https://github.com/turtlebot/turtlebot_interactions
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
