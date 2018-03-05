cmake_minimum_required(VERSION 2.8.3)
project(avatar_robot)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  robot_state_publisher
  kdl_parser
)

catkin_package()

find_package(Boost REQUIRED COMPONENTS system)

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(driver src/driver.cpp src/Dynamixle.cpp)
target_link_libraries(driver
   ${catkin_LIBRARIES}
   serial
   pthread

 )
add_executable(test_driver src/test.cpp src/Dynamixle.cpp)
target_link_libraries(test_driver
   ${catkin_LIBRARIES}
   serial
   pthread

 )

