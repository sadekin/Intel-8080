#include "io.hpp"

uint8_t IOPorts::read(uint8_t port) {
    switch (port) {
        case 0:     return 0xFF;
        case 1:     return inPort1;
        case 2:     return 0;
        case 3:     return (shiftRegister >> (8 - shiftOffset)) & 0xFF; // bit shift register read
        default:    return inPort1;
    }
}

void IOPorts::write(uint8_t port, uint8_t data) {
    switch (port) {
        case 2: shiftOffset = data & 0x07;                              break; // shift amount (3 bits)
        case 3: outPort3 = data;                                        break; // sound bits
        case 4: shiftRegister = (shiftRegister >> 8) | (data << 8);     break; // shift data
        case 5: outPort5 = data;                                        break; // sound bits
        default: break;
    }
}
