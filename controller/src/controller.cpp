#include "controller.h"

//Controller::Controller() = default;
//Controller::~Controller() = default;


void Controller::initializeSubscriber(){
    ROS_INFO("Initializing Subscriber");
    sub = nh_.subscribe(RECEIVE_BOUNDINGBOX_NAME, 1, &Controller::boxCallback, this);  
}

void Controller::initializePublisher(){
    ROS_INFO("Initializing Publisher");	
    //pub = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1, true);
}


void Controller::boxCallback(controller::BoundingBox tmp){
	ROS_INFO("In call back! ");
    bbox.x = tmp.x;
    bbox.y = tmp.y;
    bbox.width = tmp.width;
    bbox.height = tmp.height;
	drone_center_x = bbox.x + bbox.width/2;
	drone_center_y = bbox.y + bbox.height/2;
	std::cout<<"bbox.x: "<<bbox.x<<"bbox.y: "<<bbox.y << "drone_center_x: "<<drone_center_x<<"drone_center_y: "<<drone_center_y;

}

void Controller::takeoff(){
    pub = nh_.advertise<std_msgs::Empty>("/ardrone/takeoff", 1, true);

    double time_start = ros::Time::now().toSec();
    while (ros::Time::now().toSec() < time_start + duration) {
        pub.publish(std_msgs::Empty());
        ros::spinOnce();
    }
    ROS_INFO(" taken off successfully");
}

void Controller::land(){
    pub = nh_.advertise<std_msgs::Empty>("/ardrone/land", 1, true);

    double time_start = ros::Time::now().toSec();
    while (ros::Time::now().toSec() < time_start + duration) {
        pub.publish(std_msgs::Empty());
        ros::spinOnce();
    }
    ROS_INFO(" landed successfully");
}

void Controller::piloting(double x, double y, double z){
    geometry_msgs::Twist twist;
	pub = nh_.advertise<geometry_msgs::Twist>("/cmd_vel", 1, true);

    twist.linear.x = x;
    twist.linear.y = y;
    twist.linear.z = z;

    double time_start = ros::Time::now().toSec();
    while (ros::Time::now().toSec() < time_start + duration) {
        pub.publish(twist);
        ros::spinOnce();
    }
    cout<<" publishing: "<<twist<<endl;
}
