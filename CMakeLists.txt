cmake_minimum_required(VERSION 3.22)
project(Dictionary)

set(CMAKE_CXX_STANDARD 14)

add_executable(Wordlebook main.cpp Shell.cpp Shell.h Woordenboek.cpp Woordenboek.h NFA.cpp NFA.h DFA.cpp DFA.h json.hpp)
