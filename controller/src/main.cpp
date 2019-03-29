#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <stdlib.h>

#include "kcftracker.hpp"
#include "controller.h"

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "controller/BoundingBox.h"

#include "std_msgs/Bool.h"
#include "std_msgs/Empty.h"
#include "std_msgs/String.h"

using namespace std;

//Controller::Controller() = default;
//Controller::~Controller() = default;

Controller::Controller(ros::NodeHandle* nodehandle):nh_(*nodehandle)
{ 
    ROS_INFO("initialize subscriber and publisher");
	initializeSubscriber();
	initializePublisher();
}


int image_center_x = 320;
int image_center_y = 240;


int main(int argc, char* argv[]) 
{

    ros::init(argc, argv, "control_node"); 
	ros::NodeHandle nh; 

    Controller ctrl(&nh);

	double x,y,z;
	x = 0;
	y = 0;
	z = 0;

	ctrl.takeoff();

	cout<<"drone_center_x: "<<ctrl.drone_center_x<<"drone_center_y: "<<ctrl.drone_center_y;	

    ros::Rate loop_rate(50);  
    while (ctrl.nh_.ok()) 
    { 
		x = 0;
		y = 0;
		z = 0;
		
		if(ctrl.drone_center_x < image_center_x)
			y = 0.5;
		if(ctrl.drone_center_x > image_center_x)
			y = -0.5;
		//if(ctrl.drone_center_y < image_center_y)
		//	z = 0.4;
		//if(ctrl.drone_center_y < image_center_y)
		//	z = -0.1;
		
		ctrl.piloting(x,y,z);

        ros::spinOnce();  
        loop_rate.sleep(); 
    }

}

