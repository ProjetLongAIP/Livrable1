/*
 * ********************************* *
 * Copyright 2016, STEC Projet Long. *
 * All rights reserved.  	     *
 * ********************************* *
*/


#include "vrepController.h"
#include "vrep_common/simRosPauseSimulation.h"
#include "vrep_common/simRosStartSimulation.h"
#include "vrep_common/simRosGetObjectHandle.h"
#include "vrep_common/simRosLoadModel.h"
#include "vrep_common/simRosRemoveModel.h"
#include "vrep_common/simRosSetIntegerSignal.h"
#include <string.h>
#include <ros/ros.h>

// Initialisation du nombre de plateformes F
vrepController::vrepController(){
	nShuttleF = 0;
}

// PAUSE
void vrepController::pause()
{
	client_simRosPauseSimulation.call(srv_PauseSimulation);
}

// PLAY
void vrepController::play()
{
	client_simRosStartSimulation.call(srv_StartSimulation);				
}

// SHUTTLE (pour charger une plateforme dans la simulation: plateforme aléatoire puis couleur de plateforme aléatoire)
void vrepController::shuttle(char shuttleNumber)
{
		
// Pour charger une plateforme aléatoire

		/*int shuttleRand = rand() % 6 + 49;
		char shuttleNumber = shuttleRand;
		shuttleNumber = char(shuttleNumber+16);
		ROS_INFO("Prochaine plateforme qui apparait : %c" , shuttleNumber);	
		std::string shuttleName = "models/montrac/shuttle"+std::string(&shuttleNumber,1)+".ttm";*/

// Pour charger une plateforme F avec une couleur aléatoire

		std::string shuttleName = "models/montrac/shuttle"+std::string(&shuttleNumber,1)+".ttm";
		ROS_INFO("------------ ShuttleName: %s" , shuttleName.c_str());
		srv_LoadModel.request.fileName = shuttleName;
		
		int shuttleColor = rand() % 7;
		srv_ModelColor.request.signalValue = shuttleColor;

		ROS_INFO("------------ ShuttleColor: %i" , shuttleColor);


		std::string shuttleNameComplete;
		if (nShuttleF == 0) {
			shuttleNameComplete = "Shuttle"+std::string(&shuttleNumber,1);
		}
		else{
			char aux = nShuttleF - 1 + '0';	 // transformation int -> char
			ROS_INFO("-------- Aux : %c" ,aux);
			shuttleNameComplete = "Shuttle"+std::string(&shuttleNumber,1)+"#"+ std::string(&aux,1);
		}
		ROS_INFO("------ ShuttleNameComplete: %s" , shuttleNameComplete.c_str());
		nShuttleF ++;
		
		std::string shuttleColorSignal = shuttleNameComplete + "_color" ;
		ROS_INFO("-------- ShuttleColorSignal: %s" , shuttleColorSignal.c_str());
		srv_ModelColor.request.signalName = shuttleColorSignal;

		client_simRosLoadModel.call(srv_LoadModel);	
		client_simRosModelColor.call(srv_ModelColor);			
					
}

// Chargement des modèles dans la simulation lors de son lancement
void vrepController::loadModelInit(char shuttleNumber)
{
	if(shuttleNumber>54 || shuttleNumber<48) printf(" ATTENTION, LE NUMERO DU SHUTTLE DOIT ETRE COMPRIS ENTRE 0 ET 6 \n");
	else {		
		if(shuttleNumber == 48) shuttleNumber = char(74); // SI 0 -> ShuttleZ
		shuttleNumber = char(shuttleNumber+16);
		std::string shuttleName = "models/montrac/shuttle"+std::string(&shuttleNumber)+".ttm";
		srv_LoadModelInit.request.fileName = shuttleName;	
		
		client_simRosLoadModelInit.call(srv_LoadModelInit); 
	}
}



// Suppression des modèles dans la simulation
void vrepController::removeModel(int handle)
	
//objectName (string): name of the object. If possibe, don't rely on the automatic name adjustment mechanism, and always specify the full object name, including the #: if the object is "myJoint", specify "myJoint#", if the object is "myJoint#0", specify "myJoint#0", etc.
{
        srv_RemoveModel.request.handle = handle;
        client_simRosRemoveModel.call(srv_RemoveModel);
}

//Obtention d'un handle d'objet dans la simulation
int32_t vrepController::getObjectHandle(std::string objectName)
{
	srv_GetObjectHandle.request.objectName = objectName;
	client_simRosGetObjectHandle.call(srv_GetObjectHandle);
	return srv_GetObjectHandle.response.handle;
}


// On ferme le processus vrep
void vrepController::close()
{
	system("pkill vrep");
}

void vrepController::init(ros::NodeHandle n)
{
	system("cd V-Rep/ &&(./vrep.sh -h ../Simulation.ttt &) && cd .."); // On execute VREP sans afficher la fenetre
	sleep(2);
	client_simRosLoadModelInit = n.serviceClient<vrep_common::simRosLoadModel>("/vrep/simRosLoadModel");
	client_simRosRemoveModel = n.serviceClient<vrep_common::simRosRemoveModel>("/vrep/simRosRemoveModel");
	client_simRosGetObjectHandle = n.serviceClient<vrep_common::simRosRemoveModel>("/vrep/simRosGetObjectHandle");
	client_simRosStartSimulation = n.serviceClient<vrep_common::simRosStartSimulation>("/vrep/simRosStartSimulation");	
	client_simRosPauseSimulation = n.serviceClient<vrep_common::simRosPauseSimulation>("/vrep/simRosPauseSimulation");
	client_simRosModelColor = n.serviceClient<vrep_common::simRosSetIntegerSignal>("/vrep/simRosSetIntegerSignal");
	client_simRosLoadModel = n.serviceClient<vrep_common::simRosLoadModel>("/vrep/simRosLoadModel");	


}
