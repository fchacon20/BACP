cmake_minimum_required(VERSION 3.7)
project(IAA)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Course.cpp Course.h Period.cpp Period.h functions.cpp functions.h)
add_executable(IAA ${SOURCE_FILES})