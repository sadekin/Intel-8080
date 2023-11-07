#include <iostream>
#include <fstream>
#include <vector>


/*  Below is a list of some common Intel 8080 assembly language mnemonics along with their meanings.
 *  Note that this list is not exhaustive, as the 8080 has a fairly rich instruction set, but this
 *  covers many of the commonly used instructions:
 * */

//  ACI - Add Immediate with Carry
//  ADC - Add with Carry
//  ADD - Add without Carry
//  ADI - Add Immediate (without carry)
//  ANA - AND Accumulator (logical AND)
//  ANI - AND Immediate with Accumulator
// CALL - Call Subroutine
//   CC - Call on Carry
//   CM - Call if Minus
//  CMA - Complement Accumulator
//  CMC - Complement Carry Flag
//  CMP - Compare Accumulator with Register
//  CNC - Call on No Carry
//  CNZ - Call on Not Zero
//   CP - Call if Positive
//  CPE - Call if Parity Even
//  CPI - Compare Immediate with Accumulator
//  CPO - Call if Parity Odd
//   CZ - Call on Zero
//  DAA - Decimal Adjust Accumulator
//  DAD - Double Add (Add register pair to HL)
//  DCR - Decrement Register or Memory
//  DCX - Decrement Register Pair
//   DI - Disable Interrupts
//   EI - Enable Interrupts
//  HLT - Halt
//   IN - Input from Port
//  INR - Increment Register or Memory
//  INX - INcrement register pair eXchange
//   JC - Jump on Carry
//   JM - Jump on Minus
//  JMP - Jump Unconditionally
//  JNC - Jump on No Carry
//  JNZ - Jump on Not Zero
//   JP - Jump on Positive
//  JPE - Jump on Parity Even
//  JPO - Jump on Parity Odd
//   JZ - Jump on Zero
//  LDA - LoaD Accumulator Directly from Memory
// LDAX - LoaD Accumulator Indirectly (from BC or DE)
// LHLD - Load HL pair Direct from memory
//  LXI - Load register pair Immediate
//  MOV - MOVe data between registers or between register and memory
//  MVI - MoVe Immediate data to register or memory
//  NOP - No OPeration
//  ORA - OR Accumulator (logical OR)
//  ORI - OR Immediate with Accumulator
//  OUT - Output to Port
// PCHL - Move HL to Program Counter
//  POP - POP data from stack to register pair
// PUSH - PUSH register pair data onto stack
//  RAL - Rotate Accumulator Left
//  RAR - Rotate Accumulator Right
//   RC - Return on Carry
//  RET - RETurn from subroutine
//  RLC - Rotate Left through Carry
//   RM - Return on Minus
//  RNC - Return on No Carry
//  RNZ - Return on Not Zero
//   RP - Return on Positive
//  RPE - Return on Parity Even
//  RPO - Return on Parity Odd
//  RRC - Rotate Right through Carry
//  RST - ReSTart (call to fixed address)
//  SBB - SuBtract with Borrow (accumulator - register - carry)
//  SBI - Subtract Immediate with Borrow
// SHLD - Store HL Direct to memory
//  SIM - Set Interrupt Mask
// SPHL - Move HL to Stack Pointer
//  STA - STore Accumulator Directly in Memory
// STAX - STore Accumulator Indirectly (into address pointed by BC or DE)
//  STC - SeT Carry
//  SUB - SUBtract without carry
//  SUI - Subtract Immediate (without carry)
// XCHG - EXCHanGe HL with DE
//  XRA - Exclusive OR Accumulator
//  XRI - Exclusive OR Immediate with Accumulator
// XTHL - eXchange Top of stack with HL


