#include "Poste.h"  

#include <ros/ros.h>

// Used API services:
#include "vrep_common/simRosEnablePublisher.h"
#include "vrep_common/simRosEnableSubscriber.h"
//#include "actionneurs.h"
//#include "capteur.h"
#include <unistd.h>
#include <std_msgs/Int32.h>
#include <shuttles/srvGetShuttleStatus.h>


using namespace std;

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Constructeur////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
Poste::Poste(ros::NodeHandle nh)
{
	cout<<"Initialisation du poste"<<endl;
	
// Pour connaitre l'etat de capteurs
	VREPsubStopSensor = nh.subscribe("vrep/StopSensor", 1000, &Poste::StopSensorCallback, this);
	VREPsubRailSensor = nh.subscribe("vrep/RailSensor", 1000, &Poste::RailSensorCallback, this);

// Pour arreter et demarrer les navettes
	ShStop = nh.advertise<std_msgs::Int32>("/commande/ArretNavette", 1);
	ShStart = nh.advertise<std_msgs::Int32>("/commande/DemarrerNavette", 1);

// Pour recevoir et envoyer l'etat des navettes	
	ReceiveShuttle = nh.subscribe("/IOShuttle/A11_P12", 10, &Poste::newShuttleCallback, this);
	SendShuttle = nh.advertise<std_msgs::Int32>("/IOShuttle/A12_Shuttle_in", 10);
	
// Pour communiquer avec le noeud de navettes

	clientGetShuttleStatus = nh.serviceClient<shuttles::srvGetShuttleStatus>("/commande_navette/srvGetShuttleStatus");

// Pour initialiser la gestion des capteurs
	M=2;
	
	Nav_CP_in=0;
	PS_past=0;

}


/*void A11::init(ros::NodeHandle nh)
{
	
}*/


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////Callback////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

//StopSensor
void Poste::StopSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{
	//cout<<"CallBack"<<endl;
	//cout<<"~~~~~~Etat du capteur~~~~~~~"<<msg->data<<endl;


	//Recuperation de la valeur de ...
/*	//PS21
	 	PS1 = (msg->data & (int32_t)pow(2,20)) > 0;
	//PS22	 
		PS2 = (msg->data & (int32_t)pow(2,21)) > 0;	
*/
	//PS24	 
		PS_out = (msg->data & (int32_t)pow(2,23)) > 0;

/*	if(PS1)
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
	}
	else if(PS_past){
		cout<<"front descendant capteur"<<endl;
		PS_past=PS;
		this->Attente(1);
	}


	if(PS2)
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
	}
	else if(PS_past){
		cout<<"front descendant capteur"<<endl;
		PS_past=PS;
		this->Attente(1);
	}
*/

	if(PS_out)
	{
		if(!PS_past)
		{
			PS_past=PS_out;

			cout<<"chariot present"<<endl;
			
			/*if(M==1)
			{
				cout<<"chariot 2"<<endl;

				tab_AI[tab_arr]=-1;
				tab_arr++;
				if(tab_arr==taille_tab) tab_arr=0;
				
				M=2;
			}
			else if(M==2)
			{
				cout<<"chariot 1"<<endl;

				tab_AI[tab_arr]=1;
				tab_arr++;
				if(tab_arr==taille_tab) tab_arr=0;


				M=1;
			}	*/
		}
	}
	else if(PS_past){
		cout<<"front descendant capteur"<<endl;
		PS_past=PS_out;
		//this->Attente(1);
	}

}

//RailSensor
void Poste::RailSensorCallback(const std_msgs::Int32::ConstPtr& msg)
{

	//Recuperation de la valeur de CP9
	CP_in = (msg->data & (int32_t)pow(2,8)) > 0;
	
	//Detection passage d'une navette en CP9
	if(CP_in)
	{
		cout<<"CP9 actif"<<endl;
		if(!CP_in_past)
		{
			cout<<"Passage CP9"<<endl;
			CP_in_past=CP_in;
			Nav_CP_in=1;
		}
	}else
	{
		CP_in_past=CP_in;
	}


}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////   Commande Poste  /////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////



void Poste::newShuttleCallback (const std_msgs::Int32::ConstPtr& msg) {

int shuttleNew =  msg->data;

ListShuttles.push_back(shuttleNew);

// Afficher la liste
for (std::list<int>::iterator it=ListShuttles.begin(); it != ListShuttles.end(); ++it)
    std::cout << ' ' << *it;

  std::cout << '\n';

	ROS_INFO("Recuperation via service info navette d'handle %d",shuttleNew );
	srvGSS.request.handle = shuttleNew;
	
if (clientGetShuttleStatus.call(srvGSS)){
	ROS_INFO("%s , destination %d , produit %d, handle de la plateforme %d, zone %d" ,srvGSS.response.name.c_str(),srvGSS.response.destination ,srvGSS.response.product, srvGSS.response.handlePlatform, srvGSS.response.zone);
}
else{
	ROS_WARN("Cette navette n'existe pas");
}

	
if (srvGSS.response.destination != 1 && srvGSS.response.destination != 2){
	ROS_WARN("Cette navette ne devrait pas venir ici");
}

}

