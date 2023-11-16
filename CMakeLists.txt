cmake_minimum_required(VERSION 3.26)
project(Intel_8080)

set(CMAKE_CXX_STANDARD 20)

add_executable(Intel_8080 src/main.cpp
        src/cpu.hpp
        src/cpu.cpp
        src/opcodes.cpp
        src/disassemble.cpp
        src/memory.hpp
        src/memory.cpp
        src/io.hpp
        src/io.cpp
        src/display.hpp
        src/display.cpp
        src/platform.hpp
        src/platform.cpp
        src/utils.cpp
        src/audio.hpp
        src/audio.cpp)

# SFML
find_package(SFML 2.6 COMPONENTS system window graphics network audio REQUIRED)

target_include_directories(Intel_8080 PRIVATE ${SFML_INCLUDE_DIR}})
target_link_libraries(Intel_8080 PRIVATE sfml-system sfml-window sfml-graphics sfml-audio sfml-network)