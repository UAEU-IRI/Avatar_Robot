#include "ros/ros.h"
#include "Dynamixle.h"
#include <robot_state_publisher/robot_state_publisher.h>
#include <kdl_parser/kdl_parser.hpp>
#include "sensor_msgs/JointState.h"

#include <iostream>
using namespace std;



Dynamixle servo("/dev/ttyUSB0",57600);


int main(int argc, char** argv)
{

//feedback_data.name.assign(m);
ros::init(argc, argv, "driver");
ros::NodeHandle nh;
ros::Rate rate(100);



servo.config(30.0,false);

float angs[5]={0.0,0.0,0.0,0.0,0.0};

servo.LED(ALL,false);
while (ros::ok()){



try {servo.readPosition(angs);}		catch (const char* msg){std::cerr << msg << std::endl;}


cout << angs[0] << "    "<< angs[1] << "    "<< angs[2] << "    "<< angs[3] << "    "<< angs[4] << "    " << endl;


ros::spinOnce();
rate.sleep();
}return 0;}
