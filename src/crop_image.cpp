// rosrun slam_dataset_rospkg crop_image _center_x:=INT _center_y:=INT _crop_width:=INT _crop_height:=INT
#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>

class ImageCropper
{
public:
    ImageCropper(ros::NodeHandle& nh)
    {

        std::string input_topic= "";
        std::string output_topic= "";

        // Define the image transport and subscriber
        image_transport::ImageTransport it(nh);
        image_sub_ = it.subscribe(input_topic, 1, &ImageCropper::imageCallback, this);
        
        // Define the publisher for the cropped image
        image_pub_ = it.advertise(output_topic, 1);

        // Set crop parameters (center_x, center_y, width, height)
        nh.getParam("center_x", center_x_);
        nh.getParam("center_y", center_y_);
        nh.getParam("crop_width", crop_width_);
        nh.getParam("crop_height", crop_height_);
    }

    void imageCallback(const sensor_msgs::ImageConstPtr& msg)
    {
        // Convert the ROS image message to OpenCV format
        cv_bridge::CvImagePtr cv_ptr;
        try
        {
            cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        }
        catch (cv_bridge::Exception& e)
        {
            ROS_ERROR("cv_bridge exception: %s", e.what());
            return;
        }

        // Get the original image
        cv::Mat original_image = cv_ptr->image;

        // Calculate the top-left corner of the crop region
        int x = std::max(center_x_ - crop_width_ / 2, 0);
        int y = std::max(center_y_ - crop_height_ / 2, 0);

        // Ensure the crop region doesn't go out of bounds
        int width = std::min(crop_width_, original_image.cols - x);
        int height = std::min(crop_height_, original_image.rows - y);

        // Crop the image using OpenCV
        cv::Rect crop_region(x, y, width, height);
        cv::Mat cropped_image = original_image(crop_region);

        // Convert the cropped image back to a ROS image message
        sensor_msgs::ImagePtr output_msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", cropped_image).toImageMsg();

        // Publish the cropped image
        image_pub_.publish(output_msg);
    }

private:
    image_transport::Subscriber image_sub_;
    image_transport::Publisher image_pub_;

    int center_x_, center_y_, crop_width_, crop_height_;
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "image_cropper_node");
    ros::NodeHandle nh("~");

    // Create the ImageCropper object
    ImageCropper image_cropper(nh);

    // Spin
    ros::spin();

    return 0;
}