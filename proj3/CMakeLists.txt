cmake_minimum_required(VERSION 3.6)
project(proj3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR})

SET( EIGEN3_INCLUDE_DIR . )

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

add_executable(proj3 ${SOURCE_FILES} Pipeline.cpp Pipeline.h)