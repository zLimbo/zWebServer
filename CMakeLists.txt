message("\n===== [CMakeLists.txt] start")
cmake_minimum_required(VERSION 3.5)
set(CMAKE_TOOLCHAIN_FILE "/home/z/z/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS "-g -Wall -pthread")

project(zws VERSION "1.0.0")

# file(GLOB_RECURSE ALL_SRC src/*.cpp)
# foreach (F ${ALL_SRC})
#     set(SOURCES ${SOURCE} ${F})
# endforeach (F)

# set(SOURCES 
#     src/main.cpp 
#     src/WebServer.cpp src/WebServer.h 
#     src/TaskPool.cpp src/TaskPool.h 
#     src/Singleton.h
#     src/common.h
# )

message("PROJECT_NAME: " ${PROJECT_NAME})
message("PROJECT_SOURCE_DIR: " ${PROJECT_SOURCE_DIR})
message("PROJECT_BINARY_DIR: " ${PROJECT_BINARY_DIR})
message("CMAKE_CXX_STANDARD: " ${CMAKE_CXX_STANDARD})

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(study)

message("===== [CMakeLists.txt] end\n")