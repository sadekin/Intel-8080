#include "cpu.hpp"

// Add immediate to accumulator with carry
void Intel8080::ACI() {
    temp8 = read(pc++);
    temp16 = (uint16_t) reg8[A] + (uint16_t) temp8 + (uint16_t) GetFlag(CY);
    SetFlag(AC, carry(reg8[A], temp8 + GetFlag(CY), temp16, 0x08));
    SetFlag(CY, temp16 & 0xFF00);
    SetZSP(static_cast<uint8_t>(temp16));
    reg8[A] = temp16 & 0xFF;
    cycles -= 7;
}

// Add register to accumulator with carry
void Intel8080::ADC() {
    reg = opcode & 7;
    temp8 = readReg8(reg);
    temp16 = (uint16_t) reg8[A] + (uint16_t) temp8 + (uint16_t) GetFlag(CY);
    SetFlag(AC, carry(reg8[A], temp8 + GetFlag(CY), temp16, 0x08));
    SetFlag(CY, temp16 & 0xFF00);
    SetZSP(static_cast<uint8_t>(temp16));
    reg8[A] = temp16 & 0xFF;
    if (reg == M)
        cycles -= 7;
    else
        cycles -= 4;
}

// Add register/memory to accumulator
void Intel8080::ADD() {
    reg = opcode & 7;
    temp8 = readReg8(reg);
    temp16 = (uint16_t) reg8[A] + (uint16_t) temp8;
    SetFlag(CY, temp16 & 0xFF00);
    SetFlag(AC, carry(reg8[A], temp8, temp16, 0x08));
    SetZSP(static_cast<uint16_t>(temp16));
    reg8[A] = temp16 & 0xFF;
    if (reg == M)
        cycles -= 7;
    else
        cycles -= 4;
}

// Add immediate to accumulator
void Intel8080::ADI() {
    temp8 = read(pc++);
    temp16 = (uint16_t) reg8[A] + (uint16_t) temp8;
    SetFlag(CY, temp16 & 0xFF00);
    SetFlag(AC, carry(reg8[A], temp8, temp16, 0x08));
    SetZSP(static_cast<uint16_t>(temp16));
    reg8[A] = temp16 & 0xFF;
    cycles -= 7;
}

// Logical AND with accumulator
void Intel8080::ANA() {
    reg = opcode & 7;
    temp8 = readReg8(reg);
    SetFlag(AC, (reg8[A] | temp8) & 0x80);
    SetFlag(CY, false);
    reg8[A] &= temp8;
    SetZSP(reg8[A]);
    if (reg == M)
        cycles -= 7;
    else
        cycles -= 4;
}

// Logical AND immediate with accumulator
void Intel8080::ANI() {
    temp8 = read(pc++);
    SetFlag(AC, (reg8[A] | temp8) & 0x80);
    SetFlag(CY, false);
    reg8[A] &= temp8;
    SetZSP(reg8[A]);
    cycles -= 4;
}

// Call
void Intel8080::CALL() {
    temp16 = (uint16_t) read(pc) | (((uint16_t) read(pc + 1)) << 8);
    push(pc + 2);
    pc = temp16;
    cycles -= 17;
}

// Conditional call
void Intel8080::Cccc() {
    temp16 = (uint16_t) read(pc) | (((uint16_t) read(pc + 1)) << 8);
    if (TestCond((opcode >> 3) & 7)) {
        push(pc + 2);
        pc = temp16;
        cycles -= 17;
    } else {
        pc += 2;
        cycles -= 11;
    }
}

// Complement accumulator
void Intel8080::CMA() {
    reg8[A] = ~reg8[A];
    cycles -= 4;
}

// Complement carry
void Intel8080::CMC() {
    reg8[FLAGS] ^= 1;
    cycles -= 4;
}

// Compare register/memory with accumulator
void Intel8080::CMP() {
    reg = opcode & 7;
    temp8 = readReg8(reg);
    temp16 = (uint16_t) reg8[A] - (uint16_t) temp8;
    SetFlag(CY, reg8[A] < temp8);
    SetFlag(AC, (reg8[A] & 0x0F) < (temp8 & 0x0F));
    SetZSP(static_cast<uint8_t>(temp16));
    if (reg == M)
        cycles -= 7;
    else
        cycles -= 4;
}