/* Translates a stream of hex numbers back into assembly language source.
 * The 8080 is little endian, and endianess is easily dealt with by
 * reading and writing one byte at a time.
 *
 * *buffer is a valid pointer to 8080 assembly code
 * pc is the current offset into the code
 *
 * returns the number of bytes of the opcode
*/
int Disassemble8080Op(uint8_t* buffer, int pc) {
    int opcodeBytes = 1;

    printf("%04x  ", pc);

    switch (buffer[pc]) {
        case 0x00:
            printf("NOP");
            break;
        case 0x01:  // B <- byte 3, C <- byte 2
            printf("LXI    B,#$%02x%02x", buffer[pc + 2], buffer[pc + 1]);
            opcodeBytes = 3;
            break;
        case 0x02:  // (BC) <- A
            printf("STAX    B");
            break;
        case 0x03:  // BC <- BC+1
            printf("INX    B");
            break;
        case 0x04:  // B <- B+1
            printf("INR    B");
            break;
        case 0x05:  // B <- B-1
            printf("DCR    B");
            break;
        case 0x06:  // B <- byte 2
            printf("MVI    B,#$%02x", buffer[pc + 1]);
            opcodeBytes = 2;
            break;
        case 0x07:  // A = A << 1; bit 0 = prev bit 7; CY = prev bit 7
            printf("RLC");
            break;
        case 0x08:
            printf("NOP");
            break;
        case 0x09:  // HL = HL + BC
            printf("DAD    B");
            break;
        case 0x0a:  // A <- (BC)
            printf("LDAX    B");
            break;
        case 0x0b:  // BC = BC-1
            printf("DCX    B");
            break;
        case 0x0c:  // C <- C+1
            printf("INR    C");
            break;
        case 0x0d:  // 	C <- C-1
            printf("DCR    C");
            break;
        case 0x0e:  // C <- byte 2
            printf("MVI    C,#$%02x", buffer[pc + 1]);
            opcodeBytes = 2;
            break;
        case 0x0f:  // A = A >> 1; bit 7 = prev bit 0; CY = prev bit 0
            printf("RRC");
            break;
        case 0x10:
            printf("NOP");
            break;
        case 0x11:  // D <- byte 3, E <- byte 2
            printf("LXI    D,#$%02x%02x", buffer[pc + 2], buffer[pc + 1]);
            opcodeBytes = 3;
            break;
        case 0x12:  // (DE) <- A
            printf("STAX    D");
            break;
        case 0x13:  // DE <- DE + 1
            printf("INX    D");
            break;
        case 0x14:  // D <- D+1
            printf("INR    D");
            break;
        case 0x15:  // D <- D-1
            printf("DCR    D");
            break;
        case 0x16:  // D <- byte 2
            printf("MVI    D,#$%02x", buffer[pc + 1]);
            opcodeBytes = 2;
            break;
        case 0x17:  // A = A << 1; bit 0 = prev CY; CY = prev bit 7
            printf("RAL");
            break;
        case 0x18:
            printf("NOP");
            break;
        case 0x19:  // HL = HL + DE
            printf("DAD    D");
            break;
        case 0x1a:  // A <- (DE)
            printf("LDAX    D");
            break;
        case 0x1b:  // DE = DE-1
            printf("DCX    D");
            break;
        case 0x1c:  // E <- E+1
            printf("INR    E");
            break;
        case 0x1d:  // E <- E-1
            printf("DCR    E");
            break;
        case 0x1e:  // E <- byte 2
            printf("MVI    E,#$%02x", buffer[pc + 1]);
            opcodeBytes = 2;
            break;
        case 0x1f:  // A = A >> 1; bit 7 = prev bit 7; CY = prev bit 0
            printf("RAR");
            break;
        case 0x20:  // special
            printf("RIM");
            break;
        case 0x21:  // H <- byte 3, L <- byte 2
            printf("LXI    H,#$%02x%02x", buffer[pc + 2], buffer[pc + 1]);
            opcodeBytes = 3;
            break;
        case 0x22:  // (adr) <- L; (adr+1) <- H
            printf("SHLD    0x%02x%02x", buffer[pc + 2], buffer[pc + 1]);
            opcodeBytes = 3;
            break;
        case 0x23:  // HL <- HL + 1
            printf("INX    H");
            break;
        case 0x24:  // H <- H+1
            printf("INR    H");
            break;
        case 0x25:  // H <- H-1
            printf("DCR    H");
            break;
        case 0x26:  // L <- byte 2
            printf("MVI    H,#$%02x", buffer[pc + 1]);
            opcodeBytes = 2;
            break;
        case 0x27:  // special
            printf("DAA");
            break;
        case 0x28:
            printf("NOP");
            break;
        case 0x29:  // HL = HL + HI
            printf("DAD    H");
            break;
        case 0x2a:  // L <- (adr); H <- (adr+1)
            printf("LHLD    0x%02x%02x", buffer[pc + 2], buffer[pc + 1]);
            opcodeBytes = 3;
            break;
        case 0x2b:  // HL = HL-1
            printf("DCX    H");
            break;
        case 0x2c:  // L <- L+1
            printf("INR    L");
            break;
        case 0x2d:  // L <- L-1
            printf("DCR    L");
            break;
        case 0x2e:  // L <- byte 2
            printf("MVI    L,#$%02x", buffer[pc + 1]);
            opcodeBytes = 2;
            break;
        case 0x2f:  // A <- !A
            printf("CMA");
            break;
        case 0x30:  // special
            printf("SIM");
            break;
        case 0x31:  // SP.hi <- byte 3, SP.lo <- byte 2
            printf("LXI    SP,#$%02x%02x", buffer[pc + 2], buffer[pc + 1]);
            opcodeBytes = 3;
            break;
        case 0x32:  // (adr) <- A
            printf("STA    $%02x%02x", buffer[pc + 2], buffer[pc + 1]);
            opcodeBytes = 3;
            break;
        case 0x33:  // SP = SP + 1
            printf("INX    SP");
            break;
        case 0x34:  // (HL) <- (HL)+1
            printf("INR    M");
            break;
        case 0x35:  // (HL) <- (HL)-1
            printf("DCR    M");
            break;
        case 0x36:  // (HL) <- byte 2
            printf("MVI    M,#$%02x", buffer[pc + 1]);
            opcodeBytes = 2;
            break;
        case 0x37:  // CY = 1
            printf("STC");
            break;
        case 0x38:
            printf("NOP");
            break;
        case 0x39:  // HL = HL + SP
            printf("DAD    SP");
            break;
        case 0x3a:  // A <- (adr)
            printf("LDA    #$%02x%02x", buffer[pc + 2], buffer[pc + 1]);
            opcodeBytes = 3;
            break;
        case 0x3b:  // SP = SP-1
            printf("DCX    SP");
            break;
        case 0x3c:  // A <- A+1
            printf("INR    A");
            break;
        case 0x3d:  // A <- A-1
            printf("DCR    A");
            break;
        case 0x3e:  // A <- byte 2
            printf("MVI    A,#$%02x", buffer[pc + 1]);
            opcodeBytes = 2;
            break;
        case 0x3f:  // CY = !CY
            printf("CMC");
            break;
        case 0x40:  // B <- B
            printf("MOV    B,B");
            break;
        case 0x41:  // B <- C
            printf("MOV    B,C");
            break;
        case 0x42:  // B <- D
            printf("MOV    B,D");
            break;
        case 0x43:  // B <- E
            printf("MOV    B,E");
            break;
        case 0x44:  // B <- H
            printf("MOV    B,H");
            break;
        case 0x45:  // B <- L
            printf("MOV    B,L");
            break;
        case 0x46:  // B <- (HL)
            printf("MOV    B,M");
            break;
        case 0x47:  // B <- A
            printf("MOV    B,A");
            break;
        case 0x48:  // C <- B
            printf("MOV    C,B");
            break;
        case 0x49:  // C <- C
            printf("MOV    C,C");
            break;
        case 0x4a:  // C <- D
            printf("MOV    C,D");
            break;
        case 0x4b:  // C <- E
            printf("MOV    C,E");
            break;
        case 0x4c:  // C <- H
            printf("MOV    C,H");
            break;
        case 0x4d:  // C <- L
            printf("MOV    C,L");
            break;
        case 0x4e:  // C <- (HL)
            printf("MOV    C,M");
            break;
        case 0x4f:  // C <- A
            printf("MOV    C,A");
            break;
        case 0x50:  // D <- B
            printf("MOV    D,B");
            break;
        case 0x51:  // D <- C
            printf("MOV    D,C");
            break;
        case 0x52:  // D <- D
            printf("MOV    D,D");
            break;
        case 0x53:  // D <- E
            printf("MOV    D,E");
            break;
        case 0x54:  // D <- H
            printf("MOV    D,H");
            break;
        case 0x55:  // D <- L
            printf("MOV    D,L");
            break;
        case 0x56:  // D <- (HL)
            printf("MOV    D,M");
            break;
        case 0x57:  // D <- A
            printf("MOV    D,A");
            break;
        case 0x58:  // E <- B
            printf("MOV    E,B");
            break;
        case 0x59:  // E <- C
            printf("MOV    E,C");
            break;
        case 0x5a:  // E <- D
            printf("MOV    E,D");
            break;
        case 0x5b:  // E <- E
            printf("MOV    E,E");
            break;
        case 0x5c:  // E <- H
            printf("MOV    E,H");
            break;
        case 0x5d:  // E <- L
            printf("MOV    E,L");
            break;
        case 0x5e:  // E <- (HL)
            printf("MOV    E,M");
            break;
        case 0x5f:  // E <- A
            printf("MOV    E,A");
            break;
        case 0x60:  // H <- B
            printf("MOV    H,B");
            break;
        case 0x61:  // H <- C
            printf("MOV    H,C");
            break;
        case 0x62:  // H <- D
            printf("MOV    H,D");
            break;
        case 0x63:  // H <- E
            printf("MOV    H,E");
            break;
        case 0x64:  // H <- H
            printf("MOV    H,H");
            break;
        case 0x65:  // H <- L
            printf("MOV    H,L");
            break;
        case 0x66:  // H <- (HL)
            printf("MOV    H,M");
            break;
        case 0x67:  // H <- A
            printf("MOV    H,A");
            break;
        case 0x68:  // L <- B
            printf("MOV    L,B");
            break;
        case 0x69:  // L <- C
            printf("MOV    L,C");
            break;
        case 0x6a:  // L <- D
            printf("MOV    L,D");
            break;
        case 0x6b:  // L <- E
            printf("MOV    L,E");
            break;
        case 0x6c:  // L <- H
            printf("MOV    L,H");
            break;
        case 0x6d:  // L <- L
            printf("MOV    L,L");
            break;
        case 0x6e:  // L <- (HL)
            printf("MOV    L,M");
            break;
        case 0x6f:  // L <- A
            printf("MOV    L,A");
            break;
        case 0x70:  // (HL) <- B
            printf("MOV    M,B");
            break;
        case 0x71:  // (HL) <- C
            printf("MOV    M,C");
            break;
        case 0x72:  // (HL) <- D
            printf("MOV    M,D");
            break;
        case 0x73:  // (HL) <- E
            printf("MOV    M,E");
            break;
        case 0x74:  // (HL) <- H
            printf("MOV    M,H");
            break;
        case 0x75:  // (HL) <- L
            printf("MOV    M,L");
            break;
        case 0x76:  // special
            printf("HLT");
            break;
        case 0x77:  // (HL) <- A
            printf("MOV    M,A");
            break;
        case 0x78:  // A <- B
            printf("MOV    A,B");
            break;
        case 0x79:  // A <- C
            printf("MOV    A,C");
            break;
        case 0x7a:  // A <- D
            printf("MOV    A,D");
            break;
        case 0x7b:  // A <- E
            printf("MOV    A,E");
            break;
        case 0x7c:  // A <- H
            printf("MOV    A,H");
            break;
        case 0x7d:  // A <- L
            printf("MOV    A,L");
            break;
        case 0x7e:  // A <- (HL)
            printf("MOV    A,M");
            break;
        case 0x7f:  // A <- A
            printf("MOV    A,A");
            break;
        case 0x80:  // A <- A + B
            printf("ADD    B");
            break;
        case 0x81:  // A <- A + C
            printf("ADD    C");
            break;
        case 0x82:  // A <- A + D
            printf("ADD    D");
            break;
        case 0x83:  // A <- A + E
            printf("ADD    E");
            break;
        case 0x84:  // A <- A + H
            printf("ADD    H");
            break;
        case 0x85:  // A <- A + L
            printf("ADD    L");
            break;
        case 0x86:  // A <- A + (HL)
            printf("ADD    M");
            break;
        case 0x87:  // A <- A + A
            printf("ADD    A");
            break;
        case 0x88:  // A <- A + B + CY
            printf("ADC    B");
            break;
        case 0x89:  // A <- A + C + CY
            printf("ADC    C");
            break;
        case 0x8a:  // A <- A + D + CY
            printf("ADC    D");
            break;
        case 0x8b:  // A <- A + E + CY
            printf("ADC    E");
            break;
        case 0x8c:  // A <- A + H + CY
            printf("ADC    H");
            break;
        case 0x8d:  // A <- A + L + CY
            printf("ADC    L");
            break;
        case 0x8e:  // A <- A + (HL) + CY
            printf("ADC    M");
            break;
        case 0x8f:  // A <- A + A + CY
            printf("ADC    A");
            break;
        case 0x90:  // A <- A - B
            printf("SUB    B");
            break;
        case 0x91:  // A <- A - C
            printf("SUB    C");
            break;
        case 0x92:  // A <- A - D
            printf("SUB    D");
            break;
        case 0x93:  // A <- A - E
            printf("SUB    E");
            break;
        case 0x94:  // A <- A - H
            printf("SUB    H");
            break;
        case 0x95:  // A <- A - L
            printf("SUB    L");
            break;
        case 0x96:  // A <- A - (HL)
            printf("SUB    M");
            break;
        case 0x97:  // A <- A - A
            printf("SUB    A");
            break;
        case 0x98:  // A <- A - B - CY
            printf("SBB    B");
            break;
        case 0x99:  // A <- A - C - CY
            printf("SBB    C");
            break;
        case 0x9a:  // A <- A - D - CY
            printf("SBB    D");
            break;
        case 0x9b:  // A <- A - E - CY
            printf("SBB    E");
            break;
        case 0x9c:  // A <- A - H - CY
            printf("SBB    H");
            break;
        case 0x9d:  // A <- A - L - CY
            printf("SBB    L");
            break;
        case 0x9e:  // A <- A - HL - CY
            printf("SBB    M");
            break;
        case 0x9f:  // A <- A - A - CY
            printf("SBB    A");
            break;
        case 0xa0:  // A <- A & B
            printf("ANA    B");
            break;
        case 0xa1:  // A <- A & C
            printf("ANA    C");
            break;
        case 0xa2:  // A <- A & D
            printf("ANA    D");
            break;
        case 0xa3:  // A <- A & E
            printf("ANA    E");
            break;
        case 0xa4:  // A <- A & H
            printf("ANA    H");
            break;
        case 0xa5:  // A <- A & L
            printf("ANA    L");
            break;
        case 0xa6:  // A <- A & (HL)
            printf("ANA    M");
            break;
        case 0xa7:  // A <- A & A
            printf("ANA    A");
            break;
        case 0xa8:  // A <- A ^ B
            printf("XRA    B");
            break;
        case 0xa9:  // A <- A ^ C
            printf("XRA    C");
            break;
        case 0xaa:  // A <- A ^ D
            printf("XRA    D");
            break;
        case 0xab:  // A <- A ^ E
            printf("XRA    E");
            break;
        case 0xac:  // A <- A ^ H
            printf("XRA    H");
            break;
        case 0xad:  // A <- A ^ L
            printf("XRA    L");
            break;
        case 0xae:  // A <- A ^ (HL)
            printf("XRA    M");
            break;
        case 0xaf:  // A <- A ^ A
            printf("XRA    A");
            break;
        case 0xb0:  // A <- A | B
            printf("ORA    B");
            break;
        case 0xb1:  // A <- A | C
            printf("ORA    C");
            break;
        case 0xb2:  // A <- A | D
            printf("ORA    D");
            break;
        case 0xb3:  // A <- A | E
            printf("ORA    E");
            break;
        case 0xb4:  // A <- A | H
            printf("ORA    H");
            break;
        case 0xb5:  // A <- A | L
            printf("ORA    L");
            break;
        case 0xb6:  // A <- A | (HL)
            printf("ORA    M");
            break;
        case 0xb7:  // A <- A | A
            printf("ORA    A");
            break;
        case 0xb8:  // A - B
            printf("CMP    B");
            break;
        case 0xb9:  // A - C
            printf("CMP    C");
            break;
        case 0xba:  // A - D
            printf("CMP    D");
            break;
        case 0xbb:  // A - E
            printf("CMP    E");
            break;
        case 0xbc:  // A - H
            printf("CMP    H");
            break;
        case 0xbd:  // A - L
            printf("CMP    L");
            break;
        case 0xbe:  // A - (HL)
            printf("CMP    M");
            break;
        case 0xbf:  // A - A
            printf("CMP    A");
            break;
        case 0xc0:  // if NZ, RET
            printf("RNZ");
            break;
        case 0xc1:  // C <- (sp); B <- (sp+1); sp <- sp+2
            printf("POP    B");
            break;
        case 0xc2:  // if NZ, PC <- adr
            printf("JNZ    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xc3:  // PC <- adr
            printf("JMP    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xc4:  // if NZ, CALL adr
            printf("CNZ    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xc5:  // (sp-2) <- C; (sp-1) <- B; sp <- sp - 2
            printf("PUSH    B");
            break;
        case 0xc6:  // A <- A + byte
            printf("ADI    #$%02x", buffer[pc+1]);
            opcodeBytes = 2;
            break;
        case 0xc7:  // CALL $0
            printf("RST    0");
            break;
        case 0xc8:  // if Z, RET
            printf("RZ");
            break;
        case 0xc9:  // PC.lo <- (sp); PC.hi<-(sp+1); sp <- sp+2
            printf("RET");
            break;
        case 0xca:  // if Z, PC <- adr
            printf("JZ    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xcb:
            printf("NOP");
            break;
        case 0xcc:  // if Z, CALL adr
            printf("CZ    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xcd:  // (sp-1) <- PC.hi; (sp-2) <- PC.lo; sp <- sp+2; PC = adr
            printf("CALL    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xce:  // A <- A + data + CY
            printf("ACI    #$%02x", buffer[pc+1]);
            opcodeBytes = 2;
            break;
        case 0xcf:  // CALL $8
            printf("RST    1");
            break;
        case 0xd0:  // if NCY, RET
            printf("RNC");
            break;
        case 0xd1:  // E <- (sp); D <- (sp+1); sp <- sp+2
            printf("POP    D");
            break;
        case 0xd2:  // if NCY, PC <- adr
            printf("JNC    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xd3:  // special
            printf("OUT    #$%02x", buffer[pc+1]);
            opcodeBytes = 2;
            break;
        case 0xd4:  // if NCY, CALL adr
            printf("CNC    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xd5:  // (sp-2) <- E; (sp-1) <- D; sp <- sp - 2
            printf("PUSH    D");
            break;
        case 0xd6:  // A <- A - data
            printf("SUI    #$%02x", buffer[pc+1]);
            opcodeBytes = 2;
            break;
        case 0xd7:  // CALL $10
            printf("RST    2");
            break;
        case 0xd8:  // if CY, RET
            printf("RC");
            break;
        case 0xd9:
            printf("NOP");
            break;
        case 0xda:  // if CY, PC <- adr
            printf("JC    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xdb:  // special
            printf("IN    #$%02x", buffer[pc+1]);
            opcodeBytes = 2;
            break;
        case 0xdc:  // if CY, CALL adr
            printf("CC    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xdd:
            printf("NOP");
            break;
        case 0xde:  // A <- A - data - CY
            printf("SBI    #$%02x", buffer[pc+1]);
            opcodeBytes = 2;
            break;
        case 0xdf:  // CALL $18
            printf("RST    3");
            break;
        case 0xe0:  // if PO, RET
            printf("RPO");
            break;
        case 0xe1:  // L <- (sp); H <- (sp+1); sp <- sp+2
            printf("POP    H");
            break;
        case 0xe2:  // if PO, PC <- adr
            printf("JPO    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xe3:  // L <-> (SP); H <-> (SP+1)
            printf("XTHL");
            break;
        case 0xe4:  // if PO, CALL adr
            printf("CPO    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xe5:  // (sp-2)<-L; (sp-1)<-H; sp <- sp - 2
            printf("PUSH    H");
            break;
        case 0xe6:  // A <- A & data
            printf("ANI    #$%02x", buffer[pc+1]);
            opcodeBytes = 2;
            break;
        case 0xe7:  // CALL $20
            printf("RST    4");
            break;
        case 0xe8:  // if PE, RET
            printf("RPE");
            break;
        case 0xe9:  // PC.hi <- H; PC.lo <- L
            printf("PCHL");
            break;
        case 0xea:  // if PE, PC <- adr
            printf("JPE    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xeb:  // H <-> D; L <-> E
            printf("XCHG");
            break;
        case 0xec:  // if PE, CALL adr
            printf("CPE    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xed:
            printf("NOP");
            break;
        case 0xee:  // A <- A ^ data
            printf("XRI    #$%02x", buffer[pc+1]);
            opcodeBytes = 2;
            break;
        case 0xef:  // CALL $28
            printf("RST    5");
            break;
        case 0xf0:  // if P, RET
            printf("RP");
            break;
        case 0xf1:  // flags <- (sp); A <- (sp+1); sp <- sp+2
            printf("POP    PSW");
            break;
        case 0xf2:  // if P=1, PC <- adr
            printf("JP    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xf3:  // special
            printf("DI");
            break;
        case 0xf4:  // if P, PC <- adr
            printf("CP    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xf5:  // (sp-2)<-flags; (sp-1)<-A; sp <- sp - 2
            printf("PUSH    PSW");
            break;
        case 0xf6:  // A <- A | data
            printf("ORI    #$%02x", buffer[pc+1]);
            opcodeBytes = 2;
            break;
        case 0xf7:  // CALL $30
            printf("RST    6");
            break;
        case 0xf8:  // if M, RET
            printf("RM");
            break;
        case 0xf9:  // SP=HL
            printf("SPHL");
            break;
        case 0xfa:  // if M, PC <- adr
            printf("JM    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xfb:  // special
            printf("EI");
            break;
        case 0xfc:  // if M, CALL adr
            printf("CM    $%02x%02x", buffer[pc+2], buffer[pc+1]);
            opcodeBytes = 3;
            break;
        case 0xfd:
            printf("NOP");
            break;
        case 0xfe:  // A - data
            printf("CPI    #$%02x", buffer[pc+1]);
            opcodeBytes = 2;
            break;
        case 0xff:  // CALL $38
            printf("RST    7");
            break;
    }

    printf("\n");

    return opcodeBytes;
}

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
        pc += Disassemble8080Op(buffer.data(), pc);
    }

    return 0;
}
