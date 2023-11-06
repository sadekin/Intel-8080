#pragma once

#include <cstdint> // Include for uint8_t and uint16_t types

struct Flag {
    uint8_t      z : 1;     // Zero Flag (set if the result is zero)
    uint8_t      s : 1;     // Sign Flag (set if the result is negative)
    uint8_t      p : 1;     // Parity Flag (set if the number of 1 bits in the result is even)
    uint8_t     cy : 1;     // Carry Flag (set if the last addition operation resulted in a carry or if the last subtraction operation required a borrow)
    uint8_t     ac : 1;     // Auxiliary Carry Flag (used for BCD arithmetic, unused by Space Invaders)
    uint8_t    pad : 3;     // Padding to make up a byte (for the three unused bits)

    Flag() : z(0), s(0), p(0), cy(0), ac(0), pad(0) {}
};

// The state of the 8080 microprocessor.
class Intel8080 {
public:
    Intel8080();

    void Emulate8080();
public:
    uint8_t    a;           // Accumulator register
    uint8_t    b;           // General purpose register B
    uint8_t    c;           // General purpose register C
    uint8_t    d;           // General purpose register D
    uint8_t    e;           // General purpose register E
    uint8_t    h;           // High-order register H (used with L for indirect addressing)
    uint8_t    l;           // Low-order register L (used with H for indirect addressing)
    uint16_t   sp;          // Stack Pointer
    uint16_t   pc;          // Program Counter
    uint8_t*   memory;      // Pointer to a memory buffer representing RAM
    Flag       flag;        // Condition code flags
    uint8_t    int_enable;  // Interrupt enable/disable flag

    uint16_t getBC() { return (b << 8) | c; }
    uint16_t getDE() { return (d << 8) | e; }
    uint16_t getHL() {return  (h << 8) | l; }

private:
    void ADC(uint8_t operand);
    void ADD(uint8_t operand);
    void ANA(uint8_t operand);
    void CALL(uint8_t* opcode);
    void CMA();
    void CMC();
    void CMP();
    void DAA();
    void DAD(uint16_t regPair);
    void DCR(uint8_t *reg);
    void DCX(uint8_t* msgReg, uint8_t* lsbReg);
    void INR(uint8_t* reg);
    void INX(uint8_t* msbReg, uint8_t* lsbReg);
    void JMP(uint8_t* opcode);
    void LDA(uint8_t* opcode);
    void LDAX(uint16_t regPair);
    void LHLD(uint8_t* opcode);
    void LXI(uint8_t* msbReg, uint8_t* lsbReg, const uint8_t* opcode);
    void ORA(uint8_t* operand);
    void POP(uint8_t* msbReg, uint8_t* lsbReg);
    void POP_PSW();
    void PUSH(uint8_t* msbReg, uint8_t* lsbReg);
    void PUSH_PSW();
    void RAL();
    void RAR();
    void RET();
    void RLC();
    void RRC();
    void SHLD(uint8_t* opcode);
    void STA(uint8_t* opcode);
    void STAX(uint16_t regPair);
    void SBB(uint8_t operand);
    void SUB(uint8_t operand);
    void XCHG();
    void XRA(uint8_t operand);
    void XTHL();

private:
    bool parity(uint8_t value);
    void setLogicFlags();
    void setArithmeticFlags(uint8_t x, uint8_t y, bool isAddition, uint8_t prevCY);
    void setBcdArithmeticFlags(uint8_t x, uint8_t y, bool isAddition, uint8_t prevCY);
};