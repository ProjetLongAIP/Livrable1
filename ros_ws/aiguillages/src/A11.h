#ifndef AIG_11
#define AIG_11

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <unistd.h>
//#include "actionneurs.h"

#define taille_tab 3
using namespace std;


//aiguillage 1->2
class A11
{
private:

	//Subscribe Sensor
	ros::Subscriber VREPsubStopSensor;
	ros::Subscriber VREPsubRailSensor;
	ros::Subscriber VREPsubSwitchSensor;


	ros::Publisher SendShuttle;
	ros::Subscriber ReceiveShuttle;
	std::list<int> ListShuttles;

	

	ros::Publisher ShStop;
	ros::Publisher ShStart;
	ros::Publisher AigDev;
	ros::Publisher AigVer;
	ros::Publisher AigGauche;
	ros::Publisher AigDroit;

	bool PS;///Capteur stop 20 en entrée de l'aiguillage
	bool PS_past;

	bool CPD;//Capteur CP droit apres l'aiguillage
	bool CPD_past;

	bool CPG;
	bool CPG_past;

	bool Nav_CPD;///Une navette est passé en CP9
	bool Nav_CPG;

	bool Aig_D;//Capteurs aiguillage
	bool Aig_G;

	int tab_AI[taille_tab];
	int tab_att;//indice de la navette à traiter
	int tab_arr;//indice pour la navette qui arrive

	std_msgs::Int32 num_AIG;
	std_msgs::Int32 num_STOP;

	int num_aiguillage;
	int num_capt_stop;
	int num_capt_droit;
	int num_capt_gauche;

	int M;



public:
	A11(ros::NodeHandle nh);
	//void init(ros::NodeHandle nh);
	void StopSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void RailSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg);


	void Attente(int att);
	void Gauche();
	void Droit();
	void Aiguille_Navette();

	void newShuttleCallback (const std_msgs::Int32::ConstPtr& msg);

};

#endif