// Compare immediate with accumulator
void Intel8080::CPI() {
    temp8 = read(pc++);
    temp16 = (uint16_t) reg8[A] - (uint16_t) temp8;
    SetFlag(CY, borrow(reg8[A], temp8, temp16, 0x80));
    SetFlag(AC, borrow(reg8[A], temp8, temp16, 0x08));
    SetZSP(static_cast<uint8_t>(temp16));
    cycles -= 4;
}

// Decimal adjust accumulator
void Intel8080::DAA() {
    temp16 = reg8[A];
    if ((temp16 & 0x0F) > 0x09 || GetFlag(AC)) {
        SetFlag(AC, ((temp16 & 0x0F) + 0x06) & 0xF0);
        temp16 += 0x06;
        if (temp16 & 0xFF00) SetFlag(CY, true);
    }
    if ((temp16 & 0xF0) > 0x90 || GetFlag(CY)) {
        temp16 += 0x60;
        if (temp16 & 0xFF00) SetFlag(CY, true);
    }
    SetZSP(static_cast<uint8_t>(temp16));
    reg8[A] = temp16 & 0xFF;
    cycles -= 4;
}

// Double register add
void Intel8080::DAD() {
    reg = (opcode >> 4) & 3;
    temp32 = (uint32_t) reg16_HL() + (uint32_t) readRP(reg);
    write16RP(2, static_cast<uint16_t>(temp32));
    SetFlag(CY, temp32 & 0xFFFF0000);
    cycles -= 10;
}

// Decrement register/memory
void Intel8080::DCR() {
    reg = (opcode >> 3) & 7;
    temp8 = readReg8(reg);
    SetFlag(AC, (temp8 & 0x0F) == 0);
    SetZSP(temp8 - 1);
    writeReg8(reg, temp8 - 1);
    if (reg == M)
        cycles -= 10;
    else
        cycles -= 5;
}

// Decrement register pair
void Intel8080::DCX() {
    reg = (opcode >> 4) & 3;
    write16RP(reg, readRP(reg) - 1);
    cycles -= 5;
}

// Disable interrupts
void Intel8080::DI() {
    intEnable = 0;
    cycles -= 4;
}

// Enable interrupts
void Intel8080::EI() {
    intEnable = 1;
    cycles -= 4;
}

// Halt
void Intel8080::HLT() {
    pc--;
    cycles -= 7;
}

// Input fom port
void Intel8080::IN() {
    reg8[A] = inport(read(pc++));
    cycles -= 10;
}

// Increment register/memory
void Intel8080::INR() {
    reg = (opcode >> 3) & 7;
    temp8 = readReg8(reg);
    SetFlag(AC, carry(temp8, 1, temp8 + 1, 0x08));
    SetZSP(temp8 + 1);
    writeReg8(reg, temp8 + 1);
    if (reg == M)
        cycles -= 10;
    else
        cycles -= 5;
}

// Increment register pair
void Intel8080::INX() {
    reg = (opcode >> 4) & 3;
    write16RP(reg, readRP(reg) + 1);
    cycles -= 5;
}

// Jump
void Intel8080::JMP() {
    temp16 = (uint16_t) read(pc) | (((uint16_t) read(pc + 1)) << 8);
    pc = temp16;
    cycles -= 10;
}

// Conditional jump
void Intel8080::Jccc() {
    temp16 = (uint16_t) read(pc) | (((uint16_t) read(pc + 1)) << 8);
    if (TestCond((opcode >> 3) & 7))    pc = temp16;
    else                                pc += 2;
    cycles -= 10;
}

// Load accumulator direct
void Intel8080::LDA() {
    temp16 = (uint16_t) read(pc) | ((uint16_t) read(pc + 1) << 8);
    reg8[A] = read(temp16);
    pc += 2;
    cycles -= 13;
}

// Load accumulator indirect
void Intel8080::LDAX() {
    reg = (opcode >> 4) & 3;
    reg8[A] = read(readRP(reg));
    cycles -= 7;
}

