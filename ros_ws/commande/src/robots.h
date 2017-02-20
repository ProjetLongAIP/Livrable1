/**** Projet long N7 2017 ****/

#ifndef ROBOTS
#define ROBOTS

#include <ros/ros.h>
#include <iostream>

//Messages
#include <robot/RobotJoints.h>
#include <std_msgs/Int32.h>


class Robots
{
private:
	//Publishers
	ros::Publisher pub_robot_position1, pub_robot_position2, pub_robot_position3, pub_robot_position4;
	ros::Publisher pub_robot_joints1, pub_robot_joints2, pub_robot_joints3, pub_robot_joints4;
	ros::Publisher pub_pince_fermer1, pub_pince_fermer2, pub_pince_fermer3, pub_pince_fermer4;
	ros::Publisher pub_pince_ouvrir1, pub_pince_ouvrir2, pub_pince_ouvrir3, pub_pince_ouvrir4;
	ros::Publisher pub_pince_descendre1, pub_pince_descendre2, pub_pince_descendre3, pub_pince_descendre4;
	ros::Publisher pub_descendre1, pub_descendre2, pub_descendre3, pub_descendre4;
	ros::Publisher pub_monter1, pub_monter2, pub_monter3, pub_monter4;

	//Subscribers
	ros::Subscriber sub_retourRobot1, sub_retourRobot2, sub_retourRobot3, sub_retourRobot4;

	//Variables : retours des robots
	int robotInit[4];
	int robotPosition[4];
	int robotBras[4];

public:
	Robots(ros::NodeHandle noeud);
	~Robots();
	
	//Contrôles des robots
	void EnvoyerPosition(int numRobot, int numPosition);
	void EnvoyerAngles(int numRobot, int angle1, int angle2, int angle3, int angle4, int angle5, int angle6, int angle7);

	void FermerPince(int numRobot);
	void OuvrirPince(int numRobot);

	void DescendreBras(int numRobot);
	void MonterBras(int numRobot);

	//Retour depuis les nodes des robots
	void RetourRobot1Callback(const std_msgs::Int32::ConstPtr& msg);
	void RetourRobot2Callback(const std_msgs::Int32::ConstPtr& msg);
	void RetourRobot3Callback(const std_msgs::Int32::ConstPtr& msg);
	void RetourRobot4Callback(const std_msgs::Int32::ConstPtr& msg);

	int RobotInitialise(int numRobot); //retourne 1 si le robot numRobot est initialisé
	int RobotEnPosition(int numRobot); //retourne 1 si le robot numRobot est en position
	int BrasEnPosition(int numRobot);  //retourne 1 si le bras du robot numRobot est monté, -1 si le bras du robot numRobot est descendu

};


#endif
