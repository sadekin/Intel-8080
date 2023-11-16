#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "cpu.hpp"

class Display {

public:
    Display();
    void draw(Intel8080& cpu);

public:
    sf::RenderWindow window;

private:
    sf::Texture texture;            // Texture for game graphics
    sf::Texture backgroundTexture;  // Texture for the background image
    sf::Sprite  backgroundSprite;   // Sprite for the background image

    sf::Color getOverlayColor(uint8_t x, uint8_t y);
};