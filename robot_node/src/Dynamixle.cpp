#include "Dynamixle.h"
#include <SerialStream.h>
#include <iostream>

using namespace LibSerial ;

Dynamixle::Dynamixle(string port,int baudrate){
		accLimit=12271.25;
		velLimit=12271.25;
		Serial.Open(port);
				switch(baudrate) {
					case 50     : Serial.SetBaudRate( SerialStreamBuf::BAUD_50    );break;
					case 75     : Serial.SetBaudRate( SerialStreamBuf::BAUD_75    );break;
					case 110    : Serial.SetBaudRate( SerialStreamBuf::BAUD_110   );break;
					case 134    : Serial.SetBaudRate( SerialStreamBuf::BAUD_134   );break;
					case 150    : Serial.SetBaudRate( SerialStreamBuf::BAUD_150   );break;
					case 200    : Serial.SetBaudRate( SerialStreamBuf::BAUD_200   );break;
					case 300    : Serial.SetBaudRate( SerialStreamBuf::BAUD_300   );break;
					case 600    : Serial.SetBaudRate( SerialStreamBuf::BAUD_600   );break;
					case 1200   : Serial.SetBaudRate( SerialStreamBuf::BAUD_1200  );break;
					case 1800   : Serial.SetBaudRate( SerialStreamBuf::BAUD_1800  );break;
					case 2400   : Serial.SetBaudRate( SerialStreamBuf::BAUD_2400  );break;
					case 4800   : Serial.SetBaudRate( SerialStreamBuf::BAUD_4800  );break;
					case 9600   : Serial.SetBaudRate( SerialStreamBuf::BAUD_9600  );break;
					case 19200  : Serial.SetBaudRate( SerialStreamBuf::BAUD_19200 );break;
					case 38400  : Serial.SetBaudRate( SerialStreamBuf::BAUD_38400 );break;
					case 57600  : Serial.SetBaudRate( SerialStreamBuf::BAUD_57600 );break;
					case 115200 : Serial.SetBaudRate( SerialStreamBuf::BAUD_115200);break;
					case 230400 : Serial.SetBaudRate( SerialStreamBuf::BAUD_230400);break;
					default     : Serial.SetBaudRate( SerialStreamBuf::BAUD_9600  );}		
                           		}   //End of switch(baudrate)
		
unsigned char Dynamixle::readByte(){
            cond=false;
            init=clock();
            
            while((float(clock()-init)/CLOCKS_PER_SEC)<TIMEOUT)
           {
              if (Serial.rdbuf()->in_avail()>0){cond=true; break;} 
           }

              if (cond==true){Serial.readsome(next_char,1); return next_char[0];}
                else{throw -1;}
                                   } //End of Dynamixle::readByte
                                   
                                   
float Dynamixle::bytes2float(unsigned char b1,unsigned char b2,unsigned char b3,unsigned char b4){
Dynamixle::u.l=b1+(b2<<8)+(b3<<16)+(b4<<24);
return Dynamixle::u.f;
}
//-------------------------------------------------------------------------------------------------------------------------------------						
unsigned short Dynamixle::update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size)
{
        unsigned short i, j;
        unsigned short crc_table[256] = 
       {0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
        0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
        0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
        0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
        0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
        0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
        0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
        0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
        0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
        0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
        0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
        0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
        0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
        0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
        0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
        0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202};
    for(j = 0; j < data_blk_size; j++)
    {
        i = ((unsigned short)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
        crc_accum = (crc_accum << 8) ^ crc_table[i];
    }
    return crc_accum;
    }
//-------------------------------------------------------------------------------------------------------------------------------------

void Dynamixle::setDelayTime(int ID,unsigned int usec){    //Set Delay Time Protocol (2.0)
			char val=usec*0.5; // Val from 2uSec to 500uSec even numbers
			unsigned char packet[11]= {0xFF,0xFF,0xFD,0x00,ID,0x06,0x00,0x03,9,0,val};
			unsigned short crc = Dynamixle::update_crc(0,packet,11);
			unsigned char crc_L = crc;
			unsigned char crc_H = (crc>>8);
			char output_buffer[13] ={0xFF,0xFF,0xFD,0x00,ID,0x06,0x00,0x03,9,0,val,crc_L,crc_H};
			Serial.write( output_buffer,13);
			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){}}
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer			
			}//end setDelayTime

