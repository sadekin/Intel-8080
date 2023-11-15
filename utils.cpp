#include "cpu.hpp"

/* Utility functions for reading/writing memory */

uint8_t Intel8080::readReg8(uint8_t r) {
    if (r == M)
        return read(reg16_HL());
    else
        return reg8[r];
}

void Intel8080::writeReg8(uint8_t r, uint8_t value) {
    if (r == M)
        write(reg16_HL(), value);
    else
        reg8[r] = value;
}

// Register pair 'RP' fields:
// 00=BC   (B:C as 16 bit register)
// 01=DE   (D:E as 16 bit register)
// 10=HL   (H:L as 16 bit register)
// 11=SP   (Stack pointer, refers to PSW (FLAGS:A) for PUSH/POP)
// Source: http://dunfield.classiccmp.org/r/8080.txt
uint16_t Intel8080::readRP(uint8_t rp) {
    switch (rp) {
        case  0x00:     return reg16_BC();
        case  0x01:     return reg16_DE();
        case  0x02:     return reg16_HL();
        case  0x03:     return sp;
        default:        return 0;
    }
}

void Intel8080::writeRP(uint8_t rp, uint8_t lb, uint8_t hb) {
    switch (rp) {
        case 0x00:
            reg8[C] = lb;
            reg8[B] = hb;
            break;
        case 0x01:
            reg8[E] = lb;
            reg8[D] = hb;
            break;
        case 0x02:
            reg8[L] = lb;
            reg8[H] = hb;
            break;
        case 0x03:
            sp = (uint16_t) lb | ((uint16_t) hb << 8);
            break;
        default:
            break;
    }
}

void Intel8080::write16RP(uint8_t rp, uint16_t value) {
    switch (rp) {
        case 0x00:
            reg8[C] = value & 0xFF;
            reg8[B] = value >> 8;
            break;
        case 0x01:
            reg8[E] = value & 0xFF;
            reg8[D] = value >> 8;
            break;
        case 0x02:
            reg8[L] = value & 0xFF;
            reg8[H] = value >> 8;
            break;
        case 0x03:
            sp = value;
            break;
        default:
            break;
    }
}

// Register pair 'RP' fields:
// 00=BC   (B:C as 16 bit register)
// 01=DE   (D:E as 16 bit register)
// 10=HL   (H:L as 16 bit register)
// 11=SP   (Stack pointer, refers to PSW (FLAGS:A) for PUSH/POP)
// Source: http://dunfield.classiccmp.org/r/8080.txt
uint16_t Intel8080::readRP_PUSHPOP(uint8_t rp) {
    switch (rp) {
        case 0b00:     return reg16_BC();
        case 0b01:     return reg16_DE();
        case 0b10:     return reg16_HL();
        case 0b11:     return (reg16_PSW() | 0x02) & 0xFFD7;
        default:       return 0;
    }
}

void Intel8080::write16RP_PUSHPOP(uint8_t rp, uint16_t value) {
    switch (rp) {
        case 0b00:
            reg8[C] = value & 0xFF;
            reg8[B] = value >> 8;
            break;
        case 0b01:
            reg8[E] = value & 0xFF;
            reg8[D] = value >> 8;
            break;
        case 0b10:
            reg8[L] = value & 0xFF;
            reg8[H] = value >> 8;
            break;
        case 0b11:
            // FLAGS:    SZ0A 0P1C (or SZyA yPxC, where y is always 0 and x is always 1)
            // 0x02 = 0b 0000 0010 (unused flag bit 1 is always 1)
            // 0xD7 = 0b 1101 0111 (unused flag bits 3 and 5 are always 0)
            reg8[FLAGS] = ((value & 0xFF) | 0x02) & 0xD7;
            reg8[A]     =  value >> 8;
        default:
            break;
    }
}


// Little endian
void Intel8080::push(uint16_t value) {
    write(--sp, value >> 8);    // write MSB after LSB
    write(--sp, value & 0xFF);  // write LSB before MSB
}

uint16_t Intel8080::pop() {
    temp16 = read(sp++);
    temp16 |= ((uint16_t) read(sp++)) << 8;
    return temp16;    // MSB | LSB
}

/* Utility functions for setting/getting/testing condition flags */
uint8_t Intel8080::GetFlag(FLAGS8080 f) {
    return (reg8[FLAGS] >> f) & 1;
}

// Condition code 'CCC' fields: (FLAGS: S Z x A x P x C)
//    000=NZ  ('Z'ero flag not set)
//    001=Z   ('Z'ero flag set)
//    010=NC  ('C'arry flag not set)
//    011=C   ('C'arry flag set)
//    100=PO  ('P'arity flag not set - ODD)
//    101=PE  ('P'arity flag set - EVEN)
//    110=P   ('S'ign flag not set - POSITIVE)
//    111=M   ('S'ign flag set - MINUS)
// Source: http://dunfield.classiccmp.org/r/8080.txt
bool Intel8080::TestCond(uint8_t code) {
    switch (code) {
        case 0b000:     return !GetFlag(Z);
        case 0b001:     return  GetFlag(Z);
        case 0b010:     return !GetFlag(CY);
        case 0b011:     return  GetFlag(CY);
        case 0b100:     return !GetFlag(P);
        case 0b101:     return  GetFlag(P);
        case 0b110:     return !GetFlag(S);
        case 0b111:     return  GetFlag(S);
        default:        return false;
    }
}

void Intel8080::SetFlag(FLAGS8080 f, bool v) {
    if (v)
        reg8[FLAGS] |= (1 << f);
    else
        reg8[FLAGS] &= ~(1 << f);
}

void Intel8080::SetZSP(uint8_t value) {
    SetFlag(Z, value == 0);
    SetFlag(S, value & 0x80);
    SetFlag(P, parity(value));
}

// See approach 2 in editorial: https://leetcode.com/problems/number-of-1-bits/editorial/
bool Intel8080::parity(uint8_t value) {
    uint8_t oneBitsCount;
    for (oneBitsCount = 0; value != 0; oneBitsCount++)
        value &= (value - 1);
    return (oneBitsCount & 1) == 0; // Set P when there is an even number of 1 bits
}

// Basically the flag is testing the result of upper bits:
// Carry can occur in one of three cases:
// 1. msb(a) and msb(b) are set (0.1xxx + 0.1xxx -> 1.0xxx)
// 2. msb(a) is set but msb(result) is not set (0.1xxx + 0.xxxx -> 1.0xxx)
// 3. msb(b) is set but msb(result) is not set (0.xxxx + 0.1xxx -> 1.0xxx)
// Source: https://www.reddit.com/r/EmuDev/comments/110epqk/comment/j89y04a/?utm_source=share&utm_medium=web2x&context=3
bool Intel8080::carry(uint8_t a, uint8_t b, uint8_t result, uint8_t m) {
    return ((a & b) | (a & ~result) | (b & ~result)) & m;
}

// a - b = result => a = b + result
bool Intel8080::borrow(uint8_t a, uint8_t b, uint8_t result, uint8_t m) {
    return carry(result, b, a, m);
}

