//Title: ROS SLAM data collector using RealSense RGBD camera
//Date: 17/01/2024
//Author: Fischer @ iit
//Usage: rosrun slam_dataset_rospkg collect_sync _dataset_path:=[DST DATASET PATH]

#include <ros/ros.h>
#include <image_transport/image_transport.h>

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <time.h>
#include <fstream>
#include <boost/assign/list_of.hpp>
#include "rgbd_sync.h"
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

//links:
// https://answers.ros.org/question/212747/cvbridge-conversion-problem-asus-xtion-depth-image-to-opencv/
// https://answers.ros.org/question/11810/how-to-pass-arguments-tofrom-subscriber-callback-functions/
//

//typedef const boost::function< void(const sensor_msgs::ImageConstPtr &)>  callback;
//===========================================================
int main(int argc, char **argv)
{
  std::string param;
  ros::init(argc, argv, "collect_sync"); // ros node name
  ros::NodeHandle nh("~");

  //get Time and Data
  auto time_date = std::chrono::system_clock::to_time_t((std::chrono::system_clock::now()));
  tm local_tm = *localtime(&time_date);

  //create store dataset path
  std::string check; 
  std::string dataset0;
  nh.getParam("dataset_path", check);
  std::cout<<check<<"\n";

  if(check.empty())
  {
    //get pwd
    char cwd[PATH_MAX];
    std::stringstream ss;
    if (getcwd(cwd, sizeof(cwd)) != NULL) 
    {
      ss << cwd;
      ss >> dataset0;
    }
  }
  else {dataset0 = check;}

  std::cout<< "DST dataset path: " << dataset0 << std::endl; //input map

  std::string time_data_s = std::to_string(local_tm.tm_year+1900) + "_" + std::to_string(local_tm.tm_mon) + "_" + std::to_string(local_tm.tm_mday) + "_" + std::to_string(local_tm.tm_hour) + "_" + std::to_string(local_tm.tm_min) + "_" + std::to_string(local_tm.tm_sec);
  std::string dataset = dataset0 + "/" + time_data_s;
  mkdir(dataset.c_str(),0777); //create folder for the whole Dataset

  std::string rgb_file = dataset + "/rgb/rgb.txt"; //rgb txt file path 
  std::string depth_file = dataset + "/depth/depth.txt"; //depth file path
  mkdir((dataset+"/rgb").c_str(),0777); //create folder for rgb data
  mkdir((dataset+"/depth").c_str(),0777); // create folder for depth information

  
  // subscriber for color cam in case rgbd camera model localization
  message_filters::Subscriber<sensor_msgs::Image> sub_rgb(nh,"/d435/color/image_raw", 10);
  // subscriber for depth cam in case rgbd camera model localization
  message_filters::Subscriber<sensor_msgs::Image> sub_depth(nh,"/d435/aligned_depth_to_color/image_raw", 10);
  
  RGBD_Sync obj(dataset,rgb_file,depth_file);

  //sync subscribers
  typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image> sync_pol;
  message_filters::Synchronizer<sync_pol> sync(sync_pol(10),sub_rgb,sub_depth);
  sync.registerCallback(boost::bind(&RGBD_Sync::imageCallback,&obj,_1,_2));
 
  ros::spin();
}