## Instructions to run
- mkdir catkin_ws/src
- cd catkin_ws/src && catkin_init_workspace
- git clone https://github.com/pavankris/gazeboworld .
- cp -r project2_submission/* .
- cd ../
- catkin_make
- source devel/setup.bash
- roslaunch myrobot world.launch
- In other terminal, source devel/setup.bash
- roslaunch ball_chaser ball_chaser.launch

## Robot Chasing the ball
![out](https://user-images.githubusercontent.com/1688726/53022547-43047f00-3410-11e9-9025-0de4f0259f48.gif)
