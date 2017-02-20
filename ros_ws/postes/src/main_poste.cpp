/*
 * ************************************* *
 * Copyright 2016, TER Ligne Transitique *
 * All rights reserved.  	         *
 * ************************************* *
*/


//#include "capteurs.h" 
//#include "actionneurs.h" 
//#include "commande.h"
#include <ros/ros.h>
#include <unistd.h>
#include "Poste.h"
//#include "actionneurs.h"
//#include "cmdRobot.h"

using namespace std;

#define NOMBRE_PLACE 100
#define RESET   "\033[0m"
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */


void SensorCallback()
{
	cout<<"~~~~~~~~Capteur actif~~~~~~~~~~"<<endl;
}


int main(int argc, char **argv)
{	

//initialisation du noeud ros et création d'un handle associé au noeud
	ros::init(argc, argv, "Poste1");	
	ros::NodeHandle noeud;

	Poste Poste1(noeud);

	




//création et initialisation des objets Capteur et Actionneurs

	//Capteurs Capteurs(noeud); 

	//Actionneurs Actionneurs(noeud);

	//CmdRobot Robot(noeud);

	ros::Rate loop_rate(25); //fréquence de la boucle 

	

	
	
//////////////////////////////////////////////
//////////////////////////////////////////////
//////////////////////////////////////////////

		cout <<"~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
		cout <<"         Poste 1         "<<endl;
		cout <<"~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
		
	

	while (ros::ok())
	{



		/*
		// Déplacement des navettes si on utilise la simulation
		if(Capteurs.SIMULATION==true && Deplacement_effectue==0)
		{
			// Déplacement devant 2 stops différents pour initialisation
			Deplacer_navettes_2(STx,RxD,RxG,Vx,Dx,PIx,Actionneurs,12,24);

			//Mode_ligne(Actionneurs,STx,RxD,RxG,Vx,Dx,PIx);

			Deplacement_effectue=1;
		}

		// Actualisation des capteurs

		Capteurs.Actualiser(PSx,DxD,DxG,CPx,CPIx);
		Afficher_capteurs(PSx,DxD,DxG,CPx,CPIx);
		Afficher_marquage_RdP(M,NOMBRE_PLACE);
*/

		ros::spinOnce(); //permet aux fonction callback de ros dans les objets d'êtres appelées
		loop_rate.sleep(); //permet de synchroniser la boucle while. Il attend le temps qu'il reste pour faire le 25Hz (ou la fréquence indiquée dans le loop_rate)
	}

	return 0;


}
