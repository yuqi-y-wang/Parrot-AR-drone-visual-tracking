#ifndef PROJECT_CONTROLLER_H
#define PROJECT_CONTROLLER_H

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include "std_msgs/Empty.h"
#include "std_msgs/String.h"
#include "controller/BoundingBox.h"

using namespace std;

class Controller {

private:

	const std::string RECEIVE_BOUNDINGBOX_NAME = "/KCF/boundingbox";
	const double duration = 5.0;


public:

    //explicit Controller();
    //virtual ~Controller();
	
    Controller(ros::NodeHandle* nodehandle); 
	controller::BoundingBox bbox;

	ros::NodeHandle nh_; 
    ros::Subscriber sub; 
    ros::Publisher  pub;

	double drone_center_x;
	double drone_center_y;

	void initializeSubscriber(); 
    void initializePublisher();

	void boxCallback(controller::BoundingBox tmp);
	void takeoff();
	void land();
	void piloting(double x, double y, double z);

};

#endif
