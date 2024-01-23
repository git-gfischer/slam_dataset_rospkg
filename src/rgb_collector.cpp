#include "rgb_collector.h"

RGB_Collector::RGB_Collector(std::string const  &dataset, std::string const  &path)
                            : rgb_file(path.c_str(), std::ios::out) 
{
    folder = dataset;
    rgb_file<<"# color images \n";
    rgb_file<<"# timestamp filename \n";
}
//===============================================
RGB_Collector::~RGB_Collector()
{
    rgb_file.close();
    std::cout<<"rgb.txt file saved"<<"\n";
}
//==============================================
void RGB_Collector::imageCallback(const sensor_msgs::ImageConstPtr& msg)
{   
    cv::Mat rgb_image;
    try {rgb_image = cv_bridge::toCvShare(msg, "bgr8")->image; }
    catch (cv_bridge::Exception& e)
    {
        ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
    }

    

    //get timestamp
    //uint64 us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    double us = static_cast<double>(msg->header.stamp.sec) + static_cast<double>(msg->header.stamp.nsec) / 1e9;
    std::string timestamp = std::to_string(us);
    timestamp.insert(timestamp.end()-6,1,'.');

    std::string filename_rgb= this->folder+"/rgb/"+timestamp+".png";
    std::string new_line_rgb= timestamp + " " + "rgb/" + timestamp+".png \n";
    
    rgb_file << new_line_rgb;

    cv::imwrite(filename_rgb,rgb_image);

    std::cout << filename_rgb << " " << new_line_rgb <<"\n";

    //show rgb image
    //cv::imshow("rgb", rgb_image);
    //cv::waitKey(30);
  
}
