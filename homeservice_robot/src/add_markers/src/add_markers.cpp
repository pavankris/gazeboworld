
#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>
#include <actionlib_msgs/GoalStatus.h>
#include <vector>
#include <math.h>
#include <stdlib.h>

class AddMarkers
{
  ros::Publisher *marker_pub;
  std::vector<std::vector<double> > pickups;
  std::vector<double> dropOff;
  std::vector<visualization_msgs::Marker> publishedMarkers;
public:
  AddMarkers(
    ros::Publisher *marker_pub,
    std::vector<std::vector<double> > pickups,
    std::vector<double> dropOff):
  marker_pub(marker_pub), pickups(pickups), dropOff(dropOff) {}

  void publish_marker() {
    if (publishedMarkers.size() == pickups.size()) {
      return;
    }
    std::vector<double> nextPickup = pickups[publishedMarkers.size()];
    uint32_t shape = visualization_msgs::Marker::CUBE;

    ROS_INFO("Publishing markers %d", publishedMarkers.size());
    visualization_msgs::Marker marker;
    marker.header.frame_id = "map";
    marker.header.stamp = ros::Time::now();
    marker.ns = "add_markers";
    marker.id = publishedMarkers.size();
    marker.type = shape;
    marker.action = visualization_msgs::Marker::ADD;
    
    marker.pose.position.x = nextPickup[0];
    marker.pose.position.y = nextPickup[1];
    marker.pose.position.z = 0;
    marker.pose.orientation.x = 0.0;
    marker.pose.orientation.y = 0.0;
    marker.pose.orientation.z = 0.0;
    marker.pose.orientation.w = 1.0;
    
    marker.scale.x = 0.5;
    marker.scale.y = 0.5;
    marker.scale.z = 0.5;

    marker.color.r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    marker.color.g = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    marker.color.b = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
    marker.color.a = 1.0;

    marker.lifetime = ros::Duration(30);

    publishedMarkers.push_back(marker);

    marker_pub->publish(marker);

  }

  double distanceCalculate(double x1, double y1, double x2, double y2)
  {
    double x = x1 - x2; //calculating number to square in next step
    double y = y1 - y2;
    double dist;

    dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    dist = sqrt(dist);                  

    return dist;
  }

  double calculateMHDistance(double x1, double y1, double x2, double y2) {
    return fabs(x1 - x2) + fabs(y1 - y2);
  }

  void amclPoseReceived(const geometry_msgs::PoseWithCovarianceStamped msg) {
    if (publishedMarkers.size() == 0)
    {
      return;
    }
    std::vector<double> lastPickup = pickups[publishedMarkers.size()-1];

    double disance = distanceCalculate(
      msg.pose.pose.position.x,
      msg.pose.pose.position.y,
      lastPickup[0],
      lastPickup[1]
      );
    double manhattanDistance = calculateMHDistance(
      msg.pose.pose.position.x,
      msg.pose.pose.position.y,
      lastPickup[0],
      lastPickup[1]
      );
    ROS_INFO("Position-> x: [%f], y: [%f], d: [%f], md: [%f]",
      msg.pose.pose.position.x,msg.pose.pose.position.y, disance, manhattanDistance);
    if (disance < 0.12) {
      // Destination reached, remove the marker.
      visualization_msgs::Marker marker = publishedMarkers.back();
      marker.action = visualization_msgs::Marker::DELETE;
      marker_pub->publish(marker);
      ros::Duration(2).sleep();
      publish_marker();
    }
    // Check if robot current position, measure euclidean distance

  }

  void goalStatusReceived(const actionlib_msgs::GoalStatus goalStatus) {
    if (goalStatus.status == actionlib_msgs::GoalStatus::SUCCEEDED) {
      if (goalStatus.goal_id.id == std::to_string(publishedMarkers.size())) {
        // In Dropoff zone
        ROS_INFO("Droping off markers");
        for (int i=0;i<publishedMarkers.size();i++) {
          ROS_INFO("Droping marker : %d", i);
          visualization_msgs::Marker marker = publishedMarkers[i];
          marker.action = visualization_msgs::Marker::ADD;
          marker.pose.position.x = dropOff[0] + i * 0.1;
          marker.pose.position.y = dropOff[1] + i * 0.1;
          marker.lifetime = ros::Duration(5);
          marker_pub->publish(marker);
        }
      } else {
        ROS_INFO("Removing markers");
        visualization_msgs::Marker marker = publishedMarkers.back();
        marker.action = visualization_msgs::Marker::DELETE;
        marker_pub->publish(marker);
        ros::Duration(2).sleep();
        publish_marker();
      }
    } else if (goalStatus.status == actionlib_msgs::GoalStatus::LOST) {
      if (goalStatus.goal_id.id == std::to_string(publishedMarkers.size())) {
        ROS_INFO("Could not reach goal");
      } else {
        ROS_INFO("Re adding marker");
        marker_pub->publish(publishedMarkers.back());
      }
    }
  }
  ~AddMarkers() {}
  
};

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");

  std::vector<double> dropOff;
  std::vector<double> pickup;
  ros::param::get("dropoff", dropOff);
  ros::param::get("pickups", pickup);
  std::vector<std::vector<double> > pickups;
  for (int i = 0; i < pickup.size();)
  {
    std::vector<double> pickupTemp;
    pickupTemp.push_back(pickup[i++]);
    pickupTemp.push_back(pickup[i++]);
    pickups.push_back(pickupTemp);
  }
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);
  AddMarkers addMarkers(&marker_pub, pickups, dropOff);
  //ros::Subscriber sub1 = n.subscribe("odom", 10, 
  //  &AddMarkers::robotMoved, &addMarkers);
  // ros::Subscriber sub1 = n.subscribe("amcl_pose", 10, 
  //   &AddMarkers::amclPoseReceived, &addMarkers);
  ros::Subscriber sub1 = n.subscribe("goal_status", 10, 
    &AddMarkers::goalStatusReceived, &addMarkers);
  while (marker_pub.getNumSubscribers() < 1) {
      if (!ros::ok())
      {
        return 0;
      }
      ROS_WARN_ONCE("Please create a subscriber to the marker");
      sleep(1);
  }
  addMarkers.publish_marker();

  ros::spin();
  return 0;

}