// Load H and L direct
void Intel8080::LHLD() {
    temp16 = (uint16_t) read(pc) | ((uint16_t) read(pc + 1) << 8);
    reg8[L] = read(temp16 + 0);
    reg8[H] = read(temp16 + 1);
    pc += 2;
    cycles -= 16;
}

// Load register pair immediate
void Intel8080::LXI() {
    reg = (opcode >> 4) & 3;
    writeRP(reg, read(pc), read(pc + 1));
    pc += 2;
    cycles -= 10;
}

// Move data between register/memory
void Intel8080::MOV() {
    reg  = (opcode >> 3) & 7;
    reg2 = opcode & 7;
    writeReg8(reg, readReg8(reg2));
    if (reg == M || reg2 == M)
        cycles -= 7;
    else
        cycles -= 5;
}

// Move immediate to register/memory
void Intel8080::MVI() {
    reg = (opcode >> 3) & 7;
    writeReg8(reg, read(pc++));
    if (reg == M)
        cycles -= 10;
    else
        cycles -= 7;
}

// No operation
void Intel8080::NOP() {
    cycles -= 4;
}

// Inclusive OR with accumulator
void Intel8080::ORA() {
    reg = opcode & 7;
    reg8[A] |= readReg8(reg);
    SetFlag(AC, false);
    SetFlag(CY, false);
    SetZSP(reg8[A]);
    if (reg == M)
        cycles -= 7;
    else
        cycles -= 4;
}

// Inclusive OR immediate
void Intel8080::ORI() {
    reg8[A] |= read(pc++);
    SetFlag(AC, false);
    SetFlag(CY, false);
    SetZSP(reg8[A]);
    cycles -= 7;
}

// Output to port
void Intel8080::OUT() {
    outport(read(pc++), reg8[A]);
    cycles -= 10;
}

// Move H&L to program counter
void Intel8080::PCHL() {
    pc = reg16_HL();
    cycles -= 5;
}

// Pop
void Intel8080::POP() {
    reg = (opcode >> 4) & 3;
    write16RP_PUSHPOP(reg, pop());
    cycles -= 10;
}


// Push
void Intel8080::PUSH() {
    reg = (opcode >> 4) & 3;
    push(readRP_PUSHPOP(reg));
    cycles -= 11;
}

// Rotate left through carry
void Intel8080::RAL() {
    temp8 = GetFlag(CY);
    SetFlag(CY, reg8[A] & 0x80);
    reg8[A] = (reg8[A] << 1) | temp8;
    cycles -= 4;
}

// Rotate right through carry
void Intel8080::RAR() {
    temp8 = GetFlag(CY);
    SetFlag(CY, reg8[A] & 1);
    reg8[A] = (temp8 << 7) | (reg8[A] >> 1);
    cycles -= 4;
}

// Return
void Intel8080::RET() {
    pc = pop();
    cycles -= 10;
}

// Conditional return
void Intel8080::Rccc() {
    if (TestCond((opcode >> 3) & 7)) {
        pc = pop();
        cycles -= 11;
    } else {
        cycles -= 5;
    }
}

// Rotate accumulator left
void Intel8080::RLC() {
    SetFlag(CY, reg8[A] & 0x80);
    reg8[A] = (reg8[A] << 1) | (reg8[A] >> 7);
    cycles -= 4;
}

// Rotate accumulator right
void Intel8080::RRC() {
    SetFlag(CY, reg8[A] & 1);
    reg8[A] = (reg8[A] << 7) | (reg8[A] >> 1);
    cycles -= 4;
}

// Restart
void Intel8080::RST() {
    push(pc);
    pc = (uint16_t) ((opcode >> 3) & 7) << 3; // Call n * 8
    cycles -= 11;
}

// Subtract register/memory with borrow
void Intel8080::SBB() {
    reg = opcode & 7;
    temp8 = readReg8(reg);
    temp16 = (uint16_t) reg8[A] - (uint16_t) temp8 - (uint16_t) GetFlag(CY);
    SetFlag(AC, borrow(reg8[A], temp8 + GetFlag(CY), temp16, 0x08));
    SetFlag(CY, borrow(reg8[A], temp8 + GetFlag(CY), temp16, 0x80));
    SetZSP(static_cast<uint8_t>(temp16));
    reg8[A] = temp16 & 0xFF;
    if (reg == M)
        cycles -= 7;
    else
        cycles -= 4;
}

