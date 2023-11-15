#include "platform.hpp"

Platform::Platform() : display() {
    // Initialize the CPU for the emulator
    cpu = new Intel8080();

    // Load the Space Invaders game file into memory
    bool loadSuccess = cpu->memory->load("invaders", 0);

    // Check if the game file was loaded successfully
    if (!loadSuccess) {
        std::cout << "Could not load file." << std::endl;
        exit(1); // Exit if the file loading fails
    } else {
        std::cout << "Successfully loaded file." << std::endl;
    }
}

Platform::~Platform() { delete cpu; }

void Platform::Run() {
    // Initialize system clock for tracking emulation time
    sf::Clock systemTimeClock;
    sf::Time elapsedTime = sf::Time::Zero;
    systemTimeClock.restart();

    // Start the main emulation loop
    std::cout << "Starting emulation..." << std::endl;
    while(display.window->isOpen()) {
        // Update elapsed time
        elapsedTime += systemTimeClock.restart();

        // Define frame duration (approximately 60 frames per second)
        const int timePerFrameMs = 17; // 1000 ms / 60 fps ≈ 17 ms

        // Process CPU execution and interrupts at each frame
        if (elapsedTime.asMilliseconds() > timePerFrameMs) {
            // Reset elapsed time for next frame
            elapsedTime = sf::Time::Zero;

            // execute CPU cycles for half a screen update
            cpu->execute(16666);
            // Generate half-screen interrupt
            cpu->interrupt(1); // RST 1

            // execute CPU cycles for the remaining half
            cpu->execute(16666);
            // Generate full-screen interrupt
            cpu->interrupt(2); // RST 2

            // Render the current state and display it
            display.Render(*cpu);
            display.window->display();
        }

        // Handle user input for this frame
        handleInput(*display.window, *cpu->ioPorts);

        // Uncomment to update sound devices if needed
        // m_devices->UpdateSounds();
    }
}

void Platform::handleInput(sf::RenderWindow& gameWindow, IOPorts& gamePorts) {
    // Alias for easier access to keyboard keys
    using sf::Keyboard;

    sf::Event inputEvent;
    while (gameWindow.pollEvent(inputEvent)) {
        // Handle window close event
        if (inputEvent.type == sf::Event::Closed) {
            gameWindow.close();
        }

        // Handle key press events
        if (inputEvent.type == sf::Event::KeyPressed) {
            switch (inputEvent.key.code) {
                case Keyboard::C:         gamePorts.inPort1 |= (1 << 0); break; // Coin inserted
                case Keyboard::Num2:      gamePorts.inPort1 |= (1 << 1); break; // Player 2 Start
                case Keyboard::Enter:     gamePorts.inPort1 |= (1 << 2); break; // Player 1 Start
                case Keyboard::Space:     gamePorts.inPort1 |= (1 << 4); break; // Shoot button
                case Keyboard::Left:      gamePorts.inPort1 |= (1 << 5); break; // Move left
                case Keyboard::Right:     gamePorts.inPort1 |= (1 << 6); break; // Move right
                default:                                                 break;
            }
        }

        // Handle key release events
        if (inputEvent.type == sf::Event::KeyReleased) {
            switch (inputEvent.key.code) {
                case Keyboard::C:         gamePorts.inPort1 &= ~(1 << 0); break; // Coin released
                case Keyboard::Num2:      gamePorts.inPort1 &= ~(1 << 1); break; // Player 2 Start released
                case Keyboard::Enter:     gamePorts.inPort1 &= ~(1 << 2); break; // Player 1 Start released
                case Keyboard::Space:     gamePorts.inPort1 &= ~(1 << 4); break; // Shoot button released
                case Keyboard::Left:      gamePorts.inPort1 &= ~(1 << 5); break; // Move left released
                case Keyboard::Right:     gamePorts.inPort1 &= ~(1 << 6); break; // Move right released
                default:                                                  break;
            }
        }
    }
}