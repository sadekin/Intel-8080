#include "display.hpp"

constexpr uint16_t SCREEN_WIDTH = 224;
constexpr uint16_t SCREEN_HEIGHT = 256;
constexpr uint16_t MEMORY_BASE_OFFSET = 0x2400;
constexpr uint16_t VERTICAL_OFFSET_MULTIPLIER = 0x20;

Display::Display() : window(sf::VideoMode(2.5 * SCREEN_WIDTH, 2.5 * SCREEN_HEIGHT), "Space Invaders") {
    window.setFramerateLimit(60);
    texture.create(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Create and set the view to scale the original content.
    // This is done so that the window isn't too small upon initial display.
    sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    view.setSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    view.setViewport(sf::FloatRect(0, 0, 1.f, 1.f));
    window.setView(view);

    // Load the background image
    if (!backgroundTexture.loadFromFile("earth.png")) {
        std::cerr << "Failed to load background image." << std::endl;
        exit(1);
    }
    backgroundSprite.setTexture(backgroundTexture);
}

void Display::draw(Intel8080& cpu) {
    // Initialize a 224x256 image (rotated anticlockwise) filled with black color
    sf::Image image;
    image.create(224, 256, sf::Color::Black);

    // Iterate over each pixel to set its color based on CPU memory and overlay
    for (uint16_t y = 0; y < 224; y++) {     // Vertical axis
        for (int16_t x = 0; x < 256; x++) {  // Horizontal axis
            // Calculate memory offsets for pixel data
            uint16_t memoryVerticalOffset = VERTICAL_OFFSET_MULTIPLIER * y;
            uint16_t memoryHorizontalOffset = x >> 3;
            uint16_t memoryAddress = MEMORY_BASE_OFFSET + memoryVerticalOffset + memoryHorizontalOffset;
            uint8_t bitPosition = x % 8;

            // Determine if the current pixel is on or off
            bool pixelOn = (cpu.read(memoryAddress) & (1 << bitPosition)) != 0;

            // Retrieve the overlay color for the current pixel if the pixel is on.
            // Note: x and y are interchanged as the parameters due to rotation.
            sf::Color pixelColor = pixelOn ? getOverlayColor(y, x) : sf::Color::Transparent;

            // Set the pixel color with coordinates rotated counter-clockwise
            image.setPixel(y, SCREEN_HEIGHT - x - 1, pixelColor);;
        }
    }

    // Convert image to texture and create sprite for rendering
    texture.update(image);
    sf::Sprite sprite(texture);
    window.clear();
    window.draw(backgroundSprite);
    window.draw(sprite);

    // Display on screen what has been rendered to the window so far
    window.display();
}

// The screen is 256 * 224 pixels, and is rotated anti-clockwise.
// These are the overlay dimensions:
// ,_______________________________.
// |WHITE            ^             |
// |                32             |
// |                 v             |
// |-------------------------------|
// |RED              ^             |
// |                32             |
// |                 v             |
// |-------------------------------|
// |WHITE                          |
// |         < 224 >               |
// |                               |
// |                 ^             |
// |                120            |
// |                 v             |
// |                               |
// |                               |
// |                               |
// |-------------------------------|
// |GREEN                          |
// | ^                  ^          |
// |56        ^        56          |
// | v       72         v          |
// |_____      v     ______________|
// |  ^  |          | ^            |
// |<16> |  < 118 > |16   < 122 >  |
// |  v  |          | v            |
// |WHITE|          |         WHITE|
// `-------------------------------'
// Image source: https://github.com/superzazu/invaders/blob/master/src/invaders.c
sf::Color Display::getOverlayColor(uint8_t x, uint8_t y) {
    // Define overlay colors based on vertical and horizontal positions
    // Top overlay region - White
    if (y >= SCREEN_HEIGHT - 32) return sf::Color::White;

    // Second region from the top - Red
    if (y >= (SCREEN_HEIGHT - 32 - 32)) return sf::Color::Red;

    // Middle region - White
    if (y >= (SCREEN_HEIGHT - 32 - 32 - 120)) return sf::Color::White;

    // Fourth region from the top - Green
    if (y >= (SCREEN_HEIGHT - 32 - 32 - 120 - 56)) return sf::Color::Green;

    // Bottom region split into three parts
    // Leftmost part - White
    if (x <= 16) return sf::Color::White;

    // Middle part - Green
    if (x <= (16 + 118)) return sf::Color::Green;

    // Rightmost part - White
    return sf::Color::White;
}