void Dynamixle::setAccLimit(int ID,float accL){    //Set Accerlation Limit Protocol (2.0)
			long msg=(accL*1);
			char msg_L=msg;
			char msg_1=(msg>>8);
			char msg_2=(msg>>16);
			char msg_H=(msg>>24);
			unsigned char packet[14]= {0xFF,0xFF,0xFD,0x00,ID,9,0x00,0x03,40,0,msg_L,msg_1,msg_2,msg_H};
			unsigned short crc = Dynamixle::update_crc(0,packet,14);
			unsigned char crc_L = crc;
			unsigned char crc_H = (crc>>8);
			char output_buffer[16] ={0xFF,0xFF,0xFD,0x00,ID,9,0x00,0x03,40,0,msg_L,msg_1,msg_2,msg_H,crc_L,crc_H};
			Serial.write(output_buffer,16);

			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){}}
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer
			}  //end Set Accerlation Limit	

void Dynamixle::setAcc(int ID,float acc){    //Set Accerlation Protocol (2.0)
			long msg=(acc*1);
			char msg_L=msg;
			char msg_1=(msg>>8);
			char msg_2=(msg>>16);
			char msg_H=(msg>>24);
			unsigned char packet[14]= {0xFF,0xFF,0xFD,0x00,ID,9,0x00,0x03,108,0,msg_L,msg_1,msg_2,msg_H};
			unsigned short crc = Dynamixle::update_crc(0,packet,14);
			unsigned char crc_L = crc;
			unsigned char crc_H = (crc>>8);
			char output_buffer[16] ={0xFF,0xFF,0xFD,0x00,ID,9,0x00,0x03,108,0,msg_L,msg_1,msg_2,msg_H,crc_L,crc_H};
			Serial.write(output_buffer,16);

			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){}}
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer
			}  //end Set Accerlation			

void Dynamixle::setVelLimit(int ID,float velL){    //Set Velocity Limit Protocol (2.0)
			long msg=(velL*1);
			char msg_L=msg;
			char msg_1=(msg>>8);
			char msg_2=(msg>>16);
			char msg_H=(msg>>24);
			unsigned char packet[14]= {0xFF,0xFF,0xFD,0x00,ID,9,0x00,0x03,44,0,msg_L,msg_1,msg_2,msg_H};
			unsigned short crc = Dynamixle::update_crc(0,packet,14);
			unsigned char crc_L = crc;
			unsigned char crc_H = (crc>>8);
			char output_buffer[16] ={0xFF,0xFF,0xFD,0x00,ID,9,0x00,0x03,44,0,msg_L,msg_1,msg_2,msg_H,crc_L,crc_H};
			Serial.write(output_buffer,16);

			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){}}
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer
			}  //end Set Velocity Limit

void Dynamixle::setVel(int ID,float vel){    //Set Velocity Protocol (2.0)
			long msg=(vel*1);
			char msg_L=msg;
			char msg_1=(msg>>8);
			char msg_2=(msg>>16);
			char msg_H=(msg>>24);
			unsigned char packet[14]= {0xFF,0xFF,0xFD,0x00,ID,9,0x00,0x03,112,0,msg_L,msg_1,msg_2,msg_H};
			unsigned short crc = Dynamixle::update_crc(0,packet,14);
			unsigned char crc_L = crc;
			unsigned char crc_H = (crc>>8);
			char output_buffer[16] ={0xFF,0xFF,0xFD,0x00,ID,9,0x00,0x03,112,0,msg_L,msg_1,msg_2,msg_H,crc_L,crc_H};
			Serial.write(output_buffer,16);

			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){}}
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer
			}  //end Set Velocity

