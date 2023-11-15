#include "display.hpp"

Display::Display() {
    // Initialize SFML context settings with antialiasing
    sf::ContextSettings graphicsSettings;
    graphicsSettings.antialiasingLevel = 8;

    // Create a new SFML window for rendering
    // The window dimensions are set to 224x256 pixels to match the Space Invaders screen
    // The title of the window is set to "I8080 Emulator"
    window = new sf::RenderWindow(
            sf::VideoMode(224, 256),  // Window size (width, height)
            "Space Invaders",         // Window title
            sf::Style::Default,       // Window style (default style)
            graphicsSettings          // Context settings (with antialiasing)
    );

    // Set the frame rate limit of the window to 60 frames per second
    window->setFramerateLimit(60);

    // Load the background image
    if (!backgroundTexture.loadFromFile("palebluedot.png")) {
        std::cout << "Failed to load background image." << std::endl;
        exit(1); // Exit if the image loading fails
    }

    // Set up the sprite to use this texture
    backgroundSprite.setTexture(backgroundTexture);
}


Display::~Display() { delete window; }


void Display::Render(Intel8080& cpu) {
    // Initialize a 224x256 image (rotated anticlockwise) filled with black color
    sf::Image image;
    image.create(224, 256, sf::Color::Black);

    // Iterate over each pixel to set its color based on CPU memory and overlay
    for(uint16_t y = 0; y < 224; y++) {     // Vertical axis
        for(int16_t x = 0; x < 256; x++) {  // Horizontal axis
            // Calculate memory offsets for pixel data
            uint16_t memoryBaseOffset = 0x2400;
            uint16_t memoryVerticalOffset = 0x20 * y;
            uint16_t memoryHorizontalOffset = (x >> 3);
            uint16_t memoryAddress = memoryBaseOffset + memoryVerticalOffset + memoryHorizontalOffset;
            uint8_t  bitPosition = x % 8;

            // Determine if the current pixel is on or off
            bool pixelOn = (cpu.read(memoryAddress) & (1 << bitPosition)) != 0;
            sf::Color pixelColor;

            // Retrieve the overlay color for the current pixel if the pixel is on
            pixelColor = pixelOn ? getOverlayColor(y, x) : sf::Color::Black;

            // Set the pixel color with coordinates rotated counter-clockwise
            image.setPixel(y, 256 - x - 1, pixelColor);
        }
    }

    // Convert image to texture and create sprite for rendering
    sf::Texture texture;
    texture.loadFromImage(image);
    sf::Sprite sprite;
    sprite.setPosition(0, 0);
    sprite.setTexture(texture, true);

    // Clear the window with a black color (optional if background covers entire window)
    window->clear(sf::Color::Black);

    // Draw the background sprite
    window->draw(backgroundSprite);

    // Draw the sprite (rendered image) in the window
    window->draw(sprite);
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
    if (y >= 256 - 32) return sf::Color::White;

    // Second region from the top - Red
    if (y >= (256 - 32 - 32)) return sf::Color::Red;

    // Middle region - White
    if (y >= (256 - 32 - 32 - 120)) return sf::Color::White;

    // Fourth region from the top - Green
    if (y >= (256 - 32 - 32 - 120 - 56)) return sf::Color::Green;

    // Bottom region split into three parts
    // Leftmost part - White
    if (x <= 16) return sf::Color::White;

    // Middle part - Green
    if (x <= (16 + 118)) return sf::Color::Green;

    // Rightmost part - White
    return sf::Color::White;
}

