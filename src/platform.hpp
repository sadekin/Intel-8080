#pragma once

#include "cpu.hpp"
#include "display.hpp"
#include "audio.hpp"

class Platform {
public:
    Platform();
    void run();

private:
    Intel8080*   cpu;
    Display*     display;
    Audio*       audio;

    void handleInput(sf::RenderWindow& gameWindow, IOPorts& gamePorts);
    void handleAudio(IOPorts& gamePorts);

private:
    // Convenience variables for handling audio
    uint8_t prevTemp, currTemp;
};