void Dynamixle::config(float vel,bool trqEnb){    //Configuration for Servos Protocol (2.0)
			setDelayTime(ALL,10);
			setAccLimit(ALL, 100);
			setVelLimit(ALL, 500);
			setAcc(ALL, 0);
			setVel(ALL, 100);
			torqueEnable(ALL, trqEnb);
			} //End of config

void Dynamixle::torqueEnable(int ID,bool state){    //Set Torque Status Protocol (2.0)
			unsigned char packet[11]= {0xFF,0xFF,0xFD,0x00,ID,0x06,0x00,0x03,64,0,state};
			unsigned short crc = Dynamixle::update_crc(0,packet,11);
			unsigned char crc_L = crc;
			unsigned char crc_H = (crc>>8);
			char output_buffer[13] ={0xFF,0xFF,0xFD,0x00,ID,0x06,0x00,0x03,64,0,state,crc_L,crc_H};
			Serial.write( output_buffer,13);
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
			

void Dynamixle::readPosition(float *angles_array){
			//angles_array is a float array storing the following angles with the same order:
			//R_SHOULDER_1 , R_SHOULDER_2 , R_SHOULDER_3 , R_ELBOW , R_LOWER
			
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer
			
			unsigned char packet[17]= {0xFF,0xFF,0xFD,0x00,0xFE,12,0,0x82,132,0,4,0,
									   R_SHOULDER_1 , R_SHOULDER_2 , R_SHOULDER_3 , R_ELBOW , R_LOWER };
			
			unsigned short crc = Dynamixle::update_crc(0,packet,17);
			unsigned char crc_L = crc;
			unsigned char crc_H = (crc>>8);	
			char output_buffer[19] = {0xFF,0xFF,0xFD,0x00,0xFE,12,0,0x82,132,0,4,0,
									   R_SHOULDER_1 , R_SHOULDER_2 , R_SHOULDER_3 , R_ELBOW , R_LOWER,crc_L,crc_H };
			Serial.write( output_buffer,19);
			
			unsigned char pos_1,pos_2,pos_3,pos_4,_framing1,_framing2,_framing3;
			
			float _angle;
			
			unsigned char _packet[15];
			_packet[0]=0xFF;
			_packet[1]=0xFF;
			_packet[2]=0xFD;
			_packet[3]=0x00;
			
			
			for( int j=1;j<=5;j++){
			
			
			try{_framing1=Dynamixle::readByte();} catch(int msg){throw "Connection timeout";}
			try{_framing2=Dynamixle::readByte();} catch(int msg){throw "Connection timeout";}
			try{_framing3=Dynamixle::readByte();} catch(int msg){throw "Connection timeout";}
			try{Dynamixle::readByte();} catch(int msg){throw "Connection timeout";}
			
			if(_framing1==0xFF && _framing2==0xFF && _framing3==0xFD){
			
			
			for (int i=4;i<15;i++){
			try{_packet[i]=Dynamixle::readByte();} catch(int msg){throw "Connection timeout";}
			 }//for loop
			 unsigned short _crc = Dynamixle::update_crc(0,_packet,13);
			 unsigned short _msg_crc=_packet[13]+(_packet[14]<<8);
			 
			 
			 if(_crc==_msg_crc && _packet[8]==0){
			 				
			 				_angle=float((_packet[10]<<8)+_packet[9])/11.375;
			 				
							switch(_packet[4]) {

						   case R_SHOULDER_1:
							  angles_array[0]=_angle;
							  break; 

						   case R_SHOULDER_2:
							  angles_array[1]=_angle;
							  break; 

						   case R_SHOULDER_3:
							  angles_array[2]=_angle;
							  break; 
							  
						   case R_ELBOW:
							  angles_array[3]=_angle;
							  break; 
							  
							  
						   case R_LOWER:
							  angles_array[4]=_angle;
							  break; 							  							  						  

							}//switch
			 }//if(_crc ..			 
			 }//if(_framing1==0xFF &..
			 }//for loop five times 
		
									   
}//funcion end
			
void Dynamixle::writePosition(float angle1,float angle2,float angle3,float angle4,float angle5){    //Write Position fpr ALL Protocol (2.0)
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
			
			unsigned char packet[37]= {0xFF,0xFF,0xFD,0x00,0xFE,32,0x00,0x83,116,0,4,0,
									   R_SHOULDER_1 , angle1_L , angle1_H, 0x00 , 0x00 ,
									   R_SHOULDER_2 , angle2_L , angle2_H, 0x00 , 0x00 ,
									   R_SHOULDER_3 , angle3_L , angle3_H, 0x00 , 0x00 ,
									        R_ELBOW , angle4_L , angle4_H, 0x00 , 0x00 ,
									        R_LOWER , angle5_L , angle5_H, 0x00 , 0x00};
									        
			unsigned short crc = Dynamixle::update_crc(0,packet,37);
			unsigned char crc_L = crc;
			unsigned char crc_H = (crc>>8);
			char output_buffer[39] ={0xFF,0xFF,0xFD,0x00,0xFE,32,0x00,0x83,116,0,4,0,
									   R_SHOULDER_1 , angle1_L , angle1_H, 0x00 , 0x00 ,
									   R_SHOULDER_2 , angle2_L , angle2_H, 0x00 , 0x00 ,
									   R_SHOULDER_3 , angle3_L , angle3_H, 0x00 , 0x00 ,
									        R_ELBOW , angle4_L , angle4_H, 0x00 , 0x00 ,
									        R_LOWER , angle5_L , angle5_H, 0x00 , 0x00 , crc_L,crc_H};
			Serial.write( output_buffer,39);			
			} //end writePosition

void Dynamixle::writePosition(int ID,float angle){    //Write Position Protocol (2.0)
			int value=int(rad2deg(angle)/0.088);  //0.088 is to Convert 360 Degree to 4090 Value
			char angle_L=value;
			char angle_H=(value>>8);
			unsigned char packet[14]= {0xFF,0xFF,0xFD,0x00,ID,9,0x00,0x03,116,0,angle_L,angle_H,0x00,0x00};
			unsigned short crc = Dynamixle::update_crc(0,packet,14);
			unsigned char crc_L = crc;
			unsigned char crc_H = (crc>>8);			
			char output_buffer[16] ={0xFF,0xFF,0xFD,0x00,ID,9,0x00,0x03,116,0,angle_L,angle_H,0x00,0x00,crc_L,crc_H};
			Serial.write(output_buffer,16);
			
			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){std::cout<<"here"<<std::endl;}}
			while (Serial.rdbuf()->in_avail()){try{std::cout<<Serial.get()<<std::endl;}catch(int msg){std::cout<< "ID: " << ID <<std::endl; throw "Connection timeout";}} //clears input buffer    //readByte replaced with Serial.get()
			}
			
void Dynamixle::LED(int ID,bool state){    //Protocol (2.0)
			unsigned char packet[11]= {0xFF,0xFF,0xFD,0x00,ID,0x06,0x00,0x03,65,0,state};
			unsigned short crc = Dynamixle::update_crc(0,packet,11);
			unsigned char crc_L = crc;
			unsigned char crc_H = (crc>>8);
			char output_buffer[13] ={0xFF,0xFF,0xFD,0x00,ID,0x06,0x00,0x03,65,0,state,crc_L,crc_H};
			Serial.write( output_buffer,13);
			if (ID !=ALL){
			while (Serial.rdbuf()->in_avail()<1){}}
			while (Serial.rdbuf()->in_avail()){Serial.get();} //clears input buffer
			}//end LED

