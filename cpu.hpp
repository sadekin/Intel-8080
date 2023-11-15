#pragma once

#include <cstdint> // for uint8_t and uint16_t types
#include <iostream>
#include <fstream>
#include <string>

#include "memory.hpp"
#include "io.hpp"

class Intel8080 {
public:
    Intel8080() : INTE(), pc(), sp(), reg8(), memory(nullptr), ioPorts(nullptr) {
        memory = new Memory();
        ioPorts = new IOPorts();
    }

    int     Execute(int numCycles);

    uint8_t read(uint16_t addr);
    void    write(uint16_t addr, uint8_t data);

    uint8_t inport(uint8_t port);
    void    outport(uint8_t port, uint8_t data);

    void    interrupt(uint8_t n);

    int     disassemble(uint8_t opcode, uint16_t pc);

public:
    std::array<uint8_t, 9> reg8;
    uint8_t INTE;
    uint16_t sp, pc;

    int cycles;

    enum Register8 : uint8_t { B, C, D, E, H, L, M, A, FLAGS };
    enum FLAGS8080 : uint8_t { S = 7, Z = 6, AC = 4, P = 2, CY = 0 };

    Memory*     memory;
    IOPorts*    ioPorts;

    // Convenience variables
    uint8_t     opcode;
    uint8_t     reg;
    uint16_t    reg2;
    uint8_t     temp8;
    uint16_t    temp16;
    uint32_t    temp32;

    uint16_t reg16_BC() const   { return (((uint16_t) reg8[B]) << 8) | (uint16_t) reg8[C]; }
    uint16_t reg16_DE() const   { return (((uint16_t) reg8[D]) << 8) | (uint16_t) reg8[E]; }
    uint16_t reg16_HL() const   { return (((uint16_t) reg8[H]) << 8) | (uint16_t) reg8[L]; }
    uint16_t reg16_PSW() const  { return (((uint16_t) reg8[A]) << 8) | (uint16_t) reg8[FLAGS]; }

    uint8_t GetFlag(FLAGS8080 f);
    bool    TestCond(uint8_t code);
    void    SetFlag(FLAGS8080 f, bool v);
    void    SetZSP(uint8_t value);

private:
    void ACI();
    void ADC();
    void ADD();
    void ADI();
    void ANA();
    void ANI();
    void CALL();
    void Cccc();
    void CMA();
    void CMC();
    void CMP();
    void CPI();
    void DAA();
    void DAD();
    void DCR();
    void DCX();
    void DI();
    void EI();
    void HLT();
    void IN();
    void INR();
    void INX();
    void JMP();
    void Jccc();
    void LDA();
    void LDAX();
    void LHLD();
    void LXI();
    void MOV();
    void MVI();
    void NOP();
    void ORA();
    void ORI();
    void OUT();
    void PCHL();
    void POP();
    void PUSH();
    void RAL();
    void RAR();
    void RET();
    void Rccc();
    void RLC();
    void RRC();
    void RST();
    void SBB();
    void SBI();
    void SHLD();
    void SPHL();
    void STA();
    void STAX();
    void STC();
    void SUB();
    void SUI();
    void XCHG();
    void XRA();
    void XRI();
    void XTHL();

private:
    uint8_t     readReg8(uint8_t r);
    void        writeReg8(uint8_t r, uint8_t value);
    void        push(uint16_t value);
    uint16_t    pop();
    uint16_t    readRP(uint8_t rp);
    void        writeRP(uint8_t rp, uint8_t lb, uint8_t hb);
    void        write16RP(uint8_t rp, uint16_t value);
    uint16_t    readRP_PUSHPOP(uint8_t rp);
    void        write16RP_PUSHPOP(uint8_t rp, uint16_t value);

    bool        parity(uint8_t value);
    bool        carry(uint8_t a, uint8_t b, uint8_t result, uint8_t m);
    bool        borrow(uint8_t a, uint8_t b, uint8_t result, uint8_t m);
};