# SLAM_Dataset_ROSPKG
ROS package that collects and synchronizes data from following sensors: RGB-D Realsense camera, IMU and VICON motion camera system in order to create SLAM datasets. The package stores the data in a rosbag and it also can convert the rosbag in a SLAM dataset format, such as TUM dataset. </br>
The synchorinization node works only if each topic have content. 

## Dependencies
This package was tested using: </br>
1. ROS-melodic and Ubuntu18.04. </br>
2. ROS-noetic and Ubuntu20.04. </br>

### Install Realsense camera package

install the realsense-viewer at : https://github.com/IntelRealSense/librealsense/blob/master/doc/distribution_linux.md </br>

Run the following commands to install the dependencies:

```
sudo apt-get install ros-$ROS_DISTRO-realsense2-camera
sudo apt-get install ros-$ROS_DISTRO-openni-launch
mkdir rosbag image_dataset
```

After installed the dependencies and finish the compilation, make the following edits to the file:```/opt/ros/melodic/share/realsense2_camera/launch/rs_rgbd.launch ``` </br>
1.    edit the "depth_width", "depth_height" of the depth image for 640,480 respectively.
2.    edit the "color_width", "color_height" of the rgb image  for 640,480 respectively.
3.    edit the "color_fps" of the image for 30
4.    edit the "depth_fps" of the depth for 30

## Run sensor synching executable
The synching executable can be run with the following command </br>
On a terminal run: 
```
roslaunch realsense2_camera rs_rgbd.launch
```
In a another terminal:
```
roslaunch slam_dataset_rospkg slam_rosbag_sync.launch
```

### Create the rosbag
Once the synching executable is running, the following topics will be created
```
/camera/color/image_raw_sync
/camera/depth/image_rect_raw_sync
/imu/imu_sync
/vicon/pose_sync
```

Run the following command in another terminal to start recording the rosbag 
```
rosbag record -O slam_dataset.bag /camera/color/image_raw_sync /camera/depth/image_rect_raw_sync /imu/imu_sync /vicon/pose_sync
```

## Run SLAM image dataset executable
This executable can be used standalone to collect synched data from realsense RGB-D camera or can convert the image data from the rosbag. You might need to have the topic names at the ```src/collect_sync.cpp``` depending on your system.
To run it standalone, run in a terminal the realsense camera launch: 
```
roslaunch realsense2_camera rs_rgbd.launch
```
And on another terminal run the data collection package, if you don't provide the dataset path the system will store the data on the current folder:
```
rosrun slam_dataset_rospkg collect_sync _dataset_path:=[DST_DATASET_PATH]
```

To convert a rosbag, run the rosbag play command:
```
rosbag play <PATH TO ROSBAG>
```
And run the  same command in another terminal:
```
rosrun slam_dataset_rospkg collect_sync _param:=[DST_DATASET_PATH]
```

### Run image dataset executable with roslaunch
To run the package with camera visualization in parallel to the data acquisition, execute the following command:
```
roslaunch slam_dataset_rospkg slam_image_dataset.launch
```

## Ground Truth extraction
To create a text file with the robots trajectory from a motion camera system, run the following command. You will need a terminal with roscore running and another terminal with your rosbag running as well.
```
rosrun slam_dataset_rospkg gt_extraction
```