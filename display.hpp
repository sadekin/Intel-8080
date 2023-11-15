#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "cpu.hpp"

class Display {

public:
    Display();
    ~Display();

    void Render(Intel8080& cpu);
    sf::Color getOverlayColor(uint8_t x, uint8_t y);

    sf::RenderWindow* window;
    sf::Texture backgroundTexture; // Texture for the background image
    sf::Sprite backgroundSprite;   // Sprite for drawing the background image
};