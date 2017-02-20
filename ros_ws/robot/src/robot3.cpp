/**** Projet long N7 2017 ****/

#include "robot3.h"

#include "ros/ros.h"

//Services Vrep
#include <vrep_common/simRosEnablePublisher.h>
#include <vrep_common/simRosEnableSubscriber.h>

#include <vrep_common/simRosSetJointState.h>
#include <vrep_common/simRosGetObjectHandle.h>
#include <vrep_common/simRosGetJointState.h>

//Messages
#include <robot/RobotJoints.h>
#include <std_msgs/Int32.h>

#include <vector>
#include <sstream>
#include <stdlib.h>



//Constructeur
Robot3::Robot3()
{
	//Valeur de pi
	pi=3.14159265359;
	
	//Initialisation des tableaux
	for (int i=0;i<7;i++)
	{	
		mymodes[i]=1; //Ne pas changer la valeur : l'ensemble des setModes doit être à 1 pour que les joints des robots soient commandés
	}
}



//Destructeur
Robot3::~Robot3()
{}



/*** Envoyer le robot automatiquement ***/
//Fonction Callback permettant d'envoyer le robot dans une position prédéfinie
void Robot3::SendPositionCallback(const std_msgs::Int32::ConstPtr& msg)
{
	//Récupération des données du message : numéro de la position prédéfinie
	int pos;
	pos = msg->data;

	//Choix de la position en fonction du numéro reçu
	switch(pos)
	{
		case 1:
			Rpos[0]=128*pi/180;
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-40*pi/180;
			break;

		case 2:
			Rpos[0]=60*pi/180;
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-20*pi/180;
			break;

		case 3:
			Rpos[0]=15*pi/180;
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-65*pi/180;
			break;
	
		case 4:
			Rpos[0]=-53*pi/180;
			Rpos[1]=90*pi/180;
			Rpos[2]=90*pi/180;
			Rpos[3]=80*pi/180;
			Rpos[4]=90*pi/180;
			Rpos[5]=-90*pi/180;
			Rpos[6]=-40*pi/180;
			break;
	}

	
	//Utilisation du service simRosSetJointState
	std::vector<int> myhandles(Rints, Rints+sizeof(Rints)/sizeof(int));	
	std::vector<unsigned char> mysetmodes(mymodes, mymodes+sizeof(mymodes)/sizeof(unsigned char));
	std::vector<float> myvalues(Rpos, Rpos+sizeof(Rpos)/sizeof(float));

	srv_simRosSetJoint.request.handles = myhandles;
	srv_simRosSetJoint.request.setModes = mysetmodes;
	srv_simRosSetJoint.request.values = myvalues;
	
	client_simRosSetJoint.call(srv_simRosSetJoint);

	if(srv_simRosSetJoint.response.result==-1)
	{
		ROS_INFO("Position predefinie %d pour le robot 3 non atteinte",pos);
		//Retour vers la commande 
		std_msgs::Int32 retour;
		retour.data = 1;
		pub_retourCommande.publish(retour);
	}
	else
	{	
		//Utilisation du service simRosGetJointState pour savoir si le mouvement du robot est terminé : on considère que la position est atteinte si la position est à 0.001 de sa consigne
		float Position;
		for(int i=0;i<7;i++)
		{
			Position=0;
			srv_simRosGetJoint.request.handle = Rints[i];
			client_simRosGetJoint.call(srv_simRosGetJoint);
			Position = srv_simRosGetJoint.response.state.position[0];

			while(std::abs(Position-Rpos[i])>=0.001)
			{
				srv_simRosGetJoint.request.handle = Rints[i];
				client_simRosGetJoint.call(srv_simRosGetJoint);
				Position = srv_simRosGetJoint.response.state.position[0];	
			}
		}

		ROS_INFO("Position atteinte");
		//Retour vers la commande
		std_msgs::Int32 retour;
		retour.data = 2;
		pub_retourCommande.publish(retour);
	}
}




