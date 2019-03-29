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
#include "KCF/BoundingBox.h"

#include "std_msgs/Bool.h"
#include "std_msgs/Empty.h"
#include "std_msgs/String.h"

#include <stdio.h>
#include <termios.h>
 
static struct termios initial_settings, new_settings;
static int peek_character = -1;
void init_keyboard(void);
void close_keyboard(void);
int kbhit(void);
int readch(void); 

using namespace std;

Controller::Controller() = default;
Controller::~Controller() = default;

KCF::BoundingBox bbox;
double drone_center_x, drone_center_y;
int image_center_x = 320;
int image_center_y = 240;
const std::string RECEIVE_BOUNDINGBOX_NAME = "/KCF/boundingbox";


void boxCallback(KCF::BoundingBox tmp){

    bbox.x = tmp.x;
    bbox.y = tmp.y;
    bbox.width = tmp.width;
    bbox.height = tmp.height;

	drone_center_x = bbox.x + bbox.width/2;
	drone_center_y = bbox.y + bbox.height/2;
	std::cout<<"bbox.x: "<<bbox.x<<"bbox.y: "<<bbox.y << "drone_center_x: "<<drone_center_x<<"drone_center_y: "<<drone_center_y;
}


int main(int argc, char* argv[]) 
{

    ros::init(argc, argv, "control_node"); 
	Controller *pctrl = new Controller;
    ros::Subscriber sub = pctrl->nh.subscribe(RECEIVE_BOUNDINGBOX_NAME, 1000, boxCallback); 
	int x,y,z;
	x = 0;
	y = 0;
	z = 0;

	pctrl -> takeoff();
	
    ros::Rate loop_rate(50);  
    while (pctrl->nh.ok()) 
    { 
		x = 0;
		y = 0;
		z = 0;
		if(drone_center_x < image_center_x)
			y = -0.5;
		if(drone_center_x > image_center_x)
			y = 0.5;
		if(drone_center_y < image_center_y)
			z = 0.4;
		if(drone_center_y < image_center_y)
			z = -0.3;

		init_keyboard();
		if (kbhit()){
			if(readch()==27)
			break;
		}
		close_keyboard();
		
		pctrl -> piloting(x,y,z);

        ros::spin();  
        loop_rate.sleep(); 
    }

delete pctrl;

}

void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag |= ICANON;
    new_settings.c_lflag |= ECHO;
    new_settings.c_lflag |= ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
}
 
void close_keyboard()
{
    tcsetattr(0, TCSANOW, &initial_settings);
}
 
int kbhit()
{
    unsigned char ch;
    int nread;
 
    if (peek_character != -1) return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0, TCSANOW, &new_settings);
    if(nread == 1) 
    {
        peek_character = ch;
        return 1;
    }
    return 0;
}
 
int readch()
{
    char ch;
 
    if(peek_character != -1) 
    {
        ch = peek_character;
        peek_character = -1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}
 

