cmake_minimum_required(VERSION 3.26)
project(Intel_8080)

set(CMAKE_CXX_STANDARD 20)

add_executable(Intel_8080 main.cpp
        cpu.hpp
        cpu.cpp
        opcodes.cpp
        disassemble.cpp
        memory.hpp
        memory.cpp
        io.hpp
        io.cpp
        display.hpp
        display.cpp
        platform.hpp
        platform.cpp
        utils.cpp
        audio.hpp
        audio.cpp)

# SFML
find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)

target_include_directories(Intel_8080 PRIVATE ${SFML_INCLUDE_DIR}})
target_link_libraries(Intel_8080 PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)