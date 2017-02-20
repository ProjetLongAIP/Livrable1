cmake_minimum_required(VERSION 2.8.3)
project(postes)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  std_msgs
  shuttles
)


add_message_files(FILES Capteurs.msg Actionneurs.msg)



generate_messages(DEPENDENCIES std_msgs)
include_directories(${catkin_INCLUDE_DIRS})
include_directories(include ${OpenCV_INCLUDE_DIRS})
catkin_package(CATKIN_DEPENDS roscpp message_runtime)


add_executable(run_Poste1 src/main_poste.cpp src/Poste.cpp) 
target_link_libraries(run_Poste1 ${catkin_LIBRARIES} ${OpenCV_LIBRARIES})
add_dependencies(run_Poste1 commande_generate_messages_cpp ${${aiguillages}_EXPORTED_TARGETS})
