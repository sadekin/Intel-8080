#include "cpu.h"

Intel8080::Intel8080() : a(0), b(0), c(0), d(0), e(0), h(0), l(0), sp(0), pc(0), memory(nullptr), int_enable(0) {}

/* Utility functions for setting flags. */
bool Intel8080::parity(uint8_t value) {
    value ^= value >> 4; // XOR the high 4 bits with the low 4 bits
    value ^= value >> 2; // XOR the high 2 bits of the result with the low 2 bits
    value ^= value >> 1; // XOR the high 1 bit of the result with the low 1 bit

    // At this point, the lowest bit of 'value' contains the parity of the original byte.
    // If the lowest bit is 0 (meaning an even number of '1' bits), the parity is even (return true).
    // If the lowest bit is 1 (meaning an odd number of '1' bits), the parity is odd (return false).
    return !(value & 1);
}

void Intel8080::setZSP(uint8_t result) {
    flag.z = result == 0x00;
    flag.s = (result & 0x80) == 0x80;
    flag.p = parity(result);
}

/* Utility function for data transfer. */
uint16_t getAddr(const uint8_t* opcode) { return (opcode[2] << 8) | opcode[1]; }

/* Opcode Functions */

/* Data Transfer Group:
 * This group of instructions transfers data to and from registers and memory.
 * Condition flags are not affected by any instruction in this group.
 * */

// Move data between register/memory.
void Intel8080::MOV(uint8_t* dst, uint8_t* src) { *dst = *src; }

// Move to register/memory immediate.
void Intel8080::MVI(uint8_t *dst, uint8_t byte) {
    *dst = byte;
    pc++;
}

// LXI rp, data 16 (Load resister pair immediate): (rh) <- (byte 3), (rl) <- (byte 2).
// Note: Stored from LSB to MSB since the 8080 is little endian.
void Intel8080::LXI(uint8_t* rh, uint8_t* rl, const uint8_t* opcode) {
    *rh = opcode[2];    // high-order register
    *rl = opcode[1];    // low-order register
    pc += 2;
}

// LDA addr (Load Accumulator direct): (A) <- ((byte 3)(byte 2))
void Intel8080::LDA(const uint8_t* opcode) { a = memory[getAddr(opcode)]; }

// STA addr (Store Accumulator direct): ((byte 3)(byte 2)) <- (A)
void Intel8080::STA(uint8_t* opcode) { memory[getAddr(opcode)] = a; }

// LHLD addr (Load H and L direct): (L) <- ((byte 3)(byte 2)), H <- ((byte 3)(byte 2) + 1);
void Intel8080::LHLD(uint8_t* opcode) {
    uint16_t addr = getAddr(opcode);
    l = memory[addr];
    h = memory[addr + 1];
}

// SHLD addr (Store H and L direct): ((byte 3)(byte 2)) <- L, ((byte 3)(byte 2) + 1) <- H;
void Intel8080::SHLD(uint8_t* opcode) {
    uint16_t addr = getAddr(opcode);
    memory[addr]     = l;
    memory[addr + 1] = h;
}

// LDAX (Load Accumulator indirect): (A) <- ((rp))
void Intel8080::LDAX(uint16_t rp) { a = memory[rp]; }

// STAX (Store Accumulator indirect): ((rp)) <- (A)
void Intel8080::STAX(uint16_t rp) { memory[rp] = a; }

// XCHG (Exchange H and L with D and E)
void Intel8080::XCHG() {
    uint8_t tempH = h, tempL = l;
    h = d;      l = e;
    d = tempH;  e = tempL;
}

/* Arithmetic Group:
 * This group of instructions performs arithmetic operations on data in registers and memory.
 * Unless indicated otherwise, all instructions in this group affect the Zero, Sign, Parity, Carry,
 * and Auxiliary Carry flags according to the standard rules.
 * All subtraction operations are performed via two's complement arithmetic and set the carry flag
 * to one to indicate a borrow and clear it to indicate no borrow. */

// ADD/ADC/ADI/ACI r/M/data (Add register/memory/immediate with/without carry to Accumulator);
// Flags affected: ALL.
void Intel8080::ADD(uint8_t addend, bool cy) {
    uint16_t result = a + addend + cy;
    flag.cy = result > 0xFF;
    flag.ac = (((a & 0xF) + (addend & 0xF) + cy) & 0x10) == 0x10;
    a = result & 0xFF;
    setZSP(a);
}

