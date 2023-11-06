#include "cpu.h"

Intel8080::Intel8080() : a(0), b(0), c(0), d(0), e(0), h(0), l(0), sp(0), pc(0), memory(nullptr), int_enable(0) {}


bool Intel8080::parity(uint8_t value) {
    value ^= value >> 4; // XOR the high 4 bits with the low 4 bits
    value ^= value >> 2; // XOR the high 2 bits of the result with the low 2 bits
    value ^= value >> 1; // XOR the high 1 bit of the result with the low 1 bit

    // At this point, the lowest bit of 'value' contains the parity of the original byte.
    // If the lowest bit is 0 (meaning an even number of '1' bits), the parity is even (return true).
    // If the lowest bit is 1 (meaning an odd number of '1' bits), the parity is odd (return false).
    return !(value & 1);
}


void Intel8080::setArithmeticFlags(uint8_t x, uint8_t y, bool isAddition, uint8_t prevCY = 0) {
    uint16_t result = isAddition ? x + y + prevCY : x - y - prevCY;
    flag.z  = (result & 0xFF) == 0;
    flag.s  = (result & 0x80) != 0;
    flag.p  = parity(result & 0xFF);
    flag.ac = isAddition ? (x & 0x0F) + (y & 0x0F) + prevCY > 0x0F : (x & 0x0F) < (y & 0x0F) + prevCY;
}

void Intel8080::setBcdArithmeticFlags(uint8_t x, uint8_t y, bool isAddition, uint8_t prevCY = 0) {
    uint16_t result = isAddition ? x + y + prevCY : x - y - prevCY;
    flag.z  = (result & 0xFF) == 0;
    flag.s  = (result & 0x80) != 0;
    flag.p  = parity(result & 0xFF);
    flag.cy = isAddition ? result > 0xFF : x < y + prevCY;
    flag.ac = isAddition ? (x & 0x0F) + (y & 0x0F) + prevCY > 0x0F : (x & 0x0F) < (y & 0x0F) + prevCY;
}

void Intel8080::setLogicFlags() {

}

/* Opcode Functions ==================================================================================================*/

// ADC ADD Register or Memory To Accumulator With Carry; Flags affected: z, s, p, cy, ac
void Intel8080::ADC(uint8_t operand) {
    uint16_t result = a + operand + flag.cy;
    setBcdArithmeticFlags(a, operand, true, flag.cy);
    a = result & 0xFF;
}

// ADD Register or Memory To Accumulator; Flags affected: z, s, p, cy, ac
void Intel8080::ADD(uint8_t operand) {
    uint16_t result = a + operand;
    setBcdArithmeticFlags(a, operand, true);
    a = result & 0xFF;
}

// Perform bitwise AND between the accumulator and operand.
void Intel8080::ANA(uint8_t operand) {}

// Call subroutine at the specified memory address in opcode.
void Intel8080::CALL(uint8_t* opcode) {}

// Complement the accumulator (logical NOT).
void Intel8080::CMA() {}

// Complement the carry flag (toggle it).
void Intel8080::CMC() {}

// Compare the accumulator with a register or memory (set flags based on the result).
void Intel8080::CMP() {}

// DAA Decimal Adjust Accumulator for BCD (Binary Coded Decimal) arithmetic.
void Intel8080::DAA() {
    if ((a & 0x0F) > 0x09 || flag.ac) ADD(0x06);
    if ((a & 0xF0) > 0x90 || flag.cy) ADD(0x60);
}

// Double Add. Add the specified register pair to the HL register pair; Flags affected: cy
void Intel8080::DAD(uint16_t regPair) {
    uint16_t hl = getHL();
    uint32_t result = (uint32_t) hl + (uint32_t) regPair;

    h = (result >> 8) & 0xFF;
    l = result & 0xFF;

    flag.cy = (result > 0xFFFF);
}

// Decrement the value in the given register; Flags affected: z, s, p, ac
void Intel8080::DCR(uint8_t* reg) {
    uint16_t result = *reg - 1;
    setArithmeticFlags(*reg, 1, false);
    *reg = result & 0xFF;
}

// DCX Decrement the value in the given register pair; Flags affected: None
void Intel8080::DCX(uint8_t* msbReg, uint8_t* lsbReg) {
    if (--(*lsbReg) == 0xFF) (*msbReg)--; // underflow
}

