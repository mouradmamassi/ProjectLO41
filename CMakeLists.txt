cmake_minimum_required(VERSION 3.7)
project(ProjectLO41)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -lrt")
#set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -lrt")

set(SOURCE_FILES main.c Drone.h Drone.c Vaisseau.c Garage.c)
add_executable(ProjectLO41 ${SOURCE_FILES})
