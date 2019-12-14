cmake_minimum_required(VERSION 3.9)
project(lab5)
set(CMAKE_CXX_STANDARD 11)
add_executable(${PROJECT_NAME} message_loop.h message_loop.cpp message_loop_test.cpp)
