cmake_minimum_required(VERSION 3.5)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -pthread")

project(zws VERSION "1.0.0")

# file(GLOB_RECURSE ALL_SRC src/*.cpp)
# foreach (F ${ALL_SRC})
#     set(SOURCES ${SOURCE} ${F})
# endforeach (F)

set(SOURCES 
    src/main.cpp 
    src/WebServer.cpp src/WebServer.h 
    src/TaskPool.cpp src/TaskPool.h 
    src/Singleton.h
    src/common.h
)

include_directories(include)

add_executable(${PROJECT_NAME} ${SOURCES})

