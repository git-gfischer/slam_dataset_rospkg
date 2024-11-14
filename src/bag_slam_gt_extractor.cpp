#include <ros/ros.h>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include "geometry_msgs/TransformStamped.h"

#include <iostream>
#include <fstream>

//std::ofstream myfile1;
std::ofstream gtfile;

int count_digit(int number) {
   int count = 0;
   while(number != 0) {
      number = number / 10;
      count++;
   }
   return count;
}


//void MultiSubscriberCB(const sensor_msgs::ImuConstPtr& msg_imu, const sensor_msgs::CameraInfoConstPtr& msg_camera_info) {
void gtCallback(const geometry_msgs::TransformStamped::ConstPtr& msg) {
    //ROS_INFO("inside callback");
    
    std::stringstream ss;
      
    double timestamp = static_cast<double>(msg->header.stamp.sec) + static_cast<double>(msg->header.stamp.nsec) / 1e9;

    ROS_INFO("Received TransformStamped message:");
    ROS_INFO("Header Stamp sec: %f", static_cast<double>(msg->header.stamp.sec));
    ROS_INFO("Header Stamp nsec: %f", static_cast<double>(msg->header.stamp.nsec));
    ROS_INFO("Header Stamp: %f", timestamp);
    ROS_INFO("Child Frame ID: %s", msg->child_frame_id.c_str());
    ROS_INFO("Translation: x=%f, y=%f, z=%f", msg->transform.translation.x, msg->transform.translation.y, msg->transform.translation.z);
    ROS_INFO("Rotation: x=%f, y=%f, z=%f, w=%f", msg->transform.rotation.x, msg->transform.rotation.y, msg->transform.rotation.z, msg->transform.rotation.w);

    

    ss << std::to_string(timestamp) << " " 
       << msg->transform.translation.x << " "
       << msg->transform.translation.y << " "
       << msg->transform.translation.z << " "
       << msg->transform.rotation.x << " "
       << msg->transform.rotation.y << " "
       << msg->transform.rotation.z << " "
       << msg->transform.rotation.w;
    
    gtfile << ss.str()+"\n";

    // if(count_digit(msg_imu->header.stamp.nsec) == 0){
    //     ss << msg_imu->header.stamp.sec << "000000000";
    //     //std::cout << ss.str() << std::endl;
    // }
    // else if(count_digit(msg_imu->header.stamp.nsec) == 1){
    //     ss << msg_imu->header.stamp.sec << "00000000" << msg_imu->header.stamp.nsec;
    //     //std::cout << ss.str() << std::endl;
    // }
    // else if(count_digit(msg_imu->header.stamp.nsec) == 2){
    //     ss << msg_imu->header.stamp.sec << "0000000" << msg_imu->header.stamp.nsec;
    //     //std::cout << ss.str() << std::endl;
    // }
    // else if(count_digit(msg_imu->header.stamp.nsec) == 3){
    //     ss << msg_imu->header.stamp.sec << "000000" << msg_imu->header.stamp.nsec;
    //     //std::cout << ss.str() << std::endl;
    // }
    // else if(count_digit(msg_imu->header.stamp.nsec) == 4){
    //     ss << msg_imu->header.stamp.sec << "00000" << msg_imu->header.stamp.nsec;
    //     //std::cout << ss.str() << std::endl;
    // }
    // else if(count_digit(msg_imu->header.stamp.nsec) == 5){
    //     ss << msg_imu->header.stamp.sec << "0000" << msg_imu->header.stamp.nsec;
    //     //std::cout << ss.str() << std::endl;
    // }
    // else if(count_digit(msg_imu->header.stamp.nsec) == 6){
    //     ss << msg_imu->header.stamp.sec << "000" << msg_imu->header.stamp.nsec;
    //     //std::cout << ss.str() << std::endl;
    // }
    // else if(count_digit(msg_imu->header.stamp.nsec) == 7){
    //     ss << msg_imu->header.stamp.sec << "00" << msg_imu->header.stamp.nsec;
    //     //std::cout << ss.str() << std::endl;
    // }
    // else if(count_digit(msg_imu->header.stamp.nsec) == 8){
    // ss << msg_imu->header.stamp.sec << "0" << msg_imu->header.stamp.nsec;
    // //std::cout << ss.str() << std::endl;
    // }
    // else{
    // ss << msg_imu->header.stamp.sec << msg_imu->header.stamp.nsec;
    // //std::cout << ss.str() << std::endl;  
    // }     
    
    
    // ss << "," << msg_imu->angular_velocity.x << "," << msg_imu->angular_velocity.y << "," << msg_imu->angular_velocity.z << "," << msg_imu->linear_acceleration.x << "," << msg_imu->linear_acceleration.y << "," << msg_imu->linear_acceleration.z;
    // //ss << msg_camera_info->header.stamp.sec << msg_camera_info->header.stamp.nsec;
    // std::cout << ss.str() << std::endl;  
    
    // //cv::imwrite("save/"+ss.str()+".png",cv_ptrD->image);
    
    
    // //myfile << ss.str()+" "+"rgb/"+ss.str()+".png"+"\n";
    // gtfile << ss.str()+"\n";
}

int main(int argc, char** argv) {

    ROS_INFO("Extracting ground truth and saving it in .txt file");
    ros::init(argc, argv, "gt_extractor");
    //myfile1.open("timestamps.txt");
    gtfile.open("ground_truth.txt");
    ros::NodeHandle nh("~");

    std::string gt_topic;
    nh.param<std::string>("topic", gt_topic, "/vicon/vero_state_estimation/");

    // Create a subscriber for the geometry_msgs/TransformStamped topic
    ros::Subscriber gt_subscriber = nh.subscribe(gt_topic, 10, gtCallback);


    ros::spin();
    //myfile1.close();
    gtfile.close();
    return 0;
}

