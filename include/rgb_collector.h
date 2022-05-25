#ifndef RGB_COLLECTOR
#define RGB_COLLECTOR

#include <ros/ros.h>
#include <image_transport/image_transport.h>

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <fstream>
#include <sstream>
#include <string> 
#include <iostream> 
#include <chrono>


class RGB_Collector
{
    public:
        RGB_Collector(std::string const &dataset,std::string const &path);
        ~RGB_Collector();
        void imageCallback(const sensor_msgs::ImageConstPtr& msg );
    private:
        std::ofstream rgb_file;
        std::string folder;
        

};

#endif