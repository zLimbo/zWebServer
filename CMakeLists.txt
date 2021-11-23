cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -pthread")

project(zws VERSION "1.0.0")

set(SOURCES
    src/Task.cpp src/Task.h
    src/WebServer.cpp src/WebServer.h 
    src/main.cpp 
)

include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES})

