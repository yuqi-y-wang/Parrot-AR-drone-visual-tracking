#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <stdlib.h>

#include "kcftracker.hpp"

#include <ros/ros.h>
#include <cv_bridge/cv_bridge.h>    
#include <image_transport/image_transport.h>
#include <sensor_msgs/image_encodings.h>

#include "KCF/BoundingBox.h"
#include "std_msgs/Bool.h"

using namespace std;
using namespace cv;

Mat frame;
Rect roi;
bool init_flag = false;
bool begintrack_flag = false;


cv::Rect init_rect;
cv::Point origin;
bool detected=false;
bool tracking=false;
bool select_flag=false;

//const std::string RECEIVE_IMG_TOPIC_NAME = "/camera/rgb/image";
const std::string RECEIVE_IMG_TOPIC_NAME = "/ardrone/front/image_raw";
const std::string PUBLISH_BOUNDINGBOX_NAME = "/KCF/boundingbox";

void onMouse(int event, int x, int y, int, void*)
{
	if (select_flag)
	{
		init_rect.x = MIN(origin.x, x);		
		init_rect.y = MIN(origin.y, y);
		init_rect.width = abs(x - origin.x);	
		init_rect.height = abs(y - origin.y);
		init_rect &= cv::Rect(0, 0, frame.cols, frame.rows);
	}
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		tracking = false;  
		select_flag = true;	
		origin = cv::Point(x, y);		
		init_rect = cv::Rect(x, y, 0, 0);	
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		select_flag = false;
		init_flag = true;
	}
}

//received image callback
void imageCallback(const sensor_msgs::ImageConstPtr& cam_image){
cv_bridge::CvImagePtr cv_ptr;
try
{
    cv_ptr = cv_bridge::toCvCopy(cam_image,sensor_msgs::image_encodings::BGR8);
}

catch (cv_bridge::Exception& e)
{
     ROS_ERROR("cv_bridge exception:%s",e.what());
     return;
}

    frame= cv_ptr->image;
	if(begintrack_flag == false){		
	    imshow("image",frame);
	    cvWaitKey(1);
		cv::setMouseCallback("image", onMouse, 0);
	}
}


int main(int argc, char* argv[]) 
{
    ros::init(argc, argv, "KCF_node");  
    ros::NodeHandle nh;   

    image_transport::ImageTransport it(nh);  
    image_transport::Subscriber sub = it.subscribe(RECEIVE_IMG_TOPIC_NAME, 1, imageCallback); 
	ros::Publisher boundingbox_pub  = nh.advertise<KCF::BoundingBox>(PUBLISH_BOUNDINGBOX_NAME, 1);


    // Published ROI 
    KCF::BoundingBox box;

    // ROI selector
    BoxExtractor ROIbox;

    bool HOG = true;
    bool FIXEDWINDOW = true;
    bool MULTISCALE = true;
    bool SILENT = true;
    bool LAB = true;
	int event;

     // Create KCFTracker object
    KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
     // Tracker results
    Rect result;

    ros::Rate loop_rate(50);  
    while (nh.ok()) 
    { 

           if ( init_flag )
           {
  	          // initialize the tracker
      	      tracker.init((Rect)init_rect, frame);

      	      printf("Start the tracking process!\n");
              init_flag = false;
      	      begintrack_flag = true;

           }

           if ( begintrack_flag )
           { 

                // stop the program if no more images
            	if (frame.rows == 0 || frame.cols == 0)
            	        break;

      	        // update the tracking result
      	        result = tracker.update(frame);


				// publish the boundingbox
                box.x = result.x;
                box.y = result.y;
                box.width = result.width;
                box.height = result.height;
				boundingbox_pub.publish( box );

      	       // draw the tracked object
      	       rectangle(frame, result, Scalar(255, 0, 0), 2, 1);

      	       // show image with the tracked object
      	       imshow("image", frame);


      	       //quit on ESC button
      	       if (waitKey(1) == 27) 
      	              return 0;
       }

        ros::spinOnce();  
        loop_rate.sleep(); 
    }
}
