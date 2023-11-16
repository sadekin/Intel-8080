#pragma once

#include <cstdint>
#include <queue>
#include <tuple>

class IOPorts {
public:
    uint8_t read(uint8_t port);
    void    write(uint8_t port, uint8_t data);

    void    setInPort1Bit(uint8_t bitNum, bool v);  // for Player 1 input handling

    // Using a queue to keep track of changes between prev and
    // curr outPorts. This was a hacky fix for some audio issues
    // where there was sometimes delay and other times no
    // sound being registered at all.
    // Why does it work? It could be because it addresses
    // a potential issue with concurrent modifications of the
    // ports while running the emulation.
    std::queue<std::tuple<uint8_t, uint8_t, uint8_t>> playNext;

private:
    // See http://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html
    uint8_t inPort1{1 << 3};                // Player 1 input register; bit 3 is always 1
    uint8_t prevOutPort3, currOutPort3;     // consecutive bit changes correspond to particular sound cues
    uint8_t prevOutPort5, currOutPort5;     // consecutive bit changes correspond to particular sound cues

    // The 8080 instruction set does not include opcodes for shifting.
    // An 8-bit pixel image must be shifted into a 16-bit word for the
    // desired bit-position on the screen. Space Invaders adds a
    // hardware shift register to help with the math.
    uint16_t shiftRegister;
    uint8_t  shiftOffset;
};