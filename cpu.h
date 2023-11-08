#pragma once

#include <cstdint> // for uint8_t and uint16_t types

// Condition Flags:
// There are five condition flags associated with the execution of instructions on the 8080.
// There are Zero, Sign, Parity, Carry, and Auxiliary Carry, and are each represented by a
// 1-bit register in the CPU. A flag is "set" by forcing the bit to 1; "reset" by forcing the bit to O.
// Unless indicated otherwise, when an instruction affects a flag, it affects it in the following manner:
//
// Zero:                If the result of an instruction has the value 0,
//                      this flag is set; otherwise it is reset.
//
// Sign:                If the most significant bit of the result of the
//                      operation has the value 1, this flag is set;
//                      otherwise it is reset.
//
// Parity:              If the modulo 2 sum of the bits of the result of the
//                      operation is 0, (i.e., if the result has even parity),
//                      this flag is set; otherwise it is reset (i.e., if the
//                      result has odd parity).
//
// Carry:               If the instruction resulted in a carry (from addition),
//                      or a borrow (from subtraction or a comparison)
//                      out of the high-order bit, this flag is set; otherwise it is reset.
//
// Auxiliary Carry:     If the instruction caused a carry out of bit 3 and into bit 4 of
//                      the resulting value, the auxiliary carry is set; otherwise
//                      it is reset. This flag is affected by single precision additions,
//                      subtractions, increments, decrements, comparisons, and logical
//                      operations, but is principally used with additions and increments
//                      preceding a DAA (Decimal Adjust Accumulator) instruction.

class Intel8080 {
public:
    Intel8080();
    void Emulate8080();
    static int Disassemble(uint8_t* buffer, int pc);

public:
    uint8_t    a;           // Accumulator Register
    uint8_t    b;           // B Register
    uint8_t    c;           // C Register
    uint8_t    d;           // D Register
    uint8_t    e;           // E Register
    uint8_t    h;           // High-order register H (used with L for indirect addressing)
    uint8_t    l;           // Low-order register L (used with H for indirect addressing)
    uint16_t   sp;          // Stack Pointer
    uint16_t   pc;          // Program Counter
    uint8_t*   memory;      // Pointer to a memory buffer representing RAM
    uint8_t    int_enable;  // Interrupt enable/disable flag

    struct Flag {
        uint8_t   z : 1;    // Zero
        uint8_t   s : 1;    // Sign
        uint8_t   p : 1;    // Parity
        uint8_t  cy : 1;    // Carry
        uint8_t  ac : 1;    // Auxiliary Carry
        uint8_t pad : 3;    // Unused
    } flag;

    [[nodiscard]] uint16_t getBC() const { return (b << 8) | c; }
    [[nodiscard]] uint16_t getDE() const { return (d << 8) | e; }
    [[nodiscard]] uint16_t getHL() const { return (h << 8) | l; }


private:
    void ADD(uint8_t addend, bool cy = false);
    void ANA(uint8_t operand);
    void ANI(uint8_t operand);
    void CALL(uint16_t addr);
    void CALL(uint8_t* opcode);
    void CALL(uint16_t addr, bool cond);
    void CALL(uint8_t* opcode, bool cond);
    void CMA();
    void CMC();
    void CMP(uint8_t value);
    void DAA();
    void DAD(uint16_t rp);
    void DCR(uint8_t* target);
    void DCX(uint8_t* rh, uint8_t* rl);
    void DCX_SP();
    void INR(uint8_t* target);
    void INX(uint8_t* rh, uint8_t* rl);
    void INX_SP();
    void JMP(uint8_t* opcode);
    void JMP(uint8_t* opcode, bool cond);
    void LDA(const uint8_t* opcode);
    void LDAX(uint16_t rp);
    void LHLD(uint8_t* opcode);
    void LXI(uint8_t* rh, uint8_t* rl, const uint8_t* opcode);
    void LXI_SP(uint8_t* opcode);
    void MOV(uint8_t* dst, uint8_t* src);
    void MVI(uint8_t* dst, uint8_t byte);
    void ORA(uint8_t operand);
    void PCHL();
    void POP(uint8_t* rh, uint8_t* rl);
    void POP_PSW();
    void PUSH(uint8_t* rh, uint8_t* rl);
    void PUSH_PSW();
    void RAL();
    void RAR();
    void RET();
    void RET(bool cond);
    void RLC();
    void RRC();
    void RST(uint16_t n);
    void SHLD(uint8_t* opcode);
    void SPHL();
    void STA(uint8_t* opcode);
    void STAX(uint16_t rp);
    void STC();
    void SUB(uint8_t subtrahend, bool cy = false);
    void XCHG();
    void XRA(uint8_t operand);
    void XTHL();

private:
    bool parity(uint8_t value);
    void setZSP(uint8_t result);
    uint16_t getAddr(const uint8_t *opcode);
};