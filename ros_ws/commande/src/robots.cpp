/**** Projet long N7 2017 ****/

#include "robots.h"

#include <ros/ros.h>
#include <iostream>

using namespace std;

//Messages
#include <robot/RobotJoints.h>
#include <std_msgs/Int32.h>

//Couleur des messages des robots
#define RESET   "\033[0m"
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */



//Constructeur
Robots::Robots(ros::NodeHandle noeud)
{

	//Mise à 0 des retours d'initalisation des robots
	for(int i=0;i<4;i++)
	{
		robotInit[i]=0;
		robotBras[i]=0;
		robotPosition[i]=0;
	}


	/** Publishers **/
	//Positions prédéfinies
	pub_robot_position1=noeud.advertise<std_msgs::Int32>("/commande/Simulation/SendPositionRobot1",1);
	pub_robot_position2=noeud.advertise<std_msgs::Int32>("/commande/Simulation/SendPositionRobot2",1);
	pub_robot_position3=noeud.advertise<std_msgs::Int32>("/commande/Simulation/SendPositionRobot3",1);
	pub_robot_position4=noeud.advertise<std_msgs::Int32>("/commande/Simulation/SendPositionRobot4",1);

	//Positions manuelles
	pub_robot_joints1=noeud.advertise<robot::RobotJoints>("/commande/Simulation/SendJointsRobot1",1);
	pub_robot_joints2=noeud.advertise<robot::RobotJoints>("/commande/Simulation/SendJointsRobot2",1);
	pub_robot_joints3=noeud.advertise<robot::RobotJoints>("/commande/Simulation/SendJointsRobot3",1);
	pub_robot_joints4=noeud.advertise<robot::RobotJoints>("/commande/Simulation/SendJointsRobot4",1);

	//Fermer les pinces
	pub_pince_fermer1=noeud.advertise<std_msgs::Int32>("/commande/Simulation/FermerPinceRobot1",1);
	pub_pince_fermer2=noeud.advertise<std_msgs::Int32>("/commande/Simulation/FermerPinceRobot2",1);
	pub_pince_fermer3=noeud.advertise<std_msgs::Int32>("/commande/Simulation/FermerPinceRobot3",1);
	pub_pince_fermer4=noeud.advertise<std_msgs::Int32>("/commande/Simulation/FermerPinceRobot4",1);

	//Ouvrir les pinces
	pub_pince_ouvrir1=noeud.advertise<std_msgs::Int32>("/commande/Simulation/OuvrirPinceRobot1",1);
	pub_pince_ouvrir2=noeud.advertise<std_msgs::Int32>("/commande/Simulation/OuvrirPinceRobot2",1);	
	pub_pince_ouvrir3=noeud.advertise<std_msgs::Int32>("/commande/Simulation/OuvrirPinceRobot3",1);
	pub_pince_ouvrir4=noeud.advertise<std_msgs::Int32>("/commande/Simulation/OuvrirPinceRobot4",1);

	//Descendre le bras
	pub_descendre1=noeud.advertise<std_msgs::Int32>("/commande/Simulation/DescendreBras1",1);
	pub_descendre2=noeud.advertise<std_msgs::Int32>("/commande/Simulation/DescendreBras2",1);
	pub_descendre3=noeud.advertise<std_msgs::Int32>("/commande/Simulation/DescendreBras3",1);
	pub_descendre4=noeud.advertise<std_msgs::Int32>("/commande/Simulation/DescendreBras4",1);
	
	//Descendre le bras
	pub_monter1=noeud.advertise<std_msgs::Int32>("/commande/Simulation/MonterBras1",1);
	pub_monter2=noeud.advertise<std_msgs::Int32>("/commande/Simulation/MonterBras2",1);
	pub_monter3=noeud.advertise<std_msgs::Int32>("/commande/Simulation/MonterBras3",1);
	pub_monter4=noeud.advertise<std_msgs::Int32>("/commande/Simulation/MonterBras4",1);


	/*** Subscribers ***/
	//Retour des robots vers la commande
	sub_retourRobot1 = noeud.subscribe("/commande/Simulation/retourCommande1", 100, &Robots::RetourRobot1Callback,this);
	sub_retourRobot2 = noeud.subscribe("/commande/Simulation/retourCommande2", 100, &Robots::RetourRobot2Callback,this);
	sub_retourRobot3 = noeud.subscribe("/commande/Simulation/retourCommande3", 100, &Robots::RetourRobot3Callback,this);
	sub_retourRobot4 = noeud.subscribe("/commande/Simulation/retourCommande4", 100, &Robots::RetourRobot4Callback,this);

}



//Destructeur
Robots::~Robots()
{}



