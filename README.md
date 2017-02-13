# Projet Long ENSEEIHT - GEA Department - 2017 

> Command of a transport network and it simulation & Interaction of the transport network 

## Authors
- __Jean Baptiste BLANC-ROUCHOSSE__ -- @gmail.com
- __Claire DELAGE__ -- @gmail.com
- __Enrique MALDONADO__ -- @gmail.com
- __Maxime MAURIN__ -- @gmail.com
- __Aurélie QUINTANA__ -- @gmail.com
- __Celine TOME__ -- @gmail.com

## Co-authors
- __Bruno DATO__ -- bruno.dato.meneses@gmail.com
- __Abdellah ELGOURAIN__ -- elgourain@gmail.com
- __Evgeny SHULGA__ -- hlebovod@mail.ru
	-Special thanks to them for their previous work on this project

# TO DO

## Install ROS 

1. **To install the latest version of ROS, go to http://wiki.ros.org/ROS/Installation and install the appropiate version for your platform**

## Install Modbus

2. **Open a terminal and write**

	- $ sudo apt-get install libmodbus-dev 


## Create the workspace


3. **In ~/.../ Create a folder named for example "Working_Folder_1", it will contain your workspace**
	- In this folder, copy the folders *img* and *V-REP* and the file *Simulation.ttt* (you can find them in this GitHub repertory ). Every time you find "/.../" you have to replace this by the path that leads to the folder or the file we are talking about on YOUR computer)


4. **In ~/.bashrc, add these lines :**
	- source /opt/ros/*ROS_version*/setup.bash (source /opt/ros/jade/setup.bash for example if your installed ROS JADE)
	- export VREP_ROOT_DIR=~/ ... /Working_Folder_1/V-Rep/

5. **Create the workspace ros_ws : in ~/.../Working_Folder_1 :**
	- $ mkdir -p ros_ws/src
	- $ cd ros_ws/src
	- $ catkin_init_workspace
	- $ cd ..
	- $ catkin_make

6. **In ~/.bashrc, add this line :**
	- source ~/ ... /Working_Folder_1/ros_ws/devel/setup.bash (this line will avoid you to always write source devel/setup.bash every time you open a new terminal)
	- if you are using another ros workspace in parallel, don't add this line in ~/.bashrc, you will have to write the command "source devel/setup.bash" every time that you open a new terminal and that you are in /ros_ws

## Add the packages

7. **in ~/.../Working_Folder_1/ros_ws/src**
	- copy the package *automates* of ~/.../ros_ws/src (which is the folder *automates*)

8. **In ~/.../Working_Folder_1/ros_ws/ :**
	- $ catkin_make
	
  - A red line should appear which means that the executable have been created.
	- repeat these steps for each of the packages that have an executable (communication, commande_locale and commande)
	- It's recommended to add each package one by one, to do not copy all the package at once to avoid errors.

	- if you encounter errors like : no such file or directory #include "automates/Entrees.h" , go to ~/.../Working_Folder_1/ros_ws/devel/include/automates (for this file), copy Entrees.h and paste it in the folder "src" of the package that is causing problem
	- then in every programme of the package, every time you find #include "automates/Entrees.h" replace it by #include "Entrees.h"

 	- if you have another of this errors, you can find all the files.h that you need in ~/.../Working_Folder_Montrac/ros_ws/devel/include/


## Run the simulation 

9.  **In a terminal, write the command "roscore" if you haven't done it yet to launch the local master**

10. **If you are using the real transport network, ignore this step. To run the simulation, in a new terminal, in ~/ ... /Working_Folder_1**

	- You could do either: 
     $ ./simulation 1 2 5 6 (or any number of shuttle between 1 and 6 seperated with a space)
     If you wish to have initial shuttles on the rails.
     
     $ ./simulation 
     If you wish an empty initial cell. You will be able to insert shuttles with the 'Shuttle' button.
     
	- if ever the simulation doesn't lauch successfully stop it by doing CTRL+C try a second time "$ ./simulation 1 2 5 6" before restarting all the steps
 
11. **Click on the 'Play' button to start the simulation**
 
  - It is possible to insert shuttles with the 'Shuttle' button. 
  - Make sure you wait enough time between two shuttles. If you do not take this precaution shuttles will be accumulated in the starting point and eventually fall off the rails.