// INR Increment the value in the given register; Flags affected: z, s, p, ac
void Intel8080::INR(uint8_t* reg) {
    uint16_t result = *reg + 1;
    setArithmeticFlags(*reg, 1, true);
    *reg = result & 0xFF;
}

// Increment the value in the given register pair; Flags affected: None
void Intel8080::INX(uint8_t* msbReg, uint8_t* lsbReg) {
    if (++(*lsbReg) == 0) (*msbReg)++; // overflow
}

// Jump to the address specified in opcode.
void Intel8080::JMP(uint8_t* opcode) {}

// Load the accumulator with the value from the specified memory address.
void Intel8080::LDA(uint8_t* opcode) {}

// LDAX Load Accumulator (indirectly through a register pair address BC or DE); Flags affected: None
void Intel8080::LDAX(uint16_t regPair) { a = memory[regPair]; }

// Load H and L direct from memory (LHLD).
void Intel8080::LHLD(uint8_t* opcode) {}

// Load the specified register pair with the immediate value (given in opcode); Flags affected: None.
void Intel8080::LXI(uint8_t* msbReg, uint8_t* lsbReg, const uint8_t* opcode) {
    *msbReg = opcode[2];
    *lsbReg = opcode[1];
}

// Logical OR the accumulator with the specified operand.
void Intel8080::ORA(uint8_t* operand) {}

// Pop the value off the stack into the specified register pair.
void Intel8080::POP(uint8_t* msbReg, uint8_t* lsbReg) {}

// Pop the Processor Status Word off the stack (flags and accumulator).
void Intel8080::POP_PSW() {}

// Push the value in the specified register pair onto the stack.
void Intel8080::PUSH(uint8_t* msbReg, uint8_t* lsbReg) {}

// Push the Processor Status Word onto the stack (flags and accumulator).
void Intel8080::PUSH_PSW() {}

// RAL Rotate Accumulator Left Through Carry. Flags affected: cy = msb
void Intel8080::RAL() {
    uint8_t msb = (a >> 7) & 1;
    a = (a << 1) | flag.cy; // msb = prev cy
    flag.cy = msb;
}

// RAR Rotate Accumulator Right Through Carry. Flags affected: cy = lsb
void Intel8080::RAR() {
    uint8_t lsb = a & 1;
    a = (flag.cy << 7) | (a >> 1); // lsb = prev cy
    flag.cy = lsb;
}

// Return from subroutine.
void Intel8080::RET() {}

// Rotate Accumulator Left; Flags affected: cy = msb (most significant bit)
void Intel8080::RLC() {
    uint8_t msb = (a >> 7) & 1;
    a = (a << 1) | msb;
    flag.cy = msb;
}

// Rotate Accumulator Right; Flags affected: cy = lsb (least significant bit)
void Intel8080::RRC() {
    uint8_t lsb = a & 1;
    a = (lsb << 7) | (a >> 1);
    flag.cy = lsb;
}

// Store H and L direct to memory (SHLD).
void Intel8080::SHLD(uint8_t* opcode) {}

// Store the accumulator direct to memory (STA).
void Intel8080::STA(uint8_t* opcode) {}

// STAX Store Accumulator (indirectly into address pointed by BC or DE); Flags affected: None
void Intel8080::STAX(uint16_t regPair) { memory[regPair] = a; }

// Subtract operand from accumulator with borrow (SBB); Flags affected: z, s, p, cy, ac
void Intel8080::SBB(uint8_t operand) {
    uint16_t result = a - operand - flag.cy;
    setBcdArithmeticFlags(a, operand, false, flag.cy);
    a = result & 0xFF;
}

// SUB Subtract Register or Memory From Accumulator; Flags affected z, s, p, cy, ac
void Intel8080::SUB(uint8_t operand) {
    uint16_t result = a - operand;
    setBcdArithmeticFlags(a, operand, false);
    a = result & 0xFF;
}

// Exchange the contents of the H and L registers with the D and E registers.
void Intel8080::XCHG() {}

// Exclusive OR the accumulator with operand (XRA).
void Intel8080::XRA(uint8_t operand) {}

// Exchange the contents of the H and L registers with the top of the stack (XTHL).
void Intel8080::XTHL() {}


