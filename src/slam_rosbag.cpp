
//=====================================================================
//Title: ROS package for sensor syncing and to create SLAM dataset via rosbags 
//The sensors that are being synced are a RGB-D realsense camera, IMU and a Vicon motion camera system
//Date: 10/01/2024
//Author: Fischer @ iit
//Usage: rosrun slam_rosbag_sync sync_sensors
//====================================================================

#include "sensor_sync.h"

//links:
// https://answers.ros.org/question/212747/cvbridge-conversion-problem-asus-xtion-depth-image-to-opencv/
// https://answers.ros.org/question/11810/how-to-pass-arguments-tofrom-subscriber-callback-functions/
//

//===========================================================
int main(int argc, char **argv)
{
  ros::init(argc, argv, "sync_sensors"); // ros node name
  ros::NodeHandle nh;


  // subscriber for color cam in case rgbd camera model localization
  message_filters::Subscriber<sensor_msgs::Image> sub_rgb(nh,"/camera/color/image_raw", 10);
  // subscriber for depth cam in case rgbd camera model localization
  message_filters::Subscriber<sensor_msgs::Image> sub_depth(nh,"/camera/aligned_depth_to_color/image_raw", 10);
  //subscriber for IMU measurements
  message_filters::Subscriber<sensor_msgs::Imu> sub_imu(nh,"/imu",10);
  //subscriber for Vicon camera motion system
  message_filters::Subscriber<geometry_msgs::PoseStamped> sub_vicon(nh,"/vicon",10);

  Sensor_Sync obj(nh);

  //sync subscribers
  typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image,sensor_msgs::Imu,geometry_msgs::PoseStamped> sync_pol;
  message_filters::Synchronizer<sync_pol> sync(sync_pol(10),sub_rgb,sub_depth,sub_imu,sub_vicon);
  sync.registerCallback(boost::bind(&Sensor_Sync::callback,&obj,_1,_2,_3,_4));
 
  ros::spin();
}