/*** Envoyer le robot manuellement ***/
//Fonction Callback permettant d'envoyer le robot dans une position choisie par l'utilisateur
void Robot3::SendJointsCallback(const robot::RobotJoints::ConstPtr& msg)
{
	//Récupération des données du message : angles choisis par l'utilisateur
	Rpos[0]=(msg->joint1)*pi/180;
	Rpos[1]=(msg->joint2)*pi/180;
	Rpos[2]=(msg->joint3)*pi/180;
	Rpos[3]=(msg->joint4)*pi/180;
	Rpos[4]=(msg->joint5)*pi/180;
	Rpos[5]=(msg->joint6)*pi/180;
	Rpos[6]=(msg->joint7)*pi/180;


	//Utilisation du service simRosSetJointState
	std::vector<int> myhandles(Rints, Rints+sizeof(Rints)/sizeof(int));	
	std::vector<unsigned char> mysetmodes(mymodes, mymodes+sizeof(mymodes)/sizeof(unsigned char));
	std::vector<float> myvalues(Rpos, Rpos+sizeof(Rpos)/sizeof(float));
	
	srv_simRosSetJoint.request.handles = myhandles;
	srv_simRosSetJoint.request.setModes = mysetmodes;
	srv_simRosSetJoint.request.values = myvalues;

	client_simRosSetJoint.call(srv_simRosSetJoint);

	if(srv_simRosSetJoint.response.result==-1)
	{
		ROS_INFO("Position definie manuellement pour le robot 3 non atteinte");
		//Retour vers la commande
		std_msgs::Int32 retour;
		retour.data = 1;
		pub_retourCommande.publish(retour);
	}
	else
	{	
		//Utilisation du service simRosGetJointState pour savoir si le mouvement du robot est terminé : on considère que la position est atteinte si la position est à 0.001 de sa consigne
		float Position;
		for(int i=0;i<7;i++)
		{
			Position=0;
			srv_simRosGetJoint.request.handle = Rints[i];
			client_simRosGetJoint.call(srv_simRosGetJoint);
			Position = srv_simRosGetJoint.response.state.position[0];

			while(std::abs(Position-Rpos[i])>=0.001)
			{
				srv_simRosGetJoint.request.handle = Rints[i];
				client_simRosGetJoint.call(srv_simRosGetJoint);
				Position = srv_simRosGetJoint.response.state.position[0];	
			}
		}

		ROS_INFO("Position atteinte");
		//Retour vers la commande
		std_msgs::Int32 retour;
		retour.data = 2;
		pub_retourCommande.publish(retour);
	}
}



/*** Fermer la pince ***/
//Fonction Callback permettant de fermer la pince du robot en envoyant une commande sur le topic correspondant
void Robot3::FermerPinceCallback(const std_msgs::Int32::ConstPtr& msg)
{
	//Message de commande de fermeture de la pince (1 pour fermer, 0 pour ouvrir)
	std_msgs::Int32 cmd;
	cmd.data = 1;
	
	pub_pince.publish(cmd);
}



/*** Ouvrir la pince ***/
//Fonction Callback permettant d'ouvrir la pince du robot en envoyant une commande sur le topic correspondant
void Robot3::OuvrirPinceCallback(const std_msgs::Int32::ConstPtr& msg)
{
	//Message de commande d'ouverture de la pince (1 pour fermer, 0 pour ouvrir)
	std_msgs::Int32 cmd;
	cmd.data = 0;

	pub_pince.publish(cmd);
}



/*** Descendre le bras ***/
//Fonction Callback permettant de mettre le bras en position basse
void Robot3::DescendreBrasCallback(const std_msgs::Int32::ConstPtr& msg)
{
	//Récupération et modification de la position actuelle
	Rpos[0]=Rpos[0]+2*pi/180;
	Rpos[1]=Rpos[1]+10*pi/180;
	Rpos[2]=Rpos[2];
	Rpos[3]=Rpos[3];
	Rpos[4]=Rpos[4]+10*pi/180;
	Rpos[5]=Rpos[5]+3*pi/180;
	Rpos[6]=Rpos[6]+6*pi/180;

	//Utilisation du service simRosSetJointState
	std::vector<int> myhandles(Rints, Rints+sizeof(Rints)/sizeof(int));	
	std::vector<unsigned char> mysetmodes(mymodes, mymodes+sizeof(mymodes)/sizeof(unsigned char));
	std::vector<float> myvalues(Rpos, Rpos+sizeof(Rpos)/sizeof(float));

	srv_simRosSetJoint.request.handles = myhandles;
	srv_simRosSetJoint.request.setModes = mysetmodes;
	srv_simRosSetJoint.request.values = myvalues;
	
	client_simRosSetJoint.call(srv_simRosSetJoint);

	if(srv_simRosSetJoint.response.result==-1)
	{
		ROS_INFO("Bras non descendu pour le robot 3");
		//Retour vers la commande
		std_msgs::Int32 retour;
		retour.data = 3;
		pub_retourCommande.publish(retour);
	}
	else
	{
		//Utilisation du service simRosGetJointState pour savoir si le mouvement du robot est terminé : on considère que la position est atteinte si la position est à 0.001 de sa consigne
		float Position;
		for(int i=0;i<7;i++)
		{
			Position=0;
			srv_simRosGetJoint.request.handle = Rints[i];
			client_simRosGetJoint.call(srv_simRosGetJoint);
			Position = srv_simRosGetJoint.response.state.position[0];

			while(std::abs(Position-Rpos[i])>=0.001)
			{
				srv_simRosGetJoint.request.handle = Rints[i];
				client_simRosGetJoint.call(srv_simRosGetJoint);
				Position = srv_simRosGetJoint.response.state.position[0];	
			}
		}

		ROS_INFO("Bras descendu pour le robot 3");
		//Retour vers la commande
		std_msgs::Int32 retour;
		retour.data = 4;
		pub_retourCommande.publish(retour);
	}
}