/*** Envoyer les robots automatiquement ***/
//Fonction pour envoyer un robot choisi dans une position prédéfinie
void Robots::EnvoyerPosition(int numRobot, int numPosition)
{
	//Déclaration du message
	std_msgs::Int32 msg;

	//Numéro de la position souhaitée
	if(numPosition<1 || numPosition>4)
	{
		cout <<  BOLDMAGENTA << "Le numero de la position souhaitee doit etre compris entre 1 et 4." << RESET << endl;
	}
	else
	{
		msg.data = numPosition;
	}

	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:	
			pub_robot_position1.publish(msg);
			break;

		case 2:	
			pub_robot_position2.publish(msg);
			break;

		case 3:	
			pub_robot_position3.publish(msg);
			break;

		case 4:	
			pub_robot_position4.publish(msg);
			break;
	
		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}
}



/*** Envoyer les robots manuellement ***/
//Fonction pour envoyer un robot choisi dans une position définie par l'utilisateur 
void Robots::EnvoyerAngles(int numRobot, int angle1, int angle2, int angle3, int angle4, int angle5, int angle6, int angle7)
{
	//Déclaration du message
	robot::RobotJoints msg;

	//Angles (en degrés) choisis par l'utilisateur
	msg.joint1 = angle1;
	msg.joint2 = angle2;
	msg.joint3 = angle3;
	msg.joint4 = angle4;
	msg.joint5 = angle5;
	msg.joint6 = angle6;
	msg.joint7 = angle7;

	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:	
			pub_robot_joints1.publish(msg);
			break;

		case 2:	
			pub_robot_joints2.publish(msg);
			break;

		case 3:	
			pub_robot_joints3.publish(msg);
			break;

		case 4:	
			pub_robot_joints4.publish(msg);
			break;
	
		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}	
}



/*** Fermer les pinces ***/
//Fonction pour fermer la pince d'un robot choisi
void Robots::FermerPince(int numRobot)
{
	//Déclaration du message
	std_msgs::Int32 msg;
	msg.data=1;
	
	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:	
			pub_pince_fermer1.publish(msg);
			break;

		case 2:	
			pub_pince_fermer2.publish(msg);
			break;

		case 3:	
			pub_pince_fermer3.publish(msg);
			break;

		case 4:	
			pub_pince_fermer4.publish(msg);
			break;
	
		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}	
}



/*** Ouvrir les pinces ***/
//Fonction pour ouvrir la pince d'un robot choisi
void Robots::OuvrirPince(int numRobot)
{
	//Déclaration du message
	std_msgs::Int32 msg;
	msg.data=0;
	
	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:	
			pub_pince_ouvrir1.publish(msg);
			break;

		case 2:	
			pub_pince_ouvrir2.publish(msg);
			break;

		case 3:	
			pub_pince_ouvrir3.publish(msg);
			break;

		case 4:	
			pub_pince_ouvrir4.publish(msg);
			break;
	
		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}	
}



/*** Descendre les bras ***/
//Fonction pour descendre le bras d'un robot choisi
void Robots::DescendreBras(int numRobot)
{
	//Déclaration du message
	std_msgs::Int32 msg;
	msg.data=1;

	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:	
			pub_descendre1.publish(msg);
			break;

		case 2:	
			pub_descendre2.publish(msg);
			break;

		case 3:	
			pub_descendre3.publish(msg);
			break;

		case 4:	
			pub_descendre4.publish(msg);
			break;
	
		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}	
}



/*** Monter les bras ***/
//Fonction pour monter le bras d'un robot choisi
void Robots::MonterBras(int numRobot)
{
	//Déclaration du message
	std_msgs::Int32 msg;
	msg.data=0;

	//Publication du message vers le node robot en fonction du numéro de robot à commander
	switch(numRobot)
	{
		case 1:	
			pub_monter1.publish(msg);
			break;

		case 2:	
			pub_monter2.publish(msg);
			break;

		case 3:	
			pub_monter3.publish(msg);
			break;

		case 4:	
			pub_monter4.publish(msg);
			break;
	
		default:
			cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
			break;
	}	
}




/*** Retour des robots ***/
//Fonction Callback pour le retour des informations relatives au robot 1
void Robots::RetourRobot1Callback(const std_msgs::Int32::ConstPtr& msg)
{
	int retour;
	retour=msg->data;
	
	switch(retour)
	{
		case 0:
			cout <<  BOLDCYAN << "Robot 1 initialise" << RESET << endl;
			robotInit[0]=1;
			break;

		case 1:
			cout <<  BOLDCYAN << "Mouvement non effectue pour le robot 1" << RESET << endl;
			robotPosition[0]=0;
			break;

		case 2:
			cout <<  BOLDCYAN << "Robot 1 en position" << RESET << endl;
			robotPosition[0]=1;
			break;

		case 3:
			cout <<  BOLDCYAN << "Bras bloque pour le robot 1" << RESET << endl;
			robotBras[0] = 0;
			break;	

		case 4:
			cout << BOLDCYAN << "Bras descendu pour le robot 1" << RESET << endl;
			robotBras[0] = -1;
			break;	

		case 5:
			cout << BOLDCYAN << "Bras monte pour le robot 1" << RESET << endl;
			robotBras[0] = 1;
			break;	
	}
}


