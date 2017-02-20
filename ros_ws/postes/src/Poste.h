#ifndef POSTE
#define POSTE

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <unistd.h>
#include <list>


#include <vector>
#include <shuttles/srvGetShuttleStatus.h>
#include <shuttles/srvGetEmptyShuttles.h>
#include <shuttles/msgShuttleChange.h>
#include <shuttles/msgShuttleCreate.h>

//#include "actionneurs.h"


using namespace std;


class Poste
{
private:

	//Subscribe Sensor
	ros::Subscriber VREPsubStopSensor;
	ros::Subscriber VREPsubRailSensor;

	// Publisher and suscriber for the shuttle recognition (communication with the switchpoints)
	ros::Publisher SendShuttle;
	ros::Subscriber ReceiveShuttle;

	// Stop and start the shuttles
	ros::Publisher ShStop;
	ros::Publisher ShStart;
	
	// To communicate with the shuttles' services
	ros::ServiceClient clientGetShuttleStatus;
	shuttles::srvGetShuttleStatus srvGSS;

	std::list<int> ListShuttles;

// Variables for the sensors
	bool PS1;	//Capteur stop 21 
	bool PS2;	//Capteur stop 22 
	bool PS_out;	//Capteur stop 24 
	bool PS_past;

	bool CP_in;
	bool CP_in_past;

	bool Nav_CP_in;///Une navette est passé en CP9

	int M;



public:
	Poste(ros::NodeHandle nh);
	//void init(ros::NodeHandle nh);
	void StopSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void RailSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void newShuttleCallback (const std_msgs::Int32::ConstPtr& msg);
};

#endif
