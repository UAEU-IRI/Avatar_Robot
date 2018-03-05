#include "Dynamixle.h"
#include <SerialStream.h>
#include <iostream>

using namespace LibSerial ;





Dynamixle::Dynamixle(string port,int baudrate){
		Serial.Open(port);
				switch(baudrate) {
					case 50 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_50 );
						break;
					case 75 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_75 );
						break;
					case 110 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_110 );
						break;
					case 134 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_134 );
						break;
					case 150 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_150 );
						break;
					case 200 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_200 );
						break;
					case 300 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_300 );
						break;
					case 600 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_600 );
						break;
					case 1200 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_1200 );
						break;
					case 1800 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_1800 );
						break;
					case 2400 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_2400 );
						break;
					case 4800 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_4800 );
						break;
					case 9600 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_9600 );
						break;
					case 19200 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_19200 );
						break;
					case 38400 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_38400 );
						break;
					case 57600 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_57600 );
						break;
					case 115200 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_115200 );
						break;
					case 230400 :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_230400 );
						break;
					 default :
						Serial.SetBaudRate( SerialStreamBuf::BAUD_9600 );}
		
		
		}
		
unsigned char Dynamixle::readByte(){
cond=false;
init=clock();

while((float(clock()-init)/CLOCKS_PER_SEC)<TIMEOUT)
{
if (Serial.rdbuf()->in_avail()>0){cond=true; break;} 
}

if (cond==true){Serial.readsome(next_char,1); return next_char[0];}
else{throw -1;}

}


void Dynamixle::setDelayTime(int ID,unsigned int usec){
			char val=usec*0.5;

			unsigned char checksum=~(ID+4+3+5+val);
			char output_buffer[8] ={0xFF,0xFF,ID,4,3,5,val,checksum};
			Serial.write( output_buffer,8);
			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){}}
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer			
			}//end setDelayTime
	
void Dynamixle::setSpeed(int ID,float speed){
			int speedint=int(speed*1023.0/100.);
			char speed_L=speedint;
			char speed_H=(speedint>>8);
			unsigned char checksum=~(ID+5+0x03+0x20+speed_L+speed_H);
			char output_buffer[9] ={0xFF,0xFF,ID,5,0x03,0x20,speed_L,speed_H,checksum};
			Serial.write( output_buffer,9);
			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){}}
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer
			}//end setSpeed

void Dynamixle::config(float speed,bool trqEnb){
			setDelayTime(ALL,0);
			setSpeed(ALL,speed);
			torqueEnable(ALL, trqEnb);
			}//end config

	
void Dynamixle::LED(int ID,bool state){
			unsigned char checksum=~(ID+4+3+25+state);
			char output_buffer[8] ={0xFF,0xFF,ID,4,3,25,state,checksum};
			Serial.write( output_buffer,8);
			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){}}
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer
			}//end LED

void Dynamixle::torqueEnable(int ID,bool state){
			unsigned char checksum=~(ID+4+3+24+state);
			char output_buffer[8] ={0xFF,0xFF,ID,4,3,24,state,checksum};
			Serial.write( output_buffer,8);
			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){}}
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer	
			}//end torqueEnable


float Dynamixle::readPosition(int ID){
			unsigned char data[2];
			float angle;
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer
			unsigned char checksum=~(ID+4+2+0x24+2);
			char output_buffer[8] ={0xFF,0xFF,ID,4,2,0x24,2,checksum};
			Serial.write( output_buffer,8);
			
			unsigned char _ID,length,error,_checksum,temp;
			try{Dynamixle::readByte();} catch(int msg){std::cout<< "ID: " << ID <<std::endl; throw "Connection timeout";} 
			
			try{temp=Dynamixle::readByte();} catch(int msg){std::cout<< "ID: " << ID <<std::endl; throw "Connection timeout";} 
			
			if(temp==255){
			try{_ID=Dynamixle::readByte();}catch(int msg){std::cout<< "ID: " << ID <<std::endl; throw "Connection timeout";} 
			try{length=Dynamixle::readByte();}catch(int msg){std::cout<< "ID: " << ID <<std::endl; throw "Connection timeout";} 
			if (_ID==ID){
			try{error=Dynamixle::readByte();}catch(int msg){std::cout<< "ID: " << ID <<std::endl; throw "Connection timeout";} 
			for(int i=0;i<(length-2);i++){data[i]=Dynamixle::readByte();}
			try{_checksum=Dynamixle::readByte();}catch(int msg){std::cout<< "ID: " << ID <<std::endl; throw "Connection timeout";} 
			unsigned char __checksum=~(_ID+length+error+data[0]+data[1]);
			if(__checksum==_checksum && error==0){
			angle=((data[1]<<8)+data[0])*360.0/4095.0;
			} else{throw "invalid message";}
			} else{throw "invalid message";}
			} else{throw "invalid message";}
			
			
			while (Serial.rdbuf()->in_avail()){Serial.get();}
			return angle;
			
			}
			
