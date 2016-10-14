cmake_minimum_required(VERSION 3.6)
project(proj3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

AUX_SOURCE_DIRECTORY(. SOURCE_FILES)

add_executable(proj3 ${SOURCE_FILES})