#ifndef PROJECT_CONTROLLER_H
#define PROJECT_CONTROLLER_H

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "std_msgs/Empty.h"
#include "std_msgs/String.h"


using namespace std;

class Controller {

//private:
   
public:
	ros::NodeHandle nh;
    ros::Publisher pub;
	double duration = 5.0;

    explicit Controller();
    virtual ~Controller();

    void takeoff(){
    pub = nh.advertise<std_msgs::Empty>("/ardrone/takeoff", 1, true);

    double time_start = ros::Time::now().toSec();
    while (ros::Time::now().toSec() < time_start + duration) {
        pub.publish(std_msgs::Empty());
        ros::spinOnce();
    }
    ROS_INFO(" taken off successfully");
}

    void land(){
    pub = nh.advertise<std_msgs::Empty>("/ardrone/land", 1, true);

    double time_start = ros::Time::now().toSec();
    while (ros::Time::now().toSec() < time_start + duration) {
        pub.publish(std_msgs::Empty());
        ros::spinOnce();
    }
    ROS_INFO(" landed successfully");
}

    void emergency();
    void piloting(double x, double y, double z){
    geometry_msgs::Twist coordinates;

    pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel", 5);

    coordinates.linear.x = x;
    coordinates.linear.y = y;
    coordinates.linear.z = z;

    double time_start = ros::Time::now().toSec();
    while (ros::Time::now().toSec() < time_start + duration) {
        pub.publish(coordinates);
        ros::spinOnce();
    }
    ROS_INFO("coordinates: linear.x: %lf y: %lf z: %lf", coordinates.linear.x,coordinates.linear.y,coordinates.linear.z);
}

};

#endif