// SUB/SUI/SBB/SBI r/M/data (Subtract register/memory/immediate with/without borrow from Accumulator);
// Flags affected: ALL.
void Intel8080::SUB(uint8_t subtrahend, bool cy) {
    uint16_t result = a - subtrahend - cy;
    flag.cy = a < subtrahend + cy;
    flag.ac = (((a & 0xF) - (subtrahend & 0xF) - cy) & 0x10) == 0x10;
    a = result & 0xFF;
    setZSP(a);
}

// INR r/M (Increment register/memory); Flags affected: All except CY.
void Intel8080::INR(uint8_t* target) {
    uint16_t result = *target + 1;
    flag.ac = (*target & 0x0F) + 1 > 0x0F;
    *target = result & 0xFF;
    setZSP(*target);
}

// DCR r/M (Decrement register/memory); Flags affected: ALL except CY.
void Intel8080::DCR(uint8_t* target) {
    flag.ac = (*target & 0x0F) == 0;
    uint16_t result = *target - 1;
    *target = result & 0xFF;
    setZSP(*target);
}

// INX rp (Increment register pair); Flags affected: NONE
void Intel8080::INX(uint8_t* rh, uint8_t* rl) {
    if (++(*rl) == 0) (*rh)++; // overflow
}

// DCX rp (Decrement register pair); Flags affected: NONE
void Intel8080::DCX(uint8_t* rh, uint8_t* rl) {
    if (--(*rl) == 0xFF) (*rh)--; // underflow
}

// DAA Decimal Adjust Accumulator for BCD (Binary Coded Decimal) arithmetic.
// The eight-bit number in the accumulator is adjusted to form two four-bit BCD digits by
// the following process:
//
// 1. If the value of the least significant 4 bits of the accumulator is greater than 9
// or if the AC flag is set, 6 is added to the accumulator.
//
// 2. If the value of the most significant 4 bits of the accumulator is now greater than 9,
// or if the CY flag is set, 6 is added to the most significant 4 bits of the accumulator.
void Intel8080::DAA() {
    // TODO
}

// DAD rp (Add register pair to H and L); Flags affected: Only CY.
void Intel8080::DAD(uint16_t rp) {
    uint32_t result = getHL() + rp;
    h = (result >> 8) & 0xFF;
    l = result & 0xFF;
    flag.cy = result > 0xFFFF;
}

/* Logical Group:
 * This group of instructions performs logical (Boolean) operations on data in registers and memory and on
 * condition flags. Unless indicated otherwise, all instructions in this group affect the Zero, Sign, Parity,
 * Auxiliary Carry, and Carry flags according to the standard rules. */

// ANA (Logical AND register/memory with accumulator); Flags affected: ALL, cy = 0.
// Note: The 8080 logical AND instructions set the flag to reflect the logical OR of bit 3
// of the values involved in the AND operation.
void Intel8080::ANA(uint8_t operand) {
    flag.cy = 0;
    flag.ac = (a | operand) & 0x08;
    a &= operand;
    setZSP(a);
}

// ANI data (AND immediate with accumulator); Flags affected: ALL, cy = ac = 0.
void Intel8080::ANI(uint8_t operand) {
    a &= operand;
    flag.cy = flag.ac = 0;
    setZSP(a);
}

// ORA/ORI (Logical OR the accumulator with register/memory/data); Flags affected: ALL, cy = ac = 0.
void Intel8080::ORA(uint8_t operand) {
    a |= operand;
    flag.cy = flag.ac = 0;
    setZSP(a);
}


// XRA/XRI (Exclusive OR the accumulator with register/memory/data); Flags affected: ALL, cy = ac = 0.
void Intel8080::XRA(uint8_t operand) {
    a ^= operand;
    flag.cy = flag.ac = 0;
    setZSP(a);
}

// CMP/CPI (Compare register/memory/immediate with accumulator); Flags affected: ALL.
void Intel8080::CMP(uint8_t value) {
    uint16_t result = a - value;
    flag.cy = a < value;
    flag.ac = (((a & 0x0F) - (value & 0x0F)) & 0x10) == 0x10;
    setZSP(result & 0xFF);
}

// RLC (Rotate [Accumulator] left); Flags affected: cy = msb (most significant bit)
void Intel8080::RLC() {
    uint8_t msb = (a >> 7) & 1;
    a = (a << 1) | msb;
    flag.cy = msb;
}

// RRC (Rotate [Accumulator] right); Flags affected: cy = lsb (least significant bit)
void Intel8080::RRC() {
    uint8_t lsb = a & 1;
    a = (lsb << 7) | (a >> 1);
    flag.cy = lsb;
}

