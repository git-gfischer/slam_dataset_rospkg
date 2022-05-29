# SLAM_Realsense_ROS
ROS package that collects data from RGB-D Realsense camera and stores it in a SLAM dataset format

## Dependencies
This package was developt using ROS-melodic and Ubuntu18.04. </br>

install the realsense-viewer at : https://github.com/IntelRealSense/librealsense/blob/master/doc/distribution_linux.md </br>

Run the following commands to install the dependencies:

```
sudo apt-get install ros-melodic-realsense2-camera
sudo apt-get install ros-melodic-openni-launch
```

After installed the dependencies and finish the compilation, make the following edits to the file:```/opt/ros/melodic/share/realsense2_camera/launch ``` </br>
1.    edit the width, height of the rgb image for 848,480
2.    edit the width, height of the depth image  for 848,480
3.    edit the fps of the image for 30
4.    edit the fps of the depth for 30

## Run package
On a terminal run: 
```
roslaunch realsense2_camera rs_rgbd.launch
```
And on another terminal run the data collection package, if you don't provide the dataset path the system will store the data on the current folder:
```
rosrun slam_realsense_ros collect_sync _param:=[DST_DATASET_PATH]
```

## Run package with roslaunch
To run the package with camera visualization in parallel to the data acquisition, execute the following command:
```
roslaunch slam_realsense_ros  slam_data_collection.launch
```

