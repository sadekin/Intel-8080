#pragma once

#include "cpu.hpp"
#include "display.hpp"

class Platform {
public:
    Platform();
    ~Platform();
    void Run();

public:
    Intel8080* cpu;
    Display display;

    void handleInput(sf::RenderWindow& window, IOPorts& ports);
};