#ifndef RGBD_SYNC
#define RGBD_SYNC

#include <ros/ros.h>
#include <image_transport/image_transport.h>

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <fstream>
#include <sstream>
#include <string> 
#include <iostream> 
#include <chrono>


class RGBD_Sync
{
    public:
        RGBD_Sync(std::string const &dataset,std::string const &path_rgb,std::string const &path_depth);
        ~RGBD_Sync();
        void imageCallback(const sensor_msgs::ImageConstPtr& msgRGB,const sensor_msgs::ImageConstPtr& msgD);
    private:
        std::ofstream rgb_file, depth_file;
        std::string folder;
        

};

#endif