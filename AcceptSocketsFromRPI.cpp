#include "WPILib.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <cstring>
#include <iostream>

using namespace std;

class RobotDemo : public SimpleRobot
{
	struct addrinfo* roboInfo;
	int roboSock;

public:
	RobotDemo(void)	// as they are declared above.
	{
		int status;
		struct addrinfo hints;
		//clear the hints struct
		memset(&hints, 0, sizeof(hints));
		
		//get the address information for the robot
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_flags = AI_PASSIVE;
		status = getaddrinfo(NULL, "1714", &hints, &roboInfo);
		if(status != 0)
		{
			DriverStationLCD::GetInstance()->Printf(DriverStationLCD::kUser_Line4, 1, "Error getting addr");
		}
		
		//Get the sockets for the robot and RPI
		roboSock = socket(roboInfo->ai_family, roboInfo->ai_socktype, roboInfo->ai_protocol);
		if(roboSock == -1)
		{
			DriverStationLCD::GetInstance()->Printf(DriverStationLCD::kUser_Line5, 1, "Error creating sock");
		}
		
		//bind the robot to the the port
		if(bind(roboSock, roboInfo->ai_addr, roboInfo->ai_addrlen))
		{
			DriverStationLCD::GetInstance()->Printf(DriverStationLCD::kUser_Line6, 1, "Error binding");
		}
	}

	void Autonomous(void)
	{
	}

	void OperatorControl(void)
	{
		GetWatchdog().SetEnabled(true);
		while (IsOperatorControl())
		{
			char msg[26];
			memset(msg, 0, sizeof(msg));
			
			GetWatchdog().Feed();
			//Get data from the RPI
			try{
				recvfrom(roboSock, msg, sizeof(msg), 0, NULL, NULL);
				throw 1;
			}
			catch(int i)
			{
				DriverStationLCD::GetInstance()->Printf(DriverStationLCD::kUser_Line6, 1, "oops");
			}
			if(msg[0] == NULL)
			{
				DriverStationLCD::GetInstance()->Printf(DriverStationLCD::kUser_Line1, 1, "No packet received");
			}
			else
			{
				DriverStationLCD::GetInstance()->Printf(DriverStationLCD::kUser_Line1, 1, msg);
			}
			DriverStationLCD::GetInstance()->Printf(DriverStationLCD::kUser_Line2, 1, "HELLO");
			DriverStationLCD::GetInstance()->UpdateLCD();
		}
	}
	
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

