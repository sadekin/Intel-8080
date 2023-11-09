#pragma once

#include <cstdint> // for uint8_t and uint16_t types
#include <iostream>
#include <fstream>
#include <string>

class Intel8080 {
public:
    Intel8080();
    ~Intel8080();

    bool Load(const std::string& filePath, uint16_t loadAddress);
    void Emulate8080();
    int  Disassemble(uint8_t* buffer, int pc);


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
    uint8_t*   opcode;      // Current opcode

    uint8_t*   memory;      // Pointer to a memory buffer representing RAM

    bool       intEnable;   // Interrupt enable/disable flag (enable = true, disable = false)
    uint8_t    intPending;

    uint64_t   cycles;

    [[nodiscard]] uint16_t getBC() const { return (b << 8) | c; }
    [[nodiscard]] uint16_t getDE() const { return (d << 8) | e; }
    [[nodiscard]] uint16_t getHL() const { return (h << 8) | l; }

    struct Flags {
        uint8_t   z : 1;    // Zero
        uint8_t   s : 1;    // Sign
        uint8_t   p : 1;    // Parity
        uint8_t  cy : 1;    // Carry
        uint8_t  ac : 1;    // Auxiliary Carry
        uint8_t pad : 3;    // Unused
    } flag;


private:
    void ADD(uint8_t addend, bool cy = false);
    void ANA(uint8_t operand);
    void ANI(uint8_t operand);
    void CALL(uint16_t addr);
    void CALL();
    void CALL_COND(bool cond);
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
    void JMP();
    void JMP_COND(bool cond);
    void LDA();
    void LDAX(uint16_t rp);
    void LHLD();
    void LXI(uint8_t *rh, uint8_t *rl);
    void LXI_SP();
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
    void RET_COND(bool cond);
    void RLC();
    void RRC();
    void RST(uint16_t n);
    void SHLD();
    void SPHL();
    void STA();
    void STAX(uint16_t rp);
    void STC();
    void SUB(uint8_t subtrahend, bool cy = false);
    void XCHG();
    void XRA(uint8_t operand);
    void XTHL();

private:
    static bool     parity(uint8_t value);
    void            setZSP(uint8_t result);
    uint16_t        getAddr() const;
};