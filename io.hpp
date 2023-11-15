#pragma once

#include <cstdint>

class IOPorts {
public:
    uint8_t read(uint8_t port);
    void    write(uint8_t port, uint8_t data);

public:
    uint8_t inPort0{0b1110}, inPort1{1 << 3};
    uint8_t outPort3{};
    uint8_t outPort5{};

    uint16_t shiftRegister{};
    uint8_t  shiftOffset{};
};