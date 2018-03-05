#include <SerialStream.h>
#include <string>
#include <unistd.h>
#include <cstdlib>
#include <stdio.h>
#include <ctime>



using namespace std;
using namespace LibSerial;
using namespace std;

#define HEAD1 16
#define	R_SHOULDER_1	23
#define	R_SHOULDER_2	19
#define	R_SHOULDER_3	20
#define	R_ELBOW			21
#define	R_LOWER			22
#define	L_SHOULDER_1	6
#define	L_SHOULDER_2	2
#define	L_SHOULDER_3	3
#define	L_ELBOW			4
#define	ALL				0xFE
#define TIMEOUT .05

// Macros
#define deg2rad(x) x*3.14159265359/180.0
#define rad2deg(x) x*180.0/3.14159265359
#define str2int(x) atoi( x.c_str() )





class Dynamixle
{
	private:
		unsigned char readByte();
		char next_char[1];
		SerialStream Serial;
		clock_t init;
		bool cond;
		void torqueEnable(int ID,bool state);
		void setSpeed(int ID,float speed);
		void setDelayTime(int ID,unsigned int usec);
		
	public:
		
		Dynamixle(string port,int baudrate);
		void LED(int ID,bool state);
		void writePosition(float angle1,float angle2,float angle3,float angle4,float angle5);
		void writePosition(int ID,float angle);
		float readPosition(int ID);
		void config(float speed,bool trqEnb);
			
};




