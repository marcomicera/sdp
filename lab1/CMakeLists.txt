cmake_minimum_required(VERSION 3.9)
project(lab1)
set(CMAKE_CXX_STANDARD 11)
add_executable(${PROJECT_NAME} StringBuffer.h StringBuffer.cpp StringBufferTest.cpp)
