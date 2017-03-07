# Projet Long ENSEEIHT - GEA Department - 2017 - projetlong.aip@gmail.com

> Command of a transport network and it simulation & Interaction of the transport network 

## Authors
- __Jean Baptiste BLANC-ROUCHOSSE__ jbblancrouchosse@gmail.com
- __Claire DELAGE__ clr.delage@gmail.com
- __Enrique MALDONADO__ enriquemaldonado11@gmail.com
- __Maxime MAURIN__ maxime-maurin@hotmail.fr
- __Aurélie QUINTANA__ quintana.aurelie@gmail.com
- __Celine TOME__ tome.cece@gmail.com

## Co-authors
- __Bruno DATO__ -- bruno.dato.meneses@gmail.com
- __Abdellah ELGOURAIN__ -- elgourain@gmail.com
- __Evgeny SHULGA__ -- hlebovod@mail.ru

Special thanks to them for their previous work on this project

# TO DO

## Install ROS 

1. **To install the latest version of ROS, go to http://wiki.ros.org/ROS/Installation and install the appropiate version for your platform**

## Install Modbus

2. **Open a terminal and write**

	`
	$ sudo apt-get install libmodbus-dev
	`


## Create the workspace


3. **Download the content of this GitHub repertory and place it in folder for example 'TER_files'**

4. **Chose a name for your workspace. For this example will call this folder 'Working_Folder_TER'**
	- This folder will be in the 'home' repertory of Linux and it will contain all the files to execute the simulation properly
	
5. **In ~/.bashrc, add these lines :**
	- Open a terminal and write the command gedit .bashrc
	- At the end of the file add the following line replacing *ROS_version* by the ROS version you installed (e.g. indigo) and [Working_Folder] with the name of your workspace (e.g. Working_Folder_TER). See the example below
	
	```
	source /opt/ros/*ROS_version*/setup.bash
	export VREP_ROOT_DIR=~/[Working_Folder]/V-Rep/
	Example:
	source /opt/ros/indigo/setup.bash
	export VREP_ROOT_DIR=~/Working_Folder_TER/V-Rep/
	```
	
6. **In ~/.bashrc, add these lines :**
	- Also add the following lines by replacing [Working_Folder] with the name of your workspace. See the example below.
	- This line will avoid you to always write source devel/setup.bash every time you open a new terminal.
	- If you are using another ros workspace in parallel, don't add this line in ~/.bashrc, you will have to write the command "source devel/setup.bash" every time that you open a new terminal and that you are in [Working_Folder]/ros_ws
	
	
	```
	source ~/[Working_Folder]/ros_ws/devel/setup.bash
	Example:
	source ~/Working_Folder_TER/ros_ws/devel/setup.bash
	```
	
7. **Open a terminal and place you in the folder 'TER_files', where you placed the content of the GitHub**
        - Remember you can change the directory with the commande cd. 
	- Once you are in the indicated folder run the following command by replacing [Working_Folder] with the name of your workspace. See the example below. 
	
	```
	./setup [Working_Folder]
	Example:
	./setup Working_Folder_TER
	```
	
8. **'setup' will create and organize your workinspace**
	- At the end of 'setup' execution you should get the following message

	````
	-----------------------------------------------------------
	      Final catkin_make done in 'Working_Folder_TER
	          The simuation is ready to be launched
		  (...)
	 ----------------------------------------------------------
	```
	
## Run the simulation 

9.  **In a terminal, write the command "roscore" if you haven't done it yet to launch the local master**

10. **If you are using the real transport network, ignore this step. To run the simulation, open a new terminal and place you in  ~/[Working_Folder]**

	- Type the following command to launch the simulation. It will launch the default simulation file (Simulation.ttt in the ~/[Working_Folder]/sim/) :
	```
	$ ./launch.sh
	```
	- If you want to launch a specific simulation file, type:
	```
	$ ./launch.sh [Simulation_File]
	```
	- If ever the simulation doesn't lauch successfully read this list of possible errors:
		- The image of the simulation does not appear: Make sure you put the correct name of the simulation file.
		- Nothing works at all: Make sure the ROS master has been launched.
	- If any of the previous instructions solved the problem, stop the simulation by typing CTRL+C and try a second time the previous commands before restarting all the steps.
	
 
11. **Click on the 'Play' button to start the simulation**
	- The initial mode of the simulation is standby. You will have to choose a mode (Mode button) to be able to start.
  	- It is possible to insert shuttles with the 'Shuttle' button, if you are in 'Manual' or 'Random' mode. 
  	- Make sure you wait enough time between two shuttles. If you do not take this precaution shuttles will be accumulated in the starting point and eventually fall off the rails.
  
  	- If you wish to launch the auto mode, you will have to correctly configure the ProductConfiguration.config file (in ~/[Working_Folder]/). Follow the instructions in the heading of the file to know how to modify it:




