#include "platform.hpp"

Platform::Platform() : prevTemp(), currTemp() {
    // Initialize the CPU, Display, and Audio for the emulator
    cpu = new Intel8080();
    display = new Display();
    audio = new Audio();

    // Load the Space Invaders game file into memory
    bool loadSuccess = cpu->load("invaders", 0);
    if (!loadSuccess) {
        std::cout << "Could not load file." << std::endl;
        exit(1); // Exit if the file loading fails
    } else {
        std::cout << "Successfully loaded file." << std::endl;
    }

    // Load sound files
    audio->load("explosion"    , "../invaders-audio/explosion.wav"    );
    audio->load("fastinvader1" , "../invaders-audio/fastinvader1.wav" );
    audio->load("fastinvader2" , "../invaders-audio/fastinvader2.wav" );
    audio->load("fastinvader3" , "../invaders-audio/fastinvader3.wav" );
    audio->load("fastinvader4" , "../invaders-audio/fastinvader4.wav" );
    audio->load("invaderkilled", "../invaders-audio/invaderkilled.wav");
    audio->load("shoot"        , "../invaders-audio/shoot.wav"        );
    audio->load("ufo_highpitch", "../invaders-audio/ufo_highpitch.wav");
    audio->load("ufo_lowpitch" , "../invaders-audio/ufo_lowpitch.wav" );
}


// This function is the heart of the emulation platform. It sets up a system clock to keep track
// of the emulation's timing and enters a loop that keeps the emulation running. Inside this loop,
// we're balancing time to ensure a smooth frame rate, approximately 60 fps.
//
// Space Invaders originally ran on hardware that operated at a clock speed of approximately 2 MHz
// (2,000,000 cycles per second). The game was designed to maintain a consistent frame rate,
// which is crucial for gameplay and visual performance:
//
// At 60 frames per second, each frame lasts for about 1/60th of a second.
// Therefore, the number of cycles per frame is:
// (2,000,000 cycles/second) / (60 fps) ≈ 33,333 cycles/frame.
// Since each frame is divided into two halves, each half would be approximately
// 33,333 / 2 ≈ 16,666 cycles/half-frame.
//
// In each cycle, we're executing CPU instructions, handling screen interrupts for half-screen and
// full-screen updates, and updating the display. Additionally, this loop takes care of audio
// processing and user input handling, more or less ensuring that our emulation responds just
// like the original hardware would.
void Platform::run() {
    // Initialize system clock for tracking emulation time
    sf::Clock systemTimeClock;
    sf::Time elapsedTime = sf::Time::Zero;
    systemTimeClock.restart();

    std::cout << "Commencing emulation..." << std::endl;
    while (display->window.isOpen()) {
        elapsedTime += systemTimeClock.restart();   // update elapsed time

        // Define frame duration (approximately 60 frames per second)
        const int timePerFrameMs = 17; // 1000 ms / 60 fps ≈ 17 ms

        // Process CPU execution and interrupts at each frame
        if (elapsedTime.asMilliseconds() > timePerFrameMs) {
            elapsedTime = sf::Time::Zero;   // reset elapsed time for next frame

            cpu->execute(16666);            // execute CPU cycles for half a screen update
            cpu->interrupt(1);              // generate half-screen interrupt (RST 1)

            cpu->execute(16666);            // execute CPU cycles for the remaining half
            cpu->interrupt(2);              // generate full-screen interrupt (RST 2)

            display->draw(*cpu);            // render and display window
        }

        // Handle audio and user input
        handleAudio(*cpu->ioPorts);
        handleInput(display->window, *cpu->ioPorts);
    }
    std::cout << "Quit successfully." << std::endl;
}

// Processes user input events such as keyboard presses and releases,
// and updates the game state accordingly.
//
// Port 1
// bit 0 = CREDIT (1 if deposit)
// bit 1 = 2P start (1 if pressed)
// bit 2 = 1P start (1 if pressed)
// bit 3 = Always 1
// bit 4 = 1P shot (1 if pressed)
// bit 5 = 1P left (1 if pressed)
// bit 6 = 1P right (1 if pressed)
// bit 7 = Not connected
// Source: http://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html
void Platform::handleInput(sf::RenderWindow& gameWindow, IOPorts& gamePorts) {
    using sf::Keyboard;

    sf::Event inputEvent;
    while (gameWindow.pollEvent(inputEvent)) {
        // Handle window close event
        if (inputEvent.type == sf::Event::Closed) gameWindow.close();

        // Handle key press events
        if (inputEvent.type == sf::Event::KeyPressed) {
            switch (inputEvent.key.code) {
                case Keyboard::Escape:  gameWindow.close();               break; // Quit
                case Keyboard::C:       gamePorts.setInPort1Bit(0, true); break; // Coin inserted
                case Keyboard::Num2:    gamePorts.setInPort1Bit(1, true); break; // Player 2 Start
                case Keyboard::Enter:   gamePorts.setInPort1Bit(2, true); break; // Player 1 Start
                case Keyboard::Space:   gamePorts.setInPort1Bit(4, true); break; // Shoot button
                case Keyboard::Left:    gamePorts.setInPort1Bit(5, true); break; // Move left
                case Keyboard::Right:   gamePorts.setInPort1Bit(6, true); break; // Move right
                default:                                                  break; // Do nothing
            }
        }

        // Handle key release events
        if (inputEvent.type == sf::Event::KeyReleased) {
            switch (inputEvent.key.code) {
                case Keyboard::C:       gamePorts.setInPort1Bit(0, false); break; // Coin released
                case Keyboard::Num2:    gamePorts.setInPort1Bit(1, false); break; // Player 2 Start released
                case Keyboard::Enter:   gamePorts.setInPort1Bit(2, false); break; // Player 1 Start released
                case Keyboard::Space:   gamePorts.setInPort1Bit(4, false); break; // Shoot button released
                case Keyboard::Left:    gamePorts.setInPort1Bit(5, false); break; // Move left released
                case Keyboard::Right:   gamePorts.setInPort1Bit(6, false); break; // Move right released
                default:                                                   break; // Do nothing
            }
        }
    }
}

// Manages audio playback based on the game's state, playing and stopping sounds in response to game events.
//
// Port 3: (discrete sounds)
// bit 0=UFO (repeats)
// bit 1=Shot
// bit 2=Flash (player die)
// bit 3=Invader die
// bit 4=Extended play
// bit 5= AMP enable
// bit 6= NC (not wired)
// bit 7= NC (not wired)
// Port 4: (discrete sounds)
// bit 0-7 shift data (LSB on 1st write, MSB on 2nd)
//
// Port 5:
// bit 0=Fleet movement 1
// bit 1=Fleet movement 2
// bit 2=Fleet movement 3
// bit 3=Fleet movement 4
// bit 4=UFO Hit
// bit 5= NC (Cocktail mode control ... to flip screen)
// bit 6= NC (not wired)
// bit 7= NC (not wired)
// Source: http://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html
void Platform::handleAudio(IOPorts& gamePorts) {
    while (!gamePorts.playNext.empty()) {
        if (get<0>(gamePorts.playNext.front()) == 3) {
            // Play sounds based on IOPorts::outPort3
            prevTemp = get<1>(gamePorts.playNext.front());
            currTemp = get<2>(gamePorts.playNext.front());

            if (!(prevTemp & (1 << 0)) && (currTemp & (1 << 0)))        audio->play("ufo_highpitch");
            else if ((prevTemp & (1 << 0)) && !(currTemp & (1 << 0)))   audio->stop("ufo_highpitch");

            if (!(prevTemp & (1 << 1)) && (currTemp & (1 << 1)))        audio->play("shoot");
            else if ((prevTemp & (1 << 1)) && !(currTemp & (1 << 1)))   audio->stop("shoot");

            if (!(prevTemp & (1 << 2)) && (currTemp & (1 << 2)))        audio->play("explosion");
            else if ((prevTemp & (1 << 2)) && !(currTemp & (1 << 2)))   audio->stop("explosion");

            if (!(prevTemp & (1 << 3)) && (currTemp & (1 << 3)))        audio->play("invaderkilled");
            else if ((prevTemp & (1 << 3)) && !(currTemp & (1 << 3)))   audio->stop("invaderkilled");
        } else {
            // Play sounds based on IOPorts::outPort5
            prevTemp = get<1>(gamePorts.playNext.front());
            currTemp = get<2>(gamePorts.playNext.front());

            if (!(prevTemp & (1 << 0)) && (currTemp & (1 << 0)))        audio->play("fastinvader1");
            else if ((prevTemp & (1 << 0)) && !(currTemp & (1 << 0)))   audio->stop("fastinvader1");

            if (!(prevTemp & (1 << 1)) && (currTemp & (1 << 1)))        audio->play("fastinvader2");
            else if ((prevTemp & (1 << 1)) && !(currTemp & (1 << 1)))   audio->stop("fastinvader2");

            if (!(prevTemp & (1 << 2)) && (currTemp & (1 << 2)))        audio->play("fastinvader3");
            else if ((prevTemp & (1 << 2)) && !(currTemp & (1 << 2)))   audio->stop("fastinvader3");

            if (!(prevTemp & (1 << 3)) && (currTemp & (1 << 3)))        audio->play("fastinvader4");
            else if ((prevTemp & (1 << 3)) && !(currTemp & (1 << 3)))   audio->stop("fastinvader4");

            if (!(prevTemp & (1 << 4)) && (currTemp & (1 << 4)))        audio->play("ufo_lowpitch");
            else if ((prevTemp & (1 << 4)) && !(currTemp & (1 << 4)))   audio->stop("ufo_lowpitch");
        }
        gamePorts.playNext.pop();
    }
}