// Subtract immediate with borrow
void Intel8080::SBI() {
    temp8 = read(pc++);
    temp16 = (uint16_t) reg8[A] - (uint16_t) temp8 - GetFlag(CY);
    SetFlag(AC, borrow(reg8[A], temp8 + GetFlag(CY), temp16, 0x08));
    SetFlag(CY, borrow(reg8[A], temp8 + GetFlag(CY), temp16, 0x80));
    SetZSP(static_cast<uint8_t>(temp16));
    reg8[A] = temp16 & 0xFF;
    cycles -= 7;
}

// Store H and L direct
void Intel8080::SHLD() {
    temp16 = (uint16_t) read(pc) | ((uint16_t) read(pc + 1) << 8);
    write(temp16 + 0, reg8[L]);
    write(temp16 + 1, reg8[H]);
    pc += 2;
    cycles -= 16;
}

// Move H&L to SP
void Intel8080::SPHL() {
    sp = reg16_HL();
    cycles -= 5;
}

// Store accumulator direct
void Intel8080::STA() {
    temp16 = (uint16_t) read(pc) | ((uint16_t) read(pc + 1) << 8);
    write(temp16, reg8[A]);
    pc += 2;
    cycles -= 13;
}

// Store accumulator indirect
void Intel8080::STAX() {
    reg = (opcode >> 4) & 3;
    write(readRP(reg), reg8[A]);
}

// Set carry
void Intel8080::STC() {
    SetFlag(CY, true);
    cycles -= 4;
}

// Subtract register/memory from accumulator
void Intel8080::SUB() {
    reg = opcode & 7;
    temp8 = readReg8(reg);
    temp16 = (uint16_t) reg8[A] - (uint16_t) temp8;
    SetFlag(AC, borrow(reg8[A], temp8, temp16, 0x08));
    SetFlag(CY, borrow(reg8[A], temp8, temp16, 0x80));
    SetZSP(static_cast<uint8_t>(temp16));
    reg8[A] = temp16 & 0xFF;
    if (reg == M)
        cycles -= 7;
    else
        cycles -= 4;
}

// Subtract immediate from accumulator
void Intel8080::SUI() {
    temp8 = read(pc++);
    temp16 = (uint16_t) reg8[A] - (uint16_t) temp8;
    SetFlag(AC, borrow(reg8[A], temp8, temp16, 0x08));
    SetFlag(CY, borrow(reg8[A], temp8, temp16, 0x80));
    SetZSP(static_cast<uint8_t>(temp16));
    reg8[A] = temp16 & 0xFF;
    cycles -= 7;
}

// Exhange H and L with D and E
void Intel8080::XCHG() {
    temp8 = reg8[H];
    reg8[H] = reg8[D];
    reg8[D] = temp8;
    temp8 = reg8[L];
    reg8[L] = reg8[E];
    reg8[E] = temp8;
    cycles -= 5;
}

// Exclusive OR with accumulator
void Intel8080::XRA() {
    reg = opcode & 7;
    reg8[A] ^= readReg8(reg);
    SetFlag(AC, false);
    SetFlag(CY, false);
    SetZSP(reg8[A]);
    if (reg == M)
        cycles -= 7;
    else
        cycles -= 4;
}

// Exclusive OR immediate with accumulator
void Intel8080::XRI() {
    temp8 = read(pc++);
    reg8[A] ^= temp8;
    SetFlag(AC, false);
    SetFlag(CY, false);
    SetZSP(reg8[A]);
    cycles -= 7;
}

// Exchange H&L with top of stack
void Intel8080::XTHL() {
    temp16 = pop();
    push(reg16_HL());
    write16RP(2, temp16);
    cycles -= 18;
}

// Unimplemented instruction
void Intel8080::XXX() {
    printf("UNRECOGNIZED INSTRUCTION @ %04Xh: %02X\n", pc - 1, opcode);
    exit(0);
}