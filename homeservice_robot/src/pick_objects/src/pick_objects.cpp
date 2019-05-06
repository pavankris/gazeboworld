#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <visualization_msgs/Marker.h>
#include <actionlib_msgs/GoalStatus.h>
#include <vector>
#include <string.h>

// Define a client for to send goal requests to the move_base server through a SimpleActionClient
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class PickObjects
{
private:
  MoveBaseClient *ac;
  ros::Publisher *goalstatus_pub;
  std::vector<visualization_msgs::Marker> markersCollected;
  int numOfObjects;
  std::vector<double> dropOff;
public:
  PickObjects(
    MoveBaseClient *ac,
    ros::Publisher *goalstatus_pub,
    int numOfObjects,
    std::vector<double> dropOff):
  ac(ac), goalstatus_pub(goalstatus_pub), numOfObjects(numOfObjects), dropOff(dropOff) {}

  void moveToTarget(double x, double y) {
    move_base_msgs::MoveBaseGoal goal;

    // set up the frame parameters
    goal.target_pose.header.frame_id = "map";
    goal.target_pose.header.stamp = ros::Time::now();

    // Define a position and orientation for the robot to reach
    goal.target_pose.pose.position.x = x;
    goal.target_pose.pose.position.y = y;
    goal.target_pose.pose.orientation.w = 1.0;

     // Send the goal position and orientation for the robot to reach
    ROS_INFO("Sending goal");
    ac->sendGoal(goal);

  }

  bool sendGoalToRobot(double x, double y, int id) {
    moveToTarget(x, y);
    ac->waitForResult();

    if(ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED) {
      ROS_INFO("Hooray, the robot picked up object, remaining: %d",
        (int) (numOfObjects - markersCollected.size()));
      actionlib_msgs::GoalStatus goal_status;
      goal_status.goal_id.id = std::to_string(id);
      goal_status.status = actionlib_msgs::GoalStatus::SUCCEEDED;
      goalstatus_pub->publish(goal_status);
      return true;
    }
    else {
      ROS_INFO("The robot did not pick up the object");
      actionlib_msgs::GoalStatus goal_status;
      goal_status.goal_id.id = std::to_string(id);
      goal_status.status = actionlib_msgs::GoalStatus::LOST;
      goalstatus_pub->publish(goal_status);
      return false;
    }
  }

  void processMarker(const visualization_msgs::Marker marker) {
    if (marker.action == visualization_msgs::Marker::DELETE) {
      ROS_INFO("Marker picked up");
      return;
    }
    if (markersCollected.size() == numOfObjects) {
      ROS_INFO("Dropping markers");
    }
    ROS_INFO("Marker added %f %f", marker.pose.position.x, marker.pose.position.y);
    if(sendGoalToRobot(marker.pose.position.x, marker.pose.position.y, marker.id)) {
      markersCollected.push_back(marker);
    }
    if (markersCollected.size() == numOfObjects) {
        ROS_INFO("Going to Dropoff %f %f", dropOff[0], dropOff[1]);
        sendGoalToRobot(dropOff[0], dropOff[1], numOfObjects);
    }
  }
  ~PickObjects(){}
  
};
int main(int argc, char** argv){
  ros::init(argc, argv, "pick_objects");

  ros::NodeHandle n;
  MoveBaseClient ac("move_base", true);


  while(!ac.waitForServer(ros::Duration(5.0))){
    ROS_INFO("Waiting for the move_base action server to come up");
  }
  int numOfObjects;
  std::vector<double> dropOff;
  ros::param::get("numOfObjects", numOfObjects);
  ros::param::get("dropoff", dropOff);
  ros::Publisher goalstatus_pub = n.advertise<actionlib_msgs::GoalStatus>("goal_status", 1);
  PickObjects po(&ac, &goalstatus_pub, numOfObjects, dropOff);
  ros::Subscriber sub1 = n.subscribe("visualization_marker", 10, 
    &PickObjects::processMarker, &po);

  /*po.moveToTarget(x, y);
  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the base moved 2 meter backward");
  else
    ROS_INFO("The base failed to move 2 meter backward for some reason");

  // Sleep for1 second
  ros::Duration(1).sleep();
  x = -1.03;
  y = 3.1;

  po.moveToTarget(x, y);

  // Wait an infinite time for the results
  ac.waitForResult();

  // Check if the robot reached its goal
  if(ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
    ROS_INFO("Hooray, the base moved 1 meter forward again");
  else
    ROS_INFO("The base failed to move forward 1 meter for some reason");

  // Sleep for1 second
  ros::Duration(5).sleep();*/

  ros::spin();

  return 0;
}