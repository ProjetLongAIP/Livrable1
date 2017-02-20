#include "A11.h"  

#include <ros/ros.h>

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"
//#include "actionneurs.h"
//#include "capteur.h"
#include <unistd.h>
#include <std_msgs/Int32.h>

using namespace std;

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Constructeur////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
A11::A11(ros::NodeHandle nh)
{
	cout<<"Initialisation du callback"<<endl;
	//VREPsubSensor = nh.subscribe("vrep/Sensor20", 1,&A11::SensorCallback,this);



	VREPsubStopSensor = nh.subscribe("vrep/StopSensor", 1000, &A11::StopSensorCallback, this);
	VREPsubRailSensor = nh.subscribe("vrep/RailSensor", 1000, &A11::RailSensorCallback, this);
	VREPsubSwitchSensor = nh.subscribe("vrep/SwitchSensor", 1000, &A11::SwitchSensorCallback, this);
	
	//Capteurs Capteurs(noeud); 

	



	ShStop = nh.advertise<std_msgs::Int32>("/commande/ArretNavette", 1);
	ShStart = nh.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 1);
	AigDev = nh.advertise<std_msgs::Int32>("/commande/DeverouilleAiguillage", 1);
	AigVer = nh.advertise<std_msgs::Int32>("/commande/VerouilleAiguillage", 1);
	AigGauche = nh.advertise<std_msgs::Int32>("/commande/AiguillageGauche", 1);
	AigDroit = nh.advertise<std_msgs::Int32>("/commande/AiguillageDroit", 1);



	M=2;

	for (int i=0;i<taille_tab;i++) tab_AI[i]=0;//=0 pas de navette; =-1 navette à gauche; =1 navette à droite
	tab_att=0;	
	tab_arr=0;
	
	Nav_CPD=0;
	Nav_CPG=0;
PS_past=0;


	//this->A11Attente(0);
}




/*void A11::init(ros::NodeHandle nh)
{
	
}*/




//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Callback////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//StopSensor
void A11::StopSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	//cout<<"CallBack"<<endl;
	//cout<<"~~~~~~Etat du capteur~~~~~~~"<<msg->data<<endl;


	 PS = (msg->data & (int32_t)pow(2,19)) > 0;



	if(PS)
	{
	
		if(!PS_past)
		{
			PS_past=PS;

			cout<<"chariot present"<<endl;
			
			if(M==1)
			{
				cout<<"chariot 2"<<endl;

				tab_AI[tab_arr]=-1;
				tab_arr++;
				if(tab_arr==taille_tab) tab_arr=0;
				
				//this->A11Gauche();
				//usleep(2000000);
				M=2;
			}
			else if(M==2)
			{
				cout<<"chariot 1"<<endl;

				tab_AI[tab_arr]=1;
				tab_arr++;
				if(tab_arr==taille_tab) tab_arr=0;

				//this->A11Droit();
				//usleep(2000000);
				M=1;
			}
		}
	}else if(PS_past)
		{
		cout<<"front descendant capteur"<<endl;
		PS_past=PS;
		this->Attente(1);
		}
}

//RailSensor
void A11::RailSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	CPD = (msg->data & (int32_t)pow(2,8)) > 0;
	CPG = (msg->data & (int32_t)pow(2,9)) > 0;
	
	//Detection passage d'une navette en CP9
	if(CPD)
	{
		cout<<"CP9 actif"<<endl;
		if(!CPD_past)
		{
			cout<<"Passage CP9"<<endl;
			CPD_past=CPD;
			Nav_CPD=1;
		}
	}else
	{
		CPD_past=CPD;
	}

	//Detection passage d'une navette en CP10
	if(CPG)
	{
		cout<<"CP10 actif"<<endl;
		if(!CPG_past)
		{
			cout<<"Passage CP10"<<endl;
			CPG_past=CPG;
			Nav_CPG=1;
		}
	}else
	{
		CPG_past=CPG;
	}

}


//SwitchSensor
void A11::SwitchSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	Aig_D = (msg->data & (int32_t)pow(2,2*11-2)) > 0;
	Aig_G = (msg->data & (int32_t)pow(2,2*11-1)) > 0;
}
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Commande Aiguillage/////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
void A11::Attente(int att)
{
	std_msgs::Int32 num_STOP;
	num_STOP.data=20;
	if(att==1)
	{
		ShStop.publish(num_STOP);
	}else
	{
		ShStart.publish(num_STOP);
	}
}



void A11::Gauche()
{
	

	std_msgs::Int32 AI11;
	AI11.data=11;
	this->Attente(1);
	AigDev.publish(AI11);
	usleep(100000);
	AigGauche.publish(AI11);
	while(!Aig_G)
	{
		ros::spinOnce();
	}
	//usleep(5000000);
	AigVer.publish(AI11);
	usleep(100000);
	this->Attente(0);
	
	
}


void A11::Droit()
{
	std_msgs::Int32 AI11;
	AI11.data=11;
	this->Attente(1);
	cout<<"Deverouillage"<<endl;
	AigDev.publish(AI11);
	usleep(100000);
	AigDroit.publish(AI11);
	while(!Aig_D)
	{
		ros::spinOnce();
	}
	//usleep(5000000);
	AigVer.publish(AI11);
	usleep(100000);
	this->Attente(0);
	
}


void A11::Aiguille_Navette()
{

	//while(1)
	//{
		if(tab_AI[tab_att]==-1)
		{
			cout<<"Navette gauche"<<endl;
			this->Gauche();
			//if(Nav_CP10==1)
			//{
			while(!Nav_CPG)
			{
				//usleep(500000);
				cout<<"spinonce"<<Nav_CPG<<endl;
				ros::spinOnce();
			}
			tab_AI[tab_att]=0;
			tab_att++;
			if(tab_att==taille_tab) tab_att=0;
			cout<<"Remise a 0 CP10"<<endl;
			Nav_CPG=0;
			//}
		}
		if(tab_AI[tab_att]==1)
		{
			cout<<"Navette droit"<<endl;
			this->Droit();

			cout<<"Nav CP9 :"<<Nav_CPD<<endl;
			//if(Nav_CPD==1)
			//{
			while(!Nav_CPD)
			{
				//usleep(500000);
				cout<<"spinonce"<<Nav_CPD<<endl;
				ros::spinOnce();
			}
			tab_AI[tab_att]=0;
			tab_att++;
			if(tab_att==taille_tab) tab_att=0;
			cout<<"Remise a 0 CP9"<<endl;
			Nav_CPD=0;
			//}
		}
		//if(tab_AI[tab_att]==0) cout<<"Pas de navette"<<endl;
	//}
}





