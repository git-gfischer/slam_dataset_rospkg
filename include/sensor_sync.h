#pragma once

#include <ros/ros.h>
#include <image_transport/image_transport.h>

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <time.h>
#include <fstream>
#include <boost/assign/list_of.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <bits/stdc++.h>
#include <chrono>
#include <algorithm>
#include <ctime>
#include <string>
#include <sstream>

#include <unistd.h>
#include <stdio.h>
#include <limits.h>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/Imu.h>
#include <geometry_msgs/PoseStamped.h>


class Sensor_Sync
{
    public:
        Sensor_Sync(ros::NodeHandle nh);
        ~Sensor_Sync();
        void callback(const sensor_msgs::ImageConstPtr& msgRGB,
                      const sensor_msgs::ImageConstPtr& msgD,
                      const sensor_msgs::ImuConstPtr& msgIMU,
                      const geometry_msgs::PoseStampedConstPtr& msgVICON);
    private:
        ros::Publisher *pub_rgb;
        ros::Publisher *pub_depth;
        ros::Publisher *pub_imu;
        ros::Publisher *pub_vicon;

};