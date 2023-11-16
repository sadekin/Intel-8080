#pragma once

#include <cstdint> // for uint8_t and uint16_t types
#include <iostream>
#include <fstream>
#include <string>

#include "memory.hpp"
#include "io.hpp"

class Intel8080 {
public:
    Intel8080();

    // A description of each of these is included above their implementations.
    int     execute(int numCycles);
    uint8_t read(uint16_t addr) const;
    void    write(uint16_t addr, uint8_t data) const;
    uint8_t inport(uint8_t port) const;
    void    outport(uint8_t port, uint8_t data) const;
    void    interrupt(uint8_t n);
    int     disassemble(uint8_t opcode, uint16_t pc);
    bool    load(const std::string& filePath, uint16_t loadAddress) const;

    Memory*     memory;     // Pointer to memory management object
    IOPorts*    ioPorts;    // Pointer to IO port management object

private:
    uint16_t sp;            // Stack pointer
    uint16_t pc;            // Program counter
    uint8_t  opcode;        // Current instruction
    uint8_t  intEnable;     // Interrupt enable/disable flag
    int      cycles;        // Clock cycle counter for accurate emulation

    // Enumerations for register and flag identifiers.
    // Dest and Source reg fields:
    //    111=A   (Accumulator)
    //    000=B
    //    001=C
    //    010=D
    //    011=E
    //    100=H
    //    101=L
    //    110=M   (Memory reference through address in H:L)
    // Source: http://dunfield.classiccmp.org/r/8080.txt
    std::array<uint8_t, 9> reg8; // 8-bit registers: B, C, D, E, H, L, M, A, and FLAGS
    enum Register8 : uint8_t { B, C, D, E, H, L, M, A, FLAGS };
    // Sign, Zero, Auxiliary Carry, Parity, and Carry flags
    enum FLAGS8080 : uint8_t { S = 7, Z = 6, AC = 4, P = 2, CY = 0 };

    // Convenience variables used for intermediate calculations/operations
    uint8_t     reg;
    uint8_t     reg2;
    uint8_t     temp8;
    uint16_t    temp16;
    uint32_t    temp32;

    // Lookup table mapping opcodes to their corresponding operations
    typedef void (Intel8080::*Operation)();
    std::vector<Operation> lookup;

private:
    // List of all unique 8080 opcodes in alphabetical order
    void  ACI();        void   IN();        void  RAR();
    void  ADC();        void  INR();        void  RET();
    void  ADD();        void  INX();        void Rccc();
    void  ADI();        void  JMP();        void  RLC();
    void  ANA();        void Jccc();        void  RRC();
    void  ANI();        void  LDA();        void  RST();
    void CALL();        void LDAX();        void  SBB();
    void Cccc();        void LHLD();        void  SBI();
    void  CMA();        void  LXI();        void SHLD();
    void  CMC();        void  MOV();        void SPHL();
    void  CMP();        void  MVI();        void  STA();
    void  CPI();        void  NOP();        void STAX();
    void  DAA();        void  ORA();        void  STC();
    void  DAD();        void  ORI();        void  SUB();
    void  DCR();        void  OUT();        void  SUI();
    void  DCX();        void PCHL();        void XCHG();
    void   DI();        void  POP();        void  XRA();
    void   EI();        void PUSH();        void  XRI();
    void  HLT();        void  RAL();        void XTHL();

    // Captures all unimplemented instructions
    void  XXX();

private:
    // Methods to access and manipulate register values and stack operations
    uint8_t     readReg8(uint8_t r);
    void        writeReg8(uint8_t r, uint8_t value);
    void        push(uint16_t value);
    uint16_t    pop();
    uint16_t    readRP(uint8_t rp);
    void        writeRP(uint8_t rp, uint8_t lb, uint8_t hb);
    void        write16RP(uint8_t rp, uint16_t value);
    uint16_t    readRP_PUSHPOP(uint8_t rp);
    void        write16RP_PUSHPOP(uint8_t rp, uint16_t value);

    // Helper methods to generate and manipulate 16-bit register pairs from 8-bit registers
    uint16_t reg16_BC() const   { return (((uint16_t) reg8[B]) << 8) | (uint16_t) reg8[C]; }
    uint16_t reg16_DE() const   { return (((uint16_t) reg8[D]) << 8) | (uint16_t) reg8[E]; }
    uint16_t reg16_HL() const   { return (((uint16_t) reg8[H]) << 8) | (uint16_t) reg8[L]; }
    uint16_t reg16_PSW() const  { return (((uint16_t) reg8[A]) << 8) | (uint16_t) reg8[FLAGS]; }

    // Helper methods for setting/getting/testing condition flags
    uint8_t GetFlag(FLAGS8080 f);
    bool    TestCond(uint8_t code);
    void    SetFlag(FLAGS8080 f, bool v);
    void    SetZSP(uint8_t value);
    bool    parity(uint8_t value);
    bool    carry(uint8_t a, uint8_t b, uint8_t result, uint8_t mask);
    bool    borrow(uint8_t a, uint8_t b, uint8_t result, uint8_t mask);
};