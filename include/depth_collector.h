#ifndef DEPTH_COLLECTOR
#define DEPTH_COLLECTOR

#include <ros/ros.h>
#include <image_transport/image_transport.h>

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <fstream>
#include <sstream>
#include <string> 
#include <iostream> 
#include <chrono>
#include <algorithm>


class Depth_Collector
{
    public:
        Depth_Collector(std::string const &dataset,std::string const &path);
        ~Depth_Collector();
        void imageCallback(const sensor_msgs::ImageConstPtr& msg );
    private:
        std::ofstream depth_file;
        std::string folder;

};

#endif