cmake_minimum_required(VERSION 3.26)
project(Intel_8080)

set(CMAKE_CXX_STANDARD 20)

add_executable(Intel_8080 main.cpp
        cpu.h
        cpu.cpp
        opcodes.cpp)
