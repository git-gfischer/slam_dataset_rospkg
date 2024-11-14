#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

// Callback function for depth image messages
void depthImageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        // Convert the ROS image message to an OpenCV Mat using the encoding from the message
        cv_bridge::CvImageConstPtr cv_ptr = cv_bridge::toCvShare(msg, msg->encoding);

        // Get the image size (width and height)
        int width = cv_ptr->image.cols;
        int height = cv_ptr->image.rows;

        // Output the image size
        ROS_INFO("Depth image size: width = %d, height = %d", width, height);

        // Optional: Access a depth value at a specific pixel (e.g., center pixel)
        int center_x = width / 2;
        int center_y = height / 2;

        cv::imshow("depth", cv_ptr->image);
        // Wait for 1 millisecond and check if 'q' is pressed to exit
        if (cv::waitKey(1) == 'q') {return;}

        // Handle different depth image encodings
        if (msg->encoding == sensor_msgs::image_encodings::TYPE_16UC1)
        {
            // For 16-bit unsigned depth images
            uint16_t depth_value = cv_ptr->image.at<uint16_t>(center_y, center_x);
            ROS_INFO("Depth value at center pixel: %u (in millimeters)", depth_value);
        }
        else if (msg->encoding == sensor_msgs::image_encodings::TYPE_32FC1)
        {
            // For 32-bit float depth images
            float depth_value = cv_ptr->image.at<float>(center_y, center_x);
            ROS_INFO("Depth value at center pixel: %f meters", depth_value);
        }
        else
        {
            ROS_WARN("Unsupported encoding: %s", msg->encoding.c_str());
        }
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }
}

int main(int argc, char** argv)
{
    // Initialize the ROS node
    ros::init(argc, argv, "depth_image_subscriber");

    // Create a private node handle to access private parameters
    ros::NodeHandle nh("~");

    // Default depth image topic
    std::string depth_image_topic = "/camera/depth/image_raw";

    // Get the depth image topic parameter
    if (nh.getParam("depth_image_topic", depth_image_topic))
    {
        ROS_INFO("Subscribed to depth image topic: %s", depth_image_topic.c_str());
    }
    else
    {
        ROS_WARN("No depth image topic specified. Using default: %s", depth_image_topic.c_str());
    }

    // Subscribe to the depth image topic using a global node handle
    ros::NodeHandle global_nh;
    ros::Subscriber depth_image_sub = global_nh.subscribe(depth_image_topic, 1, depthImageCallback);

    // Spin to process callbacks
    ros::spin();

    return 0;
}