void Dynamixle::writePosition(float angle1,float angle2,float angle3,float angle4,float angle5){
			int value1=int(rad2deg(angle1)/0.088);  //11.375 is the conversion from angles to register value. for example; 1023 represents 300 degrees, 0 is 0 degree...
			int value2=int(rad2deg(angle2)/0.088);
			int value3=int(rad2deg(angle3)/0.088);
			int value4=int(rad2deg(angle4)/0.088);
			int value5=int(rad2deg(angle5)/0.088);
			                                              // Float Angle (H + A + B + L)
			unsigned char angle1_L=value1;
			unsigned char angle2_L=value2;
			unsigned char angle3_L=value3;
			unsigned char angle4_L=value4;
			unsigned char angle5_L=value5;

			unsigned char angle1_H=(value1>>8);
			unsigned char angle2_H=(value2>>8);
			unsigned char angle3_H=(value3>>8);
			unsigned char angle4_H=(value4>>8);
			unsigned char angle5_H=(value5>>8);
			unsigned char checksum = ~(ALL + 29 + 0x83 + 116 + 2 + R_SHOULDER_1 + angle1_L + angle1_H
									      + R_SHOULDER_2 + angle2_L + angle2_H
									      + R_SHOULDER_3 + angle3_L + angle3_H
									      + R_ELBOW      + angle4_L + angle4_H
									      + R_LOWER      + angle5_L + angle5_H);
			
			
			
			
			~(ALL+29+0x83+ 116 + 2  +R_SHOULDER_1+angle1_L+angle1_H+R_SHOULDER_2+angle2_L+angle2_H+R_ELBOW+angle4_L+angle4_H+R_LOWER+angle5_L+angle5_H);
			char output_buffer[33] ={0xFF,0xFF,ALL,29,0x83, 116 , 4 ,R_SHOULDER_1 , angle1_L , angle1_H, 0x00 , 0x00 ,
										 R_SHOULDER_2 , angle2_L , angle2_H, 0x00 , 0x00 ,
										 R_SHOULDER_3 , angle3_L , angle3_H, 0x00 , 0x00 ,
										 R_ELBOW , angle4_L , angle4_H, 0x00 , 0x00 ,
										 R_LOWER , angle5_L , angle5_H , 0x00 , 0x00 , checksum};
			Serial.write( output_buffer,33);
			}//end writePosition





void Dynamixle::writePosition(int ID,float angle){
			int value=int(rad2deg(angle)*11.375);  //11.375 is the conversion from angles to register value. for example; 1023 represents 300 degrees, 0 is 0 degree...

			char angle_L=value;
			char angle_H=(value>>8);
	
			unsigned char checksum=~(ID+0x05+0x03+ 116 +angle_L+angle_H);
			char output_buffer[11] ={0xFF,0xFF,ID,0x07,0x03, 116 ,angle_L,angle_H,0x00, 0x00,checksum};
			Serial.write( output_buffer,9);
			
			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){std::cout<<"here"<<std::endl;}}
			while (Serial.rdbuf()->in_avail()){try{readByte();}catch(int msg){std::cout<< "ID: " << ID <<std::endl; throw "Connection timeout";}} //clears input buffer
			}
			
	









