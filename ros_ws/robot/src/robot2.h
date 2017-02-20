/**** Projet long N7 2017 ****/

#ifndef ROBOT2
#define ROBOT2

#include "ros/ros.h"

//Services Vrep
#include <vrep_common/simRosSetJointState.h>
#include <vrep_common/simRosGetObjectHandle.h>
#include <vrep_common/simRosGetJointState.h>

//Messages
#include <robot/RobotJoints.h>
#include <std_msgs/Int32.h>

#include <vector>
#include <sstream>


class Robot2
{
private:

	//Service simRosGetObjectHandle
	ros::ServiceClient client_simRosGetHandle;
	vrep_common::simRosGetObjectHandle srv_simRosGetHandle;

	//Service simRosSetJointState
	ros::ServiceClient client_simRosSetJoint;
	vrep_common::simRosSetJointState srv_simRosSetJoint;

	//Service simRosGetJointState
	ros::ServiceClient client_simRosGetJoint;
	vrep_common::simRosGetJointState srv_simRosGetJoint;

	//Subscribers
	ros::Subscriber planifSendPosition;
	ros::Subscriber planifSendJoints;
	ros::Subscriber planifFermerPince;
	ros::Subscriber planifOuvrirPince;
	ros::Subscriber planifDescendreBras;
	ros::Subscriber planifMonterBras;
	
	//Publishers
	ros::Publisher pub_pince;
	ros::Publisher pub_retourCommande;

	//Variables
	float pi;
	unsigned char mymodes[7];
	float Rpos[7];
	int Rints[7];

public:

	Robot2();
	~Robot2();

	void init(ros::NodeHandle noeud);

	void SendPositionCallback(const std_msgs::Int32::ConstPtr& msg);
	void SendJointsCallback(const robot::RobotJoints::ConstPtr& msg);

	void FermerPinceCallback(const std_msgs::Int32::ConstPtr& msg);
	void OuvrirPinceCallback(const std_msgs::Int32::ConstPtr& msg);

	void DescendreBrasCallback(const std_msgs::Int32::ConstPtr& msg);
	void MonterBrasCallback(const std_msgs::Int32::ConstPtr& msg);

};


#endif

