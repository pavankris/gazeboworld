#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <ball_chaser/DriveToTarget.h>
#include <sensor_msgs/Image.h>

class ImageProcessor
{
private:
    ros::ServiceClient client;
public:
    ImageProcessor(ros::ServiceClient client):
    client(client) {}
    ~ImageProcessor(){}
    // This function calls the command_robot service to drive the robot in the specified direction
    void drive_robot(float lin_x, float ang_z)
    {

        ball_chaser::DriveToTarget srv;
        srv.request.linear_x = lin_x;
        srv.request.angular_z = ang_z;

        if (!client.call(srv))
            ROS_ERROR("Failed to call service drive_robot");

    }
    // This callback function continuously executes and reads the image data
    void process_image_callback(const sensor_msgs::Image img)
    {

        int white_pixel = 255;
        bool white_ball_found = false;

        // Loop through each pixel in the image and check if white falls in left, mid or right
        int step_size = img.step / 3;
        for (int i = 0; i < img.height; ++i)
        {
            for (int j = 0; j < img.step; ++j)
            {
                int index = i * img.step + j;
                if (img.data[index] == white_pixel)
                {
                    white_ball_found = true;
                    int position_index = step_size - j;
                    //ROS_INFO("position_index:%d step_size:%d j:%d index:%d", position_index, step_size, j, index);
                    if (position_index > 0 && position_index < step_size)
                    {
                       // Left side
                        ROS_INFO("white ball in left side");
                        drive_robot(0.5, 0.5);
                    } else if (position_index < 0 && std::abs(position_index) < step_size)
                    {
                        // Middle side
                        ROS_INFO("white ball in middle side");
                        drive_robot(0.5, 0.0);

                    } else {
                        // Right side
                        ROS_INFO("white ball in right side");
                        drive_robot(0.5, -0.5);
                    }
                    break;
                }
            }
            if (white_ball_found)
            {
                break;
            }
        }
        if (!white_ball_found)
        {
            drive_robot(0.0, 0.0);
        }
    }
    
};





int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    ros::ServiceClient client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/drive");

    ImageProcessor *imageProcessor = new ImageProcessor(client);

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10,
        &ImageProcessor::process_image_callback, imageProcessor);

    // Handle ROS communication events
    ros::spin();

    return 0;
}