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
}

float angls[5];

int main(int argc, char** argv)
{

std::string robot_desc_string;
feedback_data.header.frame_id="base_link";

feedback_data.name.push_back("neck_base_shoulder_right");
feedback_data.name.push_back("shoulder_shoulder_right");
feedback_data.name.push_back("shoulder_upperArm_right");
feedback_data.name.push_back("upperArm_midArm_right");
feedback_data.name.push_back("neck_base_shoulder_left");
feedback_data.name.push_back("shoulder_shoulder_left");
feedback_data.name.push_back("shoulder_upperArm_left");
feedback_data.name.push_back("upperArm_midArm_left");




//feedback_data.name.assign(m);
ros::init(argc, argv, "driver");
ros::NodeHandle nh;
ros::Rate rate(100);
std::string ns;
ns=ros::this_node::getName();
nh.param("robot_description", robot_desc_string, std::string());

if (!kdl_parser::treeFromString(robot_desc_string, robotKDL)){
  ROS_ERROR("Failed to construct kdl tree");
}



ros::Subscriber sub_joints= nh.subscribe(ns+"/joint_cmd", 1 ,callBack);	

robot_state_publisher::RobotStatePublisher rsp(robotKDL);
ros::Publisher pub = nh.advertise<sensor_msgs::JointState>(ns+"/jointState", 10);

std::map<std::string, double> joint_positions;
joint_positions.insert(std::make_pair("neck_base_shoulder_left", 0.0));
joint_positions.insert(std::make_pair("shoulder_shoulder_left", 0.0));
joint_positions.insert(std::make_pair("shoulder_upperArm_left", 0.0));
joint_positions.insert(std::make_pair("upperArm_midArm_left", 0.0));
joint_positions.insert(std::make_pair("midArm_lowerArm_left", 0.0));
joint_positions.insert(std::make_pair("neck_base_shoulder_right", 0.0));
joint_positions.insert(std::make_pair("shoulder_shoulder_right", 0.0));
joint_positions.insert(std::make_pair("shoulder_upperArm_right", 0.0));
joint_positions.insert(std::make_pair("upperArm_midArm_right", 0.0));
joint_positions.insert(std::make_pair("midArm_lowerArm_right", 0.0));

float neck_base_shoulder_right;
float shoulder_shoulder_right;
float shoulder_upperArm_right;
float upperArm_midArm_right;

float neck_base_shoulder_left;
float shoulder_shoulder_left;
float shoulder_upperArm_left;
float upperArm_midArm_left;

servo.config(30.0,true);

while (ros::ok()){


//read joint angles from motor
/*
try {neck_base_shoulder_right=servo.readPosition(R_SHOULDER_1);}
catch (const char* msg){std::cerr << msg << std::endl;}

try {shoulder_shoulder_right=servo.readPosition(R_SHOULDER_2);}
catch (const char* msg){std::cerr << msg << std::endl;}

try {shoulder_upperArm_right=servo.readPosition(R_SHOULDER_3);}
catch (const char* msg){std::cerr << msg << std::endl;}

try {upperArm_midArm_right=servo.readPosition(R_ELBOW);}
catch (const char* msg){std::cerr << msg << std::endl;}


try {neck_base_shoulder_left=servo.readPosition(L_SHOULDER_1);}
catch (const char* msg){std::cerr << msg << std::endl;}


try {shoulder_shoulder_left=servo.readPosition(L_SHOULDER_2);}
catch (const char* msg){std::cerr << msg << std::endl;}


try {shoulder_upperArm_left=servo.readPosition(L_SHOULDER_3);}
catch (const char* msg){std::cerr << msg << std::endl;}


try {upperArm_midArm_left=servo.readPosition(L_ELBOW);}
catch (const char* msg){std::cerr << msg << std::endl;}
*/

//robot state publisher 
joint_positions["neck_base_shoulder_right"]=deg2rad(neck_base_shoulder_right);
joint_positions["shoulder_shoulder_right"]=deg2rad(shoulder_shoulder_right);
joint_positions["shoulder_upperArm_right"]=deg2rad(shoulder_upperArm_right);
joint_positions["upperArm_midArm_right"]=deg2rad(upperArm_midArm_right);	
joint_positions["neck_base_shoulder_left"]=deg2rad(neck_base_shoulder_left);
joint_positions["shoulder_shoulder_left"]=deg2rad(shoulder_shoulder_left);
joint_positions["shoulder_upperArm_left"]=deg2rad(shoulder_upperArm_left);
joint_positions["upperArm_midArm_left"]=deg2rad(upperArm_midArm_left);	
rsp.publishTransforms(joint_positions,ros::Time::now(),"");


//publish joint angles feedback	
feedback_data.header.seq=++seq;
feedback_data.header.stamp=ros::Time::now();
feedback_data.position.clear();
feedback_data.position.push_back(deg2rad(neck_base_shoulder_right));
feedback_data.position.push_back(deg2rad(shoulder_shoulder_right));
feedback_data.position.push_back(deg2rad(shoulder_upperArm_right));
feedback_data.position.push_back(deg2rad(upperArm_midArm_right));
feedback_data.position.push_back(deg2rad(neck_base_shoulder_left));
feedback_data.position.push_back(deg2rad(shoulder_shoulder_left));
feedback_data.position.push_back(deg2rad(shoulder_upperArm_left));
feedback_data.position.push_back(deg2rad(upperArm_midArm_left));
pub.publish(feedback_data);

//check if joint commands are received
if(data.header.seq>0){
std::cout<< data.position[0] << std::endl;
servo.writePosition(data.position[0],data.position[1],data.position[2],data.position[3],data.position[4]);
}//end if

ros::spinOnce();
rate.sleep();
}return 0;}
