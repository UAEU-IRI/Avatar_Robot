#include "ros/ros.h"
#include "Dynamixle.h"
#include <robot_state_publisher/robot_state_publisher.h>
#include <kdl_parser/kdl_parser.hpp>
#include "sensor_msgs/JointState.h"

#include <iostream>

int seq=0;


Dynamixle servo("/dev/ttyUSB0",57600);
KDL::Tree robotKDL;


sensor_msgs::JointState data;

sensor_msgs::JointState feedback_data;

//Subscribers callback functions---------------------------------------
void callBack(const sensor_msgs::JointState::ConstPtr& msg)
{
data=*msg;

for(int i=0;i< data.name.size();i++ ){

cout<< str2int(data.name[i]) <<"    "<< (data.position[i]) <<endl;
servo.writePosition(str2int(data.name[i]),data.position[i]);
}

}

float angle=0.0;
int main(int argc, char** argv)
{


//feedback_data.name.assign(m);
ros::init(argc, argv, "test_driver");
ros::NodeHandle nh;
ros::Rate rate(0.5);
std::string ns;
ns=ros::this_node::getName();




ros::Subscriber sub_joints= nh.subscribe(ns+"/joint_cmd", 100 ,callBack);	
float deg=2.40;
bool stat=true;
servo.torqueEnable(ALL,true);
while (ros::ok()){

servo.LED(R_SHOULDER_1,stat);
stat=!stat;
std::cout<<"LED Blinking"<<std::endl;
deg = deg + 0.05;
servo.writePosition(R_SHOULDER_1,deg);


ros::spinOnce();
rate.sleep();
}return 0;}
