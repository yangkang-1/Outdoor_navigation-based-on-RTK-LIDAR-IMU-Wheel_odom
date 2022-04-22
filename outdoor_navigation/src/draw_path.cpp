#include "ros/ros.h"
#include "tf2_ros/transform_listener.h"
#include "geometry_msgs/TransformStamped.h"
#include "nav_msgs/Path.h"
#include "nav_msgs/Odometry.h"

nav_msgs::Path ros_path_;
geometry_msgs::PoseStamped pose_path;
void filterCallback(const nav_msgs::OdometryConstPtr &msg)
{
    pose_path.pose.position = msg->pose.pose.position ; 
    pose_path.pose.orientation = msg->pose.pose.orientation ; 
    ros_path_.poses.push_back(pose_path); 
}
int main(int argc, char *argv[])
{
    ros::init(argc,argv,"draw_path");
    ros::NodeHandle nh;

    //tf trans
    tf2_ros::Buffer buffer;
    tf2_ros::TransformListener Listener_path(buffer);
    geometry_msgs::TransformStamped pose;
    
    ros::Rate rate(10);

    ros::Publisher path_ = nh.advertise<nav_msgs::Path>("rtk_path",10) ;
    ros::Subscriber path_Sub = nh.subscribe("/odometry/filtered_map", 10, &filterCallback);

    
    while (ros::ok())
    {
        ros_path_.header.seq = 100 ;
        ros_path_.header.frame_id = "map" ;
        ros_path_.header.stamp = ros::Time::now() ;
        pose_path.header = ros_path_.header ;
        
        // try
        // {
        //     pose = buffer.lookupTransform("base_link","map",ros::Time::now()) ; 
        //     pose_path.pose.position.x = pose.transform.translation.x;
        //     pose_path.pose.position.y = pose.transform.translation.y;
        //     pose_path.pose.position.z = pose.transform.translation.z;
        //     pose_path.pose.orientation = pose.transform.rotation;
        //     ros_path_.poses.push_back(pose_path);
        // }
        // catch(const std::exception& e)
        // {
        //     std::cerr << e.what() << '\n';
        // }

        path_.publish(ros_path_);
        rate.sleep();
        ros::spinOnce();
    }
    
}