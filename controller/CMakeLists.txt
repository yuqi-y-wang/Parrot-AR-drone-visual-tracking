cmake_minimum_required(VERSION 2.8.3)
project(controller)


## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS
   roscpp
   rospy
   std_msgs
   genmsg
   message_generation
   sensor_msgs
)


## Declare ROS messages and services
add_message_files (DIRECTORY msg FILES BoundingBox.msg )
## Generate added messages and services

generate_messages(
  DEPENDENCIES
  std_msgs
  sensor_msgs
) 

## Declare a catkin package

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES controller
  DEPENDS system_lib
)

include_directories(include ${catkin_INCLUDE_DIRS})


add_executable(control_node src/control.cpp)
target_link_libraries(control_node ${catkin_LIBRARIES} ${OpenCV_LIBS})
add_dependencies(control_node KCF_generate_messages_cpp )

  








