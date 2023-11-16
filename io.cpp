#include "io.hpp"

// INPUTS
// Port 0
// bit 0 DIP4 (Seems to be self-test-request read at power up)
// bit 1 Always 1
// bit 2 Always 1
// bit 3 Always 1
// bit 4 Fire
// bit 5 Left
// bit 6 Right
// bit 7 ? tied to demux port 7 ?
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
//
// Port 2
// bit 0 = DIP3 00 = 3 ships  10 = 5 ships
// bit 1 = DIP5 01 = 4 ships  11 = 6 ships
// bit 2 = Tilt
// bit 3 = DIP6 0 = extra ship at 1500, 1 = extra ship at 1000
// bit 4 = P2 shot (1 if pressed)
// bit 5 = P2 left (1 if pressed)
// bit 6 = P2 right (1 if pressed)
// bit 7 = DIP7 Coin info displayed in demo screen 0=ON
//
// Port 3
// bit 0-7 Shift register data
//
// Source: http://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html
uint8_t IOPorts::read(uint8_t port) {
    switch (port) {
        case 0:     return 0xFF;
        case 1:     return inPort1;
        case 2:     return 0;
        case 3:     return (shiftRegister >> (8 - shiftOffset)) & 0xFF; // bit shift register read
        default:    return inPort1;
    }
}

// OUTPUTS
// Port 2:
// bit 0,1,2 Shift amount
//
// Port 3: (discrete sounds)
// bit 0=UFO (repeats)        SX0 0.raw
// bit 1=Shot                 SX1 1.raw
// bit 2=Flash (player die)   SX2 2.raw
// bit 3=Invader die          SX3 3.raw
// bit 4=Extended play        SX4
// bit 5= AMP enable          SX5
// bit 6= NC (not wired)
// bit 7= NC (not wired)
// Port 4: (discrete sounds)
// bit 0-7 shift data (LSB on 1st write, MSB on 2nd)
//
// Port 5:
// bit 0=Fleet movement 1     SX6 4.raw
// bit 1=Fleet movement 2     SX7 5.raw
// bit 2=Fleet movement 3     SX8 6.raw
// bit 3=Fleet movement 4     SX9 7.raw
// bit 4=UFO Hit              SX10 8.raw
// bit 5= NC (Cocktail mode control ... to flip screen)
// bit 6= NC (not wired)
// bit 7= NC (not wired)
//
// Port 6:
// Watchdog ... read or write to reset
//
// Source: http://computerarcheology.com/Arcade/SpaceInvaders/Hardware.html
void IOPorts::write(uint8_t port, uint8_t data) {
    switch (port) {
        case 2:                             // shift amount (3 bits)
            shiftOffset = data & 0x07;
            break;
        case 3:                             // sound bits
            prevOutPort3 = currOutPort3;
            currOutPort3 = data;
            playNext.push({3 , prevOutPort3, currOutPort3 });
            break;
        case 4:                             // shift data
            shiftRegister = (shiftRegister >> 8) | (data << 8);
            break;
        case 5:                             // sound bits
            prevOutPort5 = currOutPort5;
            currOutPort5 = data;
            playNext.push({5, prevOutPort5, currOutPort5 });
            break;
        default:
            break;
    }
}


void IOPorts::setInPort1Bit(uint8_t bitNum, bool v) {
    if (v)
        inPort1 |= (1 << bitNum);
    else
        inPort1 &= ~(1 << bitNum);
}
