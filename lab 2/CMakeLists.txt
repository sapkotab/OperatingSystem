cmake_minimum_required(VERSION 3.8)
project(lab2)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES myscheduler.cpp process.cpp process.h readyQueue.cpp readyQueue.h)
add_executable(lab2 ${SOURCE_FILES})