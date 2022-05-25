#include "depth_collector.h"

Depth_Collector::Depth_Collector(std::string const  &dataset, std::string const  &path)
                            : depth_file(path.c_str(), std::ios::out) 
{
    folder = dataset;
    depth_file<<"# depth images \n";
    depth_file<<"# timestamp filename \n";
}
//===============================================
Depth_Collector::~Depth_Collector()
{
    depth_file.close();
    std::cout<<"depth.txt file saved"<<"\n";
}
//==============================================
void Depth_Collector::imageCallback(const sensor_msgs::ImageConstPtr& msg) 
{
    //
    cv_bridge::CvImageConstPtr cv_ptr;
    //Convert from the ROS image message to a CvImage suitable for working with OpenCV for processing
    try
    {
        //Always copy, returning a mutable CvImage
        //OpenCV expects color images to use BGR channel order.
        //cv_ptr = cv_bridge::toCvCopy(msg,sensor_msgs::image_encodings::TYPE_16UC1);
        cv_ptr = cv_bridge::toCvShare(msg,sensor_msgs::image_encodings::TYPE_16UC1);

    }
    catch (cv_bridge::Exception& e)
    {
        //if there is an error during conversion, display it
        ROS_ERROR("tutorialROSOpenCV::main.cpp::cv_bridge exception: %s", e.what());
        return;
    }

    //Copy the image.data to imageBuf.
    cv::Mat depth_float_img = cv_ptr->image;  
    
    //get timestamp
    uint64 us = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    std::string timestamp = std::to_string(us);
    timestamp.insert(timestamp.end()-6,1,'.');
    
    std::string filename_depth= this->folder+"/depth/"+timestamp+".png";
    std::string new_line_depth= timestamp + " " + "depth/" + timestamp+".png \n";
    
    depth_file << new_line_depth;

    cv::imwrite(filename_depth,depth_float_img);

    std::cout << filename_depth << " " << new_line_depth <<"\n";


}