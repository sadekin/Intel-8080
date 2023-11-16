#include "memory.hpp"
#include <iostream>
#include <string>

bool Memory::load(const std::string& filePath, uint16_t loadAddress) {
    // Open the file for binary reading
    std::ifstream file(filePath, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    // Seek to the end of the file to determine its size
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // Check if there is enough memory to load the file
    if (loadAddress + fileSize > 0xFFFF) {
        std::cerr << "File size exceeds available memory space." << std::endl;
        return false;
    }

    // Read the file contents into memory starting from the specified loadAddress
    file.read(reinterpret_cast<char*>(&memory[loadAddress]), fileSize);

    if (!file) {
        std::cerr << "Failed to read file: " << filePath << std::endl;
        return false;
    }

    return true;
}

uint8_t Memory::read(uint16_t addr) {
    return memory[addr];
}

void Memory::write(uint16_t addr, uint8_t data) {
    memory[addr] = data;
}