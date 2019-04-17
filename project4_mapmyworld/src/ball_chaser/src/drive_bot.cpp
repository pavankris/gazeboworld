#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <ball_chaser/DriveToTarget.h>

// TODO: Create a handle_drive_request callback function that executes whenever a drive_bot service is requested
// This function should publish the requested linear x and angular velocities to the robot wheel joints
// After publishing the requested velocities, a message feedback should be returned with the requested wheel velocities
class BotDriver
{
private:
    ros::Publisher motor_command_publisher;
public:
    BotDriver(ros::Publisher motor_command_publisher):
    motor_command_publisher(motor_command_publisher) {}

    bool handle_drive_request(ball_chaser::DriveToTarget::Request& req,
        ball_chaser::DriveToTarget::Response& res)
    {
        //ROS_INFO("HandleDriveRequest receieved - li:%1.2f, an:%1.2f", (float)req.linear_x, (float)req.angular_z);

        geometry_msgs::Twist motor_command;
        motor_command.linear.x = req.linear_x;
        motor_command.angular.z = req.angular_z;
        // Publish angles to drive the robot
        motor_command_publisher.publish(motor_command);

        // Wait 3 seconds for arm to settle
        ros::Duration(1).sleep();

        res.msg_feedback = "Wheel velocities set linear: " + std::to_string(motor_command.linear.x) + ", angular: " + std::to_string(motor_command.angular.z);
        ROS_INFO_STREAM(res.msg_feedback);
        return true;

    
    }


    ~BotDriver(){}
    
};
int main(int argc, char** argv)
{
    // Initialize a ROS node
    ros::init(argc, argv, "drive_bot");

    // Create a ROS NodeHandle object
    ros::NodeHandle n;

    // Inform ROS master that we will be publishing a message of type geometry_msgs::Twist on the robot actuation topic with a publishing queue size of 10
    ros::Publisher motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    BotDriver *botDriver = new BotDriver(motor_command_publisher);
    ros::ServiceServer service = n.advertiseService("/ball_chaser/drive", &BotDriver::handle_drive_request, botDriver);
    ROS_INFO("Ready to send drive commands");


    ros::spin();

    return 0;
}