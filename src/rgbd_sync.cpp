#include "rgbd_sync.h"

RGBD_Sync::RGBD_Sync(std::string const  &dataset, std::string const  &path_rgb, std::string const &path_depth)
                            : rgb_file(path_rgb.c_str(), std::ios::out), depth_file(path_depth, std::ios::out) 
{
    folder = dataset;
    rgb_file<<"# color images \n";
    rgb_file<<"# timestamp filename \n";
    depth_file<<"# depth images \n";
    depth_file<<"# timestamp filename \n";
}
//===============================================
RGBD_Sync::~RGBD_Sync()
{
    rgb_file.close();
    depth_file.close();
    std::cout<<"rgb.txt file saved"<<"\n";
    std::cout<<"depth.txt file saved" <<"\n";
}
//==============================================
void RGBD_Sync::imageCallback(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD)
{   
    //get rgb image
    cv::Mat rgb_image;
    try {rgb_image = cv_bridge::toCvShare(msgRGB, "bgr8")->image; }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msgRGB->encoding.c_str());
        return;
    }

    //get depth image
    cv_bridge::CvImageConstPtr cv_ptr;
    cv::Mat depth_img;
    //Convert from the ROS image message to a CvImage suitable for working with OpenCV for processing
    try{cv_ptr = cv_bridge::toCvShare(msgD,sensor_msgs::image_encodings::TYPE_16UC1);}
    catch (cv_bridge::Exception& e)
    {
        //if there is an error during conversion, display it
        ROS_ERROR("Could not convert from '%s' to '16UC1'.", e.what());
        return;
    }
    depth_img = cv_ptr->image; 

    //get timestamp
    //uint64 us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    double us_rgb = static_cast<double>(msgRGB->header.stamp.sec) + static_cast<double>(msgRGB->header.stamp.nsec) / 1e9;
    double us_depth = static_cast<double>(msgD->header.stamp.sec) + static_cast<double>(msgD->header.stamp.nsec) / 1e9;

    std::string timestamp_rgb = std::to_string(us_rgb);
    std::string timestamp_depth = std::to_string(us_depth);
    //timestamp_rgb.insert(timestamp_rgb.end()-6,1,'.');
    //timestamp_depth.insert(timestamp_depth.end()-6,1,'.');

    //ROS_INFO("%s",timestamp_rgb.c_str() );
    //ROS_INFO("%s",timestamp_depth.c_str() );


    std::string filename_rgb= folder+"/rgb/"+timestamp_rgb+".png";
    std::string new_line_rgb= timestamp_rgb + " " + "rgb/" + timestamp_rgb+".png \n";


    std::string filename_depth= folder+"/depth/"+timestamp_depth+".png";
    std::string new_line_depth= timestamp_depth + " " + "depth/" + timestamp_depth+".png \n";
    
    
    rgb_file << new_line_rgb;
    depth_file <<new_line_depth;

    //saving images
    cv::imwrite(filename_rgb,rgb_image);
    cv::imwrite(filename_depth,depth_img);

    std::cout << "file saved:" << folder << " " <<timestamp_rgb <<"\n";

    //show rgb image
    //cv::imshow("rgb", rgb_image);
    //cv::waitKey(30);
  
}