// RAL (Rotate [accumulator] left through carry); Flags affected: cy = msb.
void Intel8080::RAL() {
    uint8_t msb = (a >> 7) & 1;
    a = (a << 1) | flag.cy; // msb = prev cy
    flag.cy = msb;
}

// RAR (Rotate [accumulator] right through carry). Flags affected: cy = lsb.
void Intel8080::RAR() {
    uint8_t lsb = a & 1;
    a = (flag.cy << 7) | (a >> 1); // lsb = prev cy
    flag.cy = lsb;
}

// CMA (Complement accumulator) (logical NOT); Flags affected: NONE.
void Intel8080::CMA() { a = ~a; }

// CMC (Complement carry); Flags affected: cy.
void Intel8080::CMC() { flag.cy = !flag.cy; }

// STC (Set carry); Flags affected: cy = 1;
void Intel8080::STC() { flag.cy = 1; }

/* Branch Group: */
// This group of instructions alter normal sequential program flow.
// Condition flags are not affected by any instruction in this group.

// JMP addr (Jump)
void Intel8080::JMP(uint8_t* opcode) { pc = getAddr(opcode); }

// Jcondition addr (Conditional jump)
void Intel8080::JMP(uint8_t *opcode, bool cond) { if (cond) pc = getAddr(opcode); }

// CALL addr (Call)
void Intel8080::CALL(uint16_t addr) {
    uint16_t ret = pc + 2;
    memory[sp - 2] = ret & 0xFF;          // store LSB before MSB
    memory[sp - 1] = (ret >> 8) & 0xFF;   // store MSB after LSB
    sp -= 2;
    pc = addr;
}

// Ccondition addr (Condition call)
void Intel8080::CALL(uint16_t addr, bool cond) { if (cond) CALL(addr); }

// RET (Return [from a subroutine])
void Intel8080::RET() {
    pc = (memory[sp + 1] << 8) | memory[sp]; //
    sp += 2;
}

// Rcondition (Conditional return)
void Intel8080::RET(bool cond) { if (cond) RET(); }

// RST n (Restart)
void Intel8080::RST(uint16_t n) { CALL(8 * n); }

// PCHL (Jump H and L indirect - move H and L to PC)
void Intel8080::PCHL() { pc = getHL(); }


/* Stack, I/O, and Machine Control Group:
 * This group of instructions performs I/O, manipulates the Stack, and alters internal control flags.
 * Unless otherwise specified, condition flags are not affected by any instructions in this group. */

// PUSH rp (Push the value in the specified register pair onto the stack)
void Intel8080::PUSH(uint8_t* rh, uint8_t* rl) {
    memory[sp - 2] = *rl;   // store LSB before MSB
    memory[sp - 1] = *rh;   // store MSB after LSB

    sp -= 2;
}

// POP rp (Pop the value off the stack into the specified register pair)
void Intel8080::POP(uint8_t* rh, uint8_t* rl) {
    *rl = memory[sp];       // LSB is stored first
    *rh = memory[sp + 1];   // MSB is sorted second

    sp += 2;
}

// PUSH PSW (Push processor status word onto the stack (flags and accumulator)).
void Intel8080::PUSH_PSW() {
    memory[sp - 1] = a;

    uint8_t psw = 0;
    psw |= flag.cy << 0;
    psw |=       1 << 1; // Not used, always one (see bottom of opcode table)
    psw |=  flag.p << 2;
    psw |= flag.ac << 4;
    psw |=  flag.z << 6;
    psw |=  flag.s << 7;
    memory[sp - 2] = psw;

    sp -= 2;
}

// POP PSW (Pop processor status word off the stack (flags and accumulator)).
void Intel8080::POP_PSW() {
    a = memory[sp + 1];

    uint8_t psw = memory[sp];
    flag.cy = (psw >> 0) & 1;
    flag.p  = (psw >> 2) & 1;
    flag.ac = (psw >> 4) & 1;
    flag.z  = (psw >> 6) & 1;
    flag.s  = (psw >> 7) & 1;

    sp += 2;
}


// XHHL (Exchange stack top with H and L)
void Intel8080::XTHL() {
    uint8_t tempH = h, tempL = l;

    h = memory[sp + 1];
    l = memory[sp];

    memory[sp + 1] = tempH;
    memory[sp] = tempL;
}

// SPHL (Move HL to SP)
void Intel8080::SPHL() { sp = getHL(); }
