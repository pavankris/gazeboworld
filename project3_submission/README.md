## Instructions to install project3, which will do monte carlo localization of robot.
```
git clone https://github.com/pavankris/gazeboworld.git
sudo apt-get update && sudo apt-get upgrade -y
sudo apt-get install libignition-math2-dev protobuf-compiler
cd project3_submission
catkin_make
source devel/setup.bash
roslaunch main main.launch &
rosrun teleop_twist_keyboard teleop_twist_keyboard.py

```

