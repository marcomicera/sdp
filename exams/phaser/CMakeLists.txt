cmake_minimum_required(VERSION 3.9)
project(phaser)
set(CMAKE_CXX_STANDARD 11)
add_executable(${PROJECT_NAME} phaser.cpp)
SET(CMAKE_CXX_FLAGS -pthread)
