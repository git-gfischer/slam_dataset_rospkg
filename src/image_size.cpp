// Get the image size based on an image topic
//Usage: rosrun slam_dataset_rospkg image_size_subscriber _image_topic:=<TOPIC NAME>


#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try
    {
        // Convert the ROS image message to an OpenCV Mat
        cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);

        // Get the image size (width and height)
        int width = cv_ptr->image.cols;
        int height = cv_ptr->image.rows;

        cv::imshow("rgb", cv_ptr->image);
        // Wait for 1 millisecond and check if 'q' is pressed to exit
        if (cv::waitKey(1) == 'q') {return;}

        // Output the image size
        ROS_INFO("Image size: width = %d, height = %d", width, height);
    }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("cv_bridge exception: %s", e.what());
    }
}

int main(int argc, char** argv)
{
    // Initialize the ROS node
    ros::init(argc, argv, "image_size_subscriber");

    // Create a ROS node handle
    ros::NodeHandle nh("~");

    // Default image topic (in case a parameter is not provided)
    std::string image_topic = "/camera/rgb/image_raw";

    // Get the topic parameter from the command line or launch file
    if (nh.getParam("image_topic", image_topic))
    {
        ROS_INFO("Subscribed to image topic: %s", image_topic.c_str());
    }
    else
    {
        ROS_WARN("No image topic specified. Using default: %s", image_topic.c_str());
    }

    // Subscribe to the image topic
    ros::Subscriber image_sub = nh.subscribe(image_topic, 1, imageCallback);

    // Spin to keep the node running and processing callbacks
    ros::spin();

    return 0;
}