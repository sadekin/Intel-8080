#pragma once

#include <string>
#include <iostream>
#include <fstream>

constexpr uint32_t RAM_SIZE = 0x10000; // 64Kb

class Memory {
public:
    bool     load(const std::string& filePath, uint16_t loadAddress);
    uint8_t  read(uint16_t addr);
    void     write(uint16_t addr, uint8_t data);

private:
    uint8_t memory[RAM_SIZE];
};