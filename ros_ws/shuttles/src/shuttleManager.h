/**** Maxime Maurin - Projet Long N7 2017 ****/

#ifndef SHUTTLEMANAGER
#define SHUTTLEMANAGER

#include <ros/ros.h>
#include "shuttles/srvGetShuttleStatus.h"
#include "shuttles/srvGetEmptyShuttles.h"
#include "shuttle.h"
#include "string.h"
#include <std_msgs/Int32.h>
#include "shuttles/msgShuttleChange.h"
#include "shuttles/msgShuttleCreate.h"
#include <cstdlib>
#include <map>
#include <vector>


class ShuttleManager
{
private:

	std::map<int,Shuttle*> ShuttlesMap; //Collection "MAP" <key,Object*>, key = handle, Object* = pointer to shuttle object

	ros::ServiceServer serviceGetShuttleStatus;
	ros::ServiceServer serviceGetEmptyShuttles;

	ros::Subscriber subProductChange, subDestinationChange, subZoneChange, subAddShuttle, subDelShuttle;

public:
	
	ShuttleManager();
	~ShuttleManager();
	
	//Function to initialize subscribers and services
	void init(ros::NodeHandle nh);

	//Callback Function for Services
	bool getShuttleStatus(shuttles::srvGetShuttleStatus::Request &req, shuttles::srvGetShuttleStatus::Response &res);
	bool getEmptyShuttles(shuttles::srvGetEmptyShuttles::Request &req, shuttles::srvGetEmptyShuttles::Response &res);
	
	//Callback Function for Subscribers
	void productChangeCallBack(const shuttles::msgShuttleChange::ConstPtr& msg);
	void destinationChangeCallBack(const shuttles::msgShuttleChange::ConstPtr& msg);
	void zoneChangeCallBack(const shuttles::msgShuttleChange::ConstPtr& msg);
	void addShuttleCallBack(const shuttles::msgShuttleCreate::ConstPtr& msg);
	void delShuttleCallBack(const std_msgs::Int32::ConstPtr& msg);


};


#endif
