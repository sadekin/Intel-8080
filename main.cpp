#include "cpu.h"

#include <iostream>
#include <fstream>
#include <vector>


// Command line args
//int main(int argc, char* argv[]) {
//    if (argc < 2) {
//        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
//        return 1;
//    }
//
//    std::ifstream flag(argv[1], std::ios::binary | std::ios::ate);
//    if (!flag) {
//        std::cerr << "Error: Couldn't open " << argv[1] << std::endl;
//        return 1;
//    }
//
//    // Determine the file size by checking the current position (which is at the end due to std::ios::ate)
//    std::streampos fsize = flag.tellg();
//    // Reset the position to the beginning of the file for reading
//    flag.seekg(0, std::ios::beg);
//
//    std::vector<unsigned char> buffer(fsize);
//    if (!flag.read(reinterpret_cast<char*>(buffer.data()), fsize)) {
//        std::cerr << "Error reading file." << std::endl;
//        return 1;
//    }
//
//    flag.close();
//
//    int pc = 0;
//    while (pc < fsize) {
//        pc += Disassemble8080Op(buffer.data(), pc);
//    }
//
//    return 0;
//}

// Without command line args
int main() {

    std::ifstream f("invaders", std::ios::binary | std::ios::ate);

    // Determine the file size by checking the current position (which is at the end due to std::ios::ate)
    std::streampos fsize = f.tellg();
    // Reset the position to the beginning of the file for reading
    f.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(fsize);
    if (!f.read(reinterpret_cast<char*>(buffer.data()), fsize)) {
        std::cerr << "Error reading file." << std::endl;
        return 1;
    }

    f.close();

    int pc = 0;
    while (pc < fsize) {
        pc += Intel8080::Disassemble(buffer.data(), pc);
    }

    return 0;
}
