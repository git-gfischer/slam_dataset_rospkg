#include "sensor_sync.h"

Sensor_Sync::Sensor_Sync(ros::NodeHandle nh)
{
    pub_rgb = new ros::Publisher(nh.advertise<sensor_msgs::Image>("/camera/color/image_raw_sync", 10));
    pub_depth = new ros::Publisher(nh.advertise<sensor_msgs::Image>("/camera/aligned_depth_to_color/image_raw_sync", 10));
    pub_imu = new ros::Publisher(nh.advertise<sensor_msgs::Imu>("imu/imu_sync", 10));
    pub_vicon = new ros::Publisher(nh.advertise<geometry_msgs::PoseStamped>("vicon/pose_sync", 10));

}
//==============================================
Sensor_Sync::~Sensor_Sync(){}
//==============================================
void Sensor_Sync::callback(const sensor_msgs::ImageConstPtr& msgRGB,
                           const sensor_msgs::ImageConstPtr& msgD,
                           const sensor_msgs::ImuConstPtr& msgIMU,
                           const geometry_msgs::PoseStampedConstPtr& msgVICON)
{
    ROS_INFO("Sync callback \n");
    pub_rgb->publish(msgRGB);
    pub_depth->publish(msgD);
    pub_imu->publish(msgIMU);
    pub_vicon->publish(msgVICON);

}