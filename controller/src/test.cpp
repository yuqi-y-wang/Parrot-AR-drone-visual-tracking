#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <stdlib.h>

#include "kcftracker.hpp"
#include "controller.h"

#include <ros/ros.h>
#include "controller/BoundingBox.h"

#include "std_msgs/Bool.h"
#include "std_msgs/Empty.h"
#include "std_msgs/String.h"

#include <stdio.h>


using namespace std;

const std::string RECEIVE_BOUNDINGBOX_NAME = "/KCF/boundingbox";
controller::BoundingBox bbox;

void boxCallback(controller::BoundingBox tmp){
	ROS_INFO("in call back ");
    bbox.x = tmp.x;
    bbox.y = tmp.y;
    bbox.width = tmp.width;
    bbox.height = tmp.height;
	std::cout<<"bbox.x: "<<bbox.x<<"bbox.y: "<<bbox.y;

}

int main(int argc, char* argv[]) 
{

    ros::init(argc, argv, "test_node"); 
	ros::NodeHandle nh; 
	
	ros::Subscriber sub = nh.subscribe(RECEIVE_BOUNDINGBOX_NAME, 1, boxCallback); 
    ros::Publisher pub = nh.advertise<controller::BoundingBox>("/test", 1);

    ros::Rate loop_rate(50);  
    while (nh.ok()) 
    { 
		ROS_INFO("in while loop");
		
		pub.publish(bbox);

		ROS_INFO("in while loop bottom");
        ros::spinOnce();  
        loop_rate.sleep(); 
    }

}
