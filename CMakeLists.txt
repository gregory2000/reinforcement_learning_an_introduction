cmake_minimum_required(VERSION 3.8)
project(reinforcement_learning_an_introduction)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES chapter03/grid-world.cpp)
add_executable(reinforcement_learning_an_introduction ${SOURCE_FILES})