//Fonction Callback pour le retour des informations relatives au robot 2
void Robots::RetourRobot2Callback(const std_msgs::Int32::ConstPtr& msg)
{
	int retour;
	retour=msg->data;
	
	switch(retour)
	{
		case 0:
			cout <<  BOLDCYAN << "Robot 2 initialise" << RESET << endl;
			robotInit[1]=1;
			break;

		case 1:
			cout <<  BOLDCYAN << "Mouvement non effectue pour le robot 2" << RESET << endl;
			robotPosition[1]=0;
			break;

		case 2:
			cout <<  BOLDCYAN << "Robot 2 en position" << RESET << endl;
			robotPosition[1]=1;
			break;

		case 3:
			cout <<  BOLDCYAN << "Bras bloque pour le robot 2" << RESET << endl;
			robotBras[1] = 0;
			break;	

		case 4:
			cout << BOLDCYAN << "Bras descendu pour le robot 2" << RESET << endl;
			robotBras[1] = -1;
			break;	

		case 5:
			cout << BOLDCYAN << "Bras monte pour le robot 2" << RESET << endl;
			robotBras[1] = 1;
			break;	
	}
}


//Fonction Callback pour le retour des informations relatives au robot 3
void Robots::RetourRobot3Callback(const std_msgs::Int32::ConstPtr& msg)
{
	int retour;
	retour=msg->data;
	
	switch(retour)
	{
		case 0:
			cout <<  BOLDCYAN << "Robot 3 initialise" << RESET << endl;
			robotInit[2]=1;
			break;

		case 1:
			cout <<  BOLDCYAN << "Mouvement non effectue pour le robot 3" << RESET << endl;
			robotPosition[2]=0;
			break;

		case 2:
			cout <<  BOLDCYAN << "Robot 3 en position" << RESET << endl;
			robotPosition[2]=1;
			break;

		case 3:
			cout <<  BOLDCYAN << "Bras bloque pour le robot 3" << RESET << endl;
			robotBras[2] = 0;
			break;	

		case 4:
			cout << BOLDCYAN << "Bras descendu pour le robot 3" << RESET << endl;
			robotBras[2] = -1;
			break;	

		case 5:
			cout << BOLDCYAN << "Bras monte pour le robot 3" << RESET << endl;
			robotBras[2] = 1;
			break;	
	}
}


//Fonction Callback pour le retour des informations relatives au robot 4
void Robots::RetourRobot4Callback(const std_msgs::Int32::ConstPtr& msg)
{
	int retour;
	retour=msg->data;
	
	switch(retour)
	{
		case 0:
			cout <<  BOLDCYAN << "Robot 4 initialise" << RESET << endl;
			robotInit[3]=1;
			break;

		case 1:
			cout <<  BOLDCYAN << "Mouvement non effectue pour le robot 4" << RESET << endl;
			robotPosition[3]=0;
			break;

		case 2:
			cout <<  BOLDCYAN << "Robot 4 en position" << RESET << endl;
			robotPosition[3]=1;
			break;

		case 3:
			cout <<  BOLDCYAN << "Bras bloque pour le robot 4" << RESET << endl;
			robotBras[3] = 0;
			break;	

		case 4:
			cout << BOLDCYAN << "Bras descendu pour le robot 4" << RESET << endl;
			robotBras[3] = -1;
			break;	

		case 5:
			cout << BOLDCYAN << "Bras monte pour le robot 4" << RESET << endl;
			robotBras[3] = 1;
			break;	
	}
}



//Fonction permettant de savoir si le robot choisi est initalisé
int Robots::RobotInitialise(int numRobot)
{
	int Robot;
	if(numRobot<1 || numRobot>4)
	{
		cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
	}
	else
	{
		Robot=robotInit[numRobot-1];
	}
	return Robot;
}



//Fonction permettant de savoir si le robot choisi est en position
int Robots::RobotEnPosition(int numRobot)
{
	int Robot;
	if(numRobot<1 || numRobot>4)
	{
		cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
	}
	else
	{
		Robot=robotPosition[numRobot-1];
		robotPosition[numRobot-1]=0;
	}
	return Robot;	
}



//Fonction permettant de savoir si le robot choisi est en position
int Robots::BrasEnPosition(int numRobot)
{
	int Robot;
	if(numRobot<1 || numRobot>4)
	{
		cout <<  BOLDMAGENTA << "Le numero du robot doit etre compris entre 1 et 4." << RESET << endl;
	}
	else
	{
		Robot=robotBras[numRobot-1];
		robotBras[numRobot-1]=0;
	}
	return Robot;
}

