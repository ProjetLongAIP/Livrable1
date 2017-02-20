#include "A12.h"  

#include <ros/ros.h>

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"
//#include "actionneurs.h"
#include <unistd.h>
#include <std_msgs/Int32.h>

using namespace std;

A12::A12(ros::NodeHandle nh)
{
	cout<<"Initialisation du callback"<<endl;
	//VREPsubSensor23 = nh.subscribe("vrep/Sensor23", 1,&A12::SensorCallbackd,this);
	//VREPsubSensor24 = nh.subscribe("vrep/Sensor24", 1,&A12::SensorCallbackg,this);


	VREPsubSensor = nh.subscribe("vrep/StopSensor", 1000, &A12::SensorCallback, this);
	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1000, &A12::SwitchSensorCallback, this);

	ShStop = nh.advertise<std_msgs::Int32>("/commande/ArretNavette", 1000);
	ShStart = nh.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 1000);
	AigDev = nh.advertise<std_msgs::Int32>("/commande/DeverouilleAiguillage", 1000);
	AigVer = nh.advertise<std_msgs::Int32>("/commande/VerouilleAiguillage", 1000);
	AigGauche = nh.advertise<std_msgs::Int32>("/commande/AiguillageGauche", 1000);
	AigDroit = nh.advertise<std_msgs::Int32>("/commande/AiguillageDroit", 1000);

	for (int i=0;i<taille_tab;i++) tab_AI[i]=0;//=0 pas de navette; =-1 navette à gauche; =1 navette à droite
	tab_att=0;	
	tab_arr=0;
	

	PSG_past=0;
	PSD_past=0;
	PS_past=0;

	Nav_PS= 0;

	num_aiguillage = 12;
	num_stop_droit = 24;
	num_stop_gauche = 23;
	num_capt_sortie = 1;


	num_STOPg.data=num_stop_gauche;
	num_STOPd.data=num_stop_droit;

	num_AIG.data=num_aiguillage;

	
	
}
 



//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Callback////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void A12::SensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	//cout<<"CallBack"<<endl;
	//cout<<"~~~~~~Etat du capteur~~~~~~~"<<msg->data<<endl;

	PSG = (msg->data & (int32_t)pow(2,num_stop_gauche -1)) > 0;
	PSD = (msg->data & (int32_t)pow(2,num_stop_droit -1)) > 0;
	PS = (msg->data & (int32_t)pow(2,num_capt_sortie -1)) > 0;

		
	if(PSG == 1)
	{
	
		if(!PSG_past)
		{
			PSG_past=PSG;

			cout<<"chariot present à gauche"<<endl;
			tab_AI[tab_arr]=-1;
			tab_arr++;
			if(tab_arr==taille_tab) tab_arr=0;

		}
	
	}
	else if(PSG_past)
	{
		cout<<"front descendant capteur gauche"<<endl;
		PSG_past=PSG;
		this->Attente(1,-1);
	}
	
	if(PSD == 1)
	{
	
		if(!PSD_past)
		{
			PSD_past=PSD;

			cout<<"chariot present à droite"<<endl;
			tab_AI[tab_arr]=1;
			tab_arr++;
			if(tab_arr==taille_tab) tab_arr=0;
		}
	
	}
	else if(PSD_past)
	{
		cout<<"front descendant capteur droit"<<endl;
		PSD_past=PSD;
		this->Attente(-1,1);
	}
	
	

	if(PS ==0)
	{	cout<<"PS non actif"<<endl;
		if(PS_past)
		{
			cout<<"Passage PS effectué"<<endl;
			PS_past=PS;
			Nav_PS =1;
		}
	}
	else
	{
		PS_past=PS;
	}
	
}

//SwitchSensor
void A12::SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	Aig_D = (msg->data & (int32_t)pow(2,2*num_aiguillage-2)) > 0;
	Aig_G = (msg->data & (int32_t)pow(2,2*num_aiguillage-1)) > 0;
}

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Commande Aiguillage/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

void A12::Attente(int attg, int attd)
{
	

	if(attd==1)
	{
		cout<<"stop droit"<<endl;
		ShStop.publish(num_STOPd);
	}
	else if(attd==0)
	{
		cout<<"start droit"<<endl;
		ShStart.publish(num_STOPd);
	}

	if(attg==1)
	{
		cout<<"stop gauche"<<endl;
		ShStop.publish(num_STOPg);
	}
	else if(attg==0)
	{
		cout<<"start gauche"<<endl;
		ShStart.publish(num_STOPg);
	}
}

void A12::Gauche()
{
	
	this->Attente(1,1);
	AigDev.publish(num_AIG);
	usleep(1000000);
	AigGauche.publish(num_AIG);
	while(!Aig_G)
	{
	ros::spinOnce();
	}
	//usleep(3000000);
	AigVer.publish(num_AIG);
	usleep(1000000);
	this->Attente(0,1);
}

void A12::Droit()
{
	
	this->Attente(1,1);
	cout<<"Deverouillage"<<endl;
	AigDev.publish(num_AIG);
	usleep(1000000);
	AigDroit.publish(num_AIG);
	while(!Aig_D)
	{
	ros::spinOnce();
	}
	//usleep(3000000);
	AigVer.publish(num_AIG);
	usleep(1000000);
	this->Attente(1,0);
}

	
	


void A12::Aiguille_Navette()
{

	
		if(tab_AI[tab_att]==-1) 
		{
			cout<<"Navette présente à gauche"<<endl;
			this->Gauche();

			while(!Nav_PS)
			{
				usleep(500000);
				cout<<"gauche"<<Nav_PS<<endl;
				ros::spinOnce();
			}
			tab_AI[tab_att]=0;
			tab_att++;
			if(tab_att==taille_tab) tab_att=0;
			cout<<"Remise a 0 PS"<<endl;
			Nav_PS=0;
			
		}
		else if(tab_AI[tab_att]==1)
		{
			cout<<"Navette présente à droite"<<endl;
			this->Droit();

			while(!Nav_PS)
			{
				usleep(500000);
				cout<<"droit"<<Nav_PS<<endl;
				ros::spinOnce();
			}
			tab_AI[tab_att]=0;
			tab_att++;
			if(tab_att==taille_tab) tab_att=0;
			cout<<"Remise a 0 PS"<<endl;
			Nav_PS=0;
			
		}
}





