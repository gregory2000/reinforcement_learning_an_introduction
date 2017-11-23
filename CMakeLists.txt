cmake_minimum_required(VERSION 3.8)
project(reinforcement_learning_an_introduction)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES ai_poole/chap04/main.cpp ai_poole/chap04/example49.cpp ai_poole/chap04/example49.h)
add_executable(reinforcement_learning_an_introduction ${SOURCE_FILES})