//Title: ROS SLAM data collector using RealSense RGBD camera
//Date: 22/01/2022
//Author: Fischer @ LabRob
//Usage: rosrun slam_data_collection collect _rect_flag:=true
//                                               (OPTIONAL)

#include <ros/ros.h>
#include <image_transport/image_transport.h>

#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <time.h>
#include <fstream>
#include <boost/assign/list_of.hpp>
#include "rgb_collector.h"
#include "depth_collector.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <bits/stdc++.h>
#include <chrono>
#include <ctime>

//links:
// https://answers.ros.org/question/212747/cvbridge-conversion-problem-asus-xtion-depth-image-to-opencv/
// https://answers.ros.org/question/11810/how-to-pass-arguments-tofrom-subscriber-callback-functions/
//

typedef const boost::function< void(const sensor_msgs::ImageConstPtr &)>  callback;
//===========================================================
int main(int argc, char **argv)
{
  std::string param;
  ros::init(argc, argv, "collect"); // ros node name
  ros::NodeHandle nh;

  //get user input parameter
  bool rect_flag = false;
  if(nh.getParam("rect_flag", param)) 
  {
    ROS_INFO("saving rect images");
    rect_flag = true;
  }

  //get Time and Data
  auto time_date = std::chrono::system_clock::to_time_t((std::chrono::system_clock::now()));
  tm local_tm = *localtime(&time_date);

  //create store dataset path
  std::string dataset0 = "/home/fischer/Desktop/Fischer/Projects/Research/SLAM/REALSENSE/data/";
  std::string time_data_s = std::to_string(local_tm.tm_year+1900) + "_feb_" + std::to_string(local_tm.tm_mday) + "_" + std::to_string(local_tm.tm_hour) + "_" + std::to_string(local_tm.tm_min) + "_" + std::to_string(local_tm.tm_sec);
  std::string dataset = dataset0 + time_data_s;
  mkdir(dataset.c_str(),0777); //create folder for the whole Dataset

  std::string rgb_file = dataset + "/rgb/rgb.txt"; //rgb txt file path 
  std::string depth_file = dataset + "/depth/depth.txt"; //depth file path
  mkdir((dataset+"/rgb").c_str(),0777); //create folder for rgb data
  mkdir((dataset+"/depth").c_str(),0777); // create folder for depth information

  // Data collector class
  RGB_Collector rgb(dataset,rgb_file);
  Depth_Collector depth(dataset,depth_file);

  // Creating ROS callback functions
  image_transport::ImageTransport it(nh);
  callback boundImageCallback_rgb = boost::bind(&RGB_Collector::imageCallback, &rgb/*&images*/, _1);
  callback boundImageCallback_depth = boost::bind(&Depth_Collector::imageCallback, &depth/*&images*/, _1);

  //Subscribe to topics 
  image_transport::Subscriber sub_rgb = it.subscribe("/camera/color/image_raw", 10, boundImageCallback_rgb);
  image_transport::Subscriber sub_depth = it.subscribe("/camera/aligned_depth_to_color/image_raw", 10, boundImageCallback_depth);
  /*
  //Realsense camera provides more image information
  //THis flag sets these extra images to be saved as well
  if(rect_flag) 
  {
    std::string rgb_rect_file = dataset + "/rgb_rect/rgb.txt";
    std::string depth_rect_file = dataset + "/depth_rect/depth.txt";
    mkdir((dataset+"/rgb_rect").c_str(),0777);
    mkdir((dataset+"/depth_rect").c_str(),0777);

    RGB_Collector rgb_rect(dataset,rgb_rect_file);
    Depth_Collector depth_rect(dataset,depth_rect_file);

    */
     //callback boundImageCallback_rgb_rect = boost::bind(&RGB_Collector::imageCallback, &rgb_rect/*&images*/, _1);
    //callback boundImageCallback_depth_rect = boost::bind(&Depth_Collector::imageCallback, &depth_rect/*&images*/, _1);
    
    //image_transport::Subscriber sub_rgb_rect = it.subscribe("/camera/color/image_rect_color", 10, boundImageCallback_rgb_rect);
    //image_transport::Subscriber sub_depth_rect = it.subscribe("/camera/depth/image_rect_raw", 10, boundImageCallback_depth_rect);

  //}
 
  ros::spin();
}