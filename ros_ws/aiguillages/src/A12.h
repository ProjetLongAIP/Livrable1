#ifndef AIG_12
#define AIG_12

#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <unistd.h>
//#include "actionneurs.h"

#define taille_tab 4
using namespace std;

class A12
{
private:

	//Subscribe Sensor
	ros::Subscriber VREPsubSensor;
	ros::Subscriber VREPsubSwitchSensor;


	ros::Publisher ShStop;
	ros::Publisher ShStart;
	ros::Publisher AigDev;
	ros::Publisher AigVer;
	ros::Publisher AigGauche;
	ros::Publisher AigDroit; 


	bool PSG; // capteur PSG en entrée aiguillage gauche
	bool PSG_past;
	bool PSD; // capteur PSD en entrée aiguillage droit
	bool PSD_past;

	bool PS;
	bool PS_past;

	bool Nav_PS;

	bool Aig_D;
	bool Aig_G;


	int tab_AI[taille_tab];
	int tab_att;//indice de la navette à traiter
	int tab_arr;//indice pour la navette qui arrive

	int num_aiguillage ;
	int num_stop_droit;
	int num_stop_gauche;
	int num_capt_sortie;


	std_msgs::Int32 num_STOPg;
	std_msgs::Int32 num_STOPd;

	std_msgs::Int32 num_AIG;


 
public:
	A12(ros::NodeHandle nh);
	//void init(ros::NodeHandle nh);
	//void SensorCallbackd(const std_msgs::Int32::ConstPtr& msg);
	//void SensorCallbackg(const std_msgs::Int32::ConstPtr& msg);
	void SensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg);
	void Attente(int attd, int attg);
	void Gauche();
	void Droit();
	void Aiguille_Navette();

};

#endif
