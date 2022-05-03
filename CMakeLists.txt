cmake_minimum_required(VERSION 3.22)
project(dictionary)

set(CMAKE_CXX_STANDARD 14)

add_executable(dictionary main.cpp CLI.cpp CLI.h)
