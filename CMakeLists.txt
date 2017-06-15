cmake_minimum_required(VERSION 3.7)
project(ProjectLO41)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

set(SOURCE_FILES main.c Drone.h Drone.c Vaisseau.c Garage.c List.c)
add_executable(ProjectLO41 ${SOURCE_FILES})
