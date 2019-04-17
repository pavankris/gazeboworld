## Instructions to install project3, which will do monte carlo localization of robot.
```
git clone https://github.com/pavankris/gazeboworld.git
sudo apt-get update && sudo apt-get upgrade -y
sudo apt-get install libignition-math2-dev protobuf-compiler
cd project4_mapmyworld
catkin_make
source devel/setup.bash
roslaunch main main.launch &
rosrun teleop_twist_keyboard teleop_twist_keyboard.py

```

![out5](https://user-images.githubusercontent.com/1688726/56288593-f10c6c80-60d3-11e9-9550-87ab1f40eab6.png)
