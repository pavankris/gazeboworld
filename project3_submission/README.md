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

![out5](https://user-images.githubusercontent.com/1688726/54287184-67accc00-4562-11e9-8b13-e1ef51fa0c5c.gif)
