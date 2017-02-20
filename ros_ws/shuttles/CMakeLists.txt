cmake_minimum_required(VERSION 2.8.3)
project(shuttles)

find_package(catkin REQUIRED)
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs 
  genmsg
  message_generation
)

include_directories(${catkin_INCLUDE_DIRS})
set(EXECUTABLE_OUTPUT_PATH ../../../)


add_message_files(
	FILES
	msgShuttleChange.msg
	msgShuttleCreate.msg
	)

add_service_files(
	FILES
	srvGetShuttleStatus.srv
	srvGetEmptyShuttles.srv
	)

generate_messages( DEPENDENCIES std_msgs)
catkin_package( CATKIN_DEPENDS message_runtime) 

add_executable(shuttletest src/mainCommandeShuttle.cpp src/shuttle.cpp src/shuttleManager.cpp)  
target_link_libraries(shuttletest ${catkin_LIBRARIES})
add_dependencies(shuttletest shuttletest_generate_messages_cpp)