/*** Monter le bras ***/
//Fonction Callback permettant de mettre le bras en position haute
void Robot3::MonterBrasCallback(const std_msgs::Int32::ConstPtr& msg)
{
	//Récupération et modification de la position actuelle
	Rpos[0]=Rpos[0]-2*pi/180;
	Rpos[1]=Rpos[1]-10*pi/180;
	Rpos[2]=Rpos[2];
	Rpos[3]=Rpos[3];
	Rpos[4]=Rpos[4]-10*pi/180;
	Rpos[5]=Rpos[5]-3*pi/180;
	Rpos[6]=Rpos[6]-6*pi/180;

	//Utilisation du service simRosSetJointState
	std::vector<int> myhandles(Rints, Rints+sizeof(Rints)/sizeof(int));	
	std::vector<unsigned char> mysetmodes(mymodes, mymodes+sizeof(mymodes)/sizeof(unsigned char));
	std::vector<float> myvalues(Rpos, Rpos+sizeof(Rpos)/sizeof(float));

	srv_simRosSetJoint.request.handles = myhandles;
	srv_simRosSetJoint.request.setModes = mysetmodes;
	srv_simRosSetJoint.request.values = myvalues;
	
	client_simRosSetJoint.call(srv_simRosSetJoint);

	if(srv_simRosSetJoint.response.result==-1)
	{
		ROS_INFO("Bras non monte pour le robot 3");
		//Retour vers la commande
		std_msgs::Int32 retour;
		retour.data = 3;
		pub_retourCommande.publish(retour);
	}
	else
	{
		//Utilisation du service simRosGetJointState pour savoir si le mouvement du robot est terminé : on considère que la position est atteinte si la position est à 0.001 de sa consigne
		float Position;
		for(int i=0;i<7;i++)
		{
			Position=0;
			srv_simRosGetJoint.request.handle = Rints[i];
			client_simRosGetJoint.call(srv_simRosGetJoint);
			Position = srv_simRosGetJoint.response.state.position[0];

			while(std::abs(Position-Rpos[i])>=0.001)
			{
				srv_simRosGetJoint.request.handle = Rints[i];
				client_simRosGetJoint.call(srv_simRosGetJoint);
				Position = srv_simRosGetJoint.response.state.position[0];	
			}
		}

		ROS_INFO("Bras monte pour le robot 3");
		//Retour vers la commande
		std_msgs::Int32 retour;
		retour.data = 5;
		pub_retourCommande.publish(retour);
	}
}




//Initialisation des services, des publishers et des subscribers + Récupération des handles des robots
void Robot3::init(ros::NodeHandle noeud)
{
	//Déclaration service simRosGetObjectHandle
	client_simRosGetHandle = noeud.serviceClient<vrep_common::simRosGetObjectHandle>("/vrep/simRosGetObjectHandle");

	//Déclaration service simRosSetJointState
	client_simRosSetJoint = noeud.serviceClient<vrep_common::simRosSetJointState>("/vrep/simRosSetJointState");

	//Déclaration service simRosGetJointState
	client_simRosGetJoint = noeud.serviceClient<vrep_common::simRosGetJointState>("/vrep/simRosGetJointState");


	//Subscribers
	planifSendPosition = noeud.subscribe("/commande/Simulation/SendPositionRobot3",1,&Robot3::SendPositionCallback,this);

	planifSendJoints = noeud.subscribe("/commande/Simulation/SendJointsRobot3",1,&Robot3::SendJointsCallback,this);
	
 	planifFermerPince = noeud.subscribe("/commande/Simulation/FermerPinceRobot3",1,&Robot3::FermerPinceCallback,this);

	planifOuvrirPince = noeud.subscribe("/commande/Simulation/OuvrirPinceRobot3",1,&Robot3::OuvrirPinceCallback,this);
	
	planifDescendreBras = noeud.subscribe("/commande/Simulation/DescendreBras3",1,&Robot3::DescendreBrasCallback,this);

	planifMonterBras = noeud.subscribe("/commande/Simulation/MonterBras3",1,&Robot3::MonterBrasCallback,this);


	//Publishers
	pub_pince = noeud.advertise<std_msgs::Int32>("/robot/cmdPinceRobot3", 1);

	pub_retourCommande = noeud.advertise<std_msgs::Int32>("/commande/Simulation/retourCommande3", 1);
	

	//Utilisation du service simRosGetObjectHandle pour obtenir les handles du robot 3
	for (int i=1;i<8;i++)
	{
		std::stringstream sr;	
		sr << i;
		srv_simRosGetHandle.request.objectName="LBR_iiwa_14_R820_joint" + sr.str() + "#1";

		client_simRosGetHandle.call(srv_simRosGetHandle);
		
		Rints[i-1]=srv_simRosGetHandle.response.handle;
		
		if(Rints[i-1]==-1)
		{
			ROS_INFO("Robot 3 : Handle non obtenu pour joint %d",i);
		}
		else
		{
			if(i==7)
			{
				ROS_INFO("Robot 3 OK");
				//Retour vers la commande
				std_msgs::Int32 retour;
				retour.data = 0;
				pub_retourCommande.publish(retour);
			}
		} 
	}	
}




