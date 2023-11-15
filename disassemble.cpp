#include "cpu.hpp"
#include <iostream>

/*  Below is a list of some common Intel 8080 assembly language mnemonics along with their meanings.
 *  Note that this list is not exhaustive, as the 8080 has a fairly rich instruction set, but this
 *  covers many of the commonly used instructions. I have left them here for convenience purposes.
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
// LHLD - load HL pair Direct from memory
//  LXI - load register pair Immediate
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
 *
 * returns the number of bytes of the opcode
*/

int Intel8080::disassemble(uint8_t opcode, uint16_t pc) {
    int opBytes = 1;
    printf("%04x  ", pc);

    switch (opcode) {
        // 0x00 - 0x0f
        case 0x00: printf("NOP");                                                                       break;
        case 0x01: printf("LXI   B,#$%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;               break;
        case 0x02: printf("STAX  B");                                                                   break;
        case 0x03: printf("INX   B");                                                                   break;
        case 0x04: printf("INR   B");                                                                   break;
        case 0x05: printf("DCR   B");                                                                   break;
        case 0x06: printf("MVI   B,#$%02x", read(pc + 1)); opBytes = 2;                                 break;
        case 0x07: printf("RLC");                                                                       break;
        case 0x08: printf("NOP");                                                                       break;
        case 0x09: printf("DAD   B");                                                                   break;
        case 0x0a: printf("LDAX  B");                                                                   break;
        case 0x0b: printf("DCX   B");                                                                   break;
        case 0x0c: printf("INR   C");                                                                   break;
        case 0x0d: printf("DCR   C");                                                                   break;
        case 0x0e: printf("MVI   C,#$%02x", read(pc + 1)); opBytes = 2;                                 break;
        case 0x0f: printf("RRC");                                                                       break;

        // 0x10 - 0x1f
        case 0x10: printf("NOP");                                                                       break;
        case 0x11: printf("LXI   D,#$%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;               break;
        case 0x12: printf("STAX  D");                                                                   break;
        case 0x13: printf("INX   D");                                                                   break;
        case 0x14: printf("INR   D");                                                                   break;
        case 0x15: printf("DCR   D");                                                                   break;
        case 0x16: printf("MVI   D,#$%02x", read(pc + 1)); opBytes = 2;                                 break;
        case 0x17: printf("RAL");                                                                       break;
        case 0x18: printf("NOP");                                                                       break;
        case 0x19: printf("DAD   D");                                                                   break;
        case 0x1a: printf("LDAX  D");                                                                   break;
        case 0x1b: printf("DCX   D");                                                                   break;
        case 0x1c: printf("INR   E");                                                                   break;
        case 0x1d: printf("DCR   E");                                                                   break;
        case 0x1e: printf("MVI   E,#$%02x", read(pc + 1)); opBytes = 2;                                 break;
        case 0x1f: printf("RAR");                                                                       break;

        // 0x20 - 0x2f
        case 0x20: printf("RIM");                                                                       break;
        case 0x21: printf("LXI   H,#$%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;               break;
        case 0x22: printf("SHLD  0x%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                 break;
        case 0x23: printf("INX   H");                                                                   break;
        case 0x24: printf("INR   H");                                                                   break;
        case 0x25: printf("DCR   H");                                                                   break;
        case 0x26: printf("MVI   H,#$%02x", read(pc + 1)); opBytes = 2;                                 break;
        case 0x27: printf("DAA");                                                                       break;
        case 0x28: printf("NOP");                                                                       break;
        case 0x29: printf("DAD   H");                                                                   break;
        case 0x2a: printf("LHLD  0x%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                 break;
        case 0x2b: printf("DCX   H");                                                                   break;
        case 0x2c: printf("INR   L");                                                                   break;
        case 0x2d: printf("DCR   L");                                                                   break;
        case 0x2e: printf("MVI   L,#$%02x", read(pc + 1)); opBytes = 2;                                 break;
        case 0x2f: printf("CMA");                                                                       break;

        // 0x30 - 0x3f
        case 0x30: printf("SIM");                                                                       break;
        case 0x31: printf("LXI   SP,#$%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;              break;
        case 0x32: printf("STA   $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0x33: printf("INX   SP");                                                                  break;
        case 0x34: printf("INR   M");                                                                   break;
        case 0x35: printf("DCR   M");                                                                   break;
        case 0x36: printf("MVI   M,#$%02x", read(pc + 1)); opBytes = 2;                                 break;
        case 0x37: printf("STC");                                                                       break;
        case 0x38: printf("NOP");                                                                       break;
        case 0x39: printf("DAD   SP");                                                                  break;
        case 0x3a: printf("LDA   #$%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                 break;
        case 0x3b: printf("DCX   SP");                                                                  break;
        case 0x3c: printf("INR   A");                                                                   break;
        case 0x3d: printf("DCR   A");                                                                   break;
        case 0x3e: printf("MVI   A,#$%02x", read(pc + 1)); opBytes = 2;                                 break;
        case 0x3f: printf("CMC");                                                                       break;

        // 0x40 - 0x4f
        case 0x40: printf("MOV   B,B");                                                                 break;
        case 0x41: printf("MOV   B,C");                                                                 break;
        case 0x42: printf("MOV   B,D");                                                                 break;
        case 0x43: printf("MOV   B,E");                                                                 break;
        case 0x44: printf("MOV   B,H");                                                                 break;
        case 0x45: printf("MOV   B,L");                                                                 break;
        case 0x46: printf("MOV   B,M");                                                                 break;
        case 0x47: printf("MOV   B,A");                                                                 break;
        case 0x48: printf("MOV   C,B");                                                                 break;
        case 0x49: printf("MOV   C,C");                                                                 break;
        case 0x4a: printf("MOV   C,D");                                                                 break;
        case 0x4b: printf("MOV   C,E");                                                                 break;
        case 0x4c: printf("MOV   C,H");                                                                 break;
        case 0x4d: printf("MOV   C,L");                                                                 break;
        case 0x4e: printf("MOV   C,M");                                                                 break;
        case 0x4f: printf("MOV   C,A");                                                                 break;

        // 0x50 - 0x5f
        case 0x50: printf("MOV   D,B");                                                                 break;
        case 0x51: printf("MOV   D,C");                                                                 break;
        case 0x52: printf("MOV   D,D");                                                                 break;
        case 0x53: printf("MOV   D,E");                                                                 break;
        case 0x54: printf("MOV   D,H");                                                                 break;
        case 0x55: printf("MOV   D,L");                                                                 break;
        case 0x56: printf("MOV   D,M");                                                                 break;
        case 0x57: printf("MOV   D,A");                                                                 break;
        case 0x58: printf("MOV   E,B");                                                                 break;
        case 0x59: printf("MOV   E,C");                                                                 break;
        case 0x5a: printf("MOV   E,D");                                                                 break;
        case 0x5b: printf("MOV   E,E");                                                                 break;
        case 0x5c: printf("MOV   E,H");                                                                 break;
        case 0x5d: printf("MOV   E,L");                                                                 break;
        case 0x5e: printf("MOV   E,M");                                                                 break;
        case 0x5f: printf("MOV   E,A");                                                                 break;

        // 0x60 - 0x6f
        case 0x60: printf("MOV   H,B");                                                                 break;
        case 0x61: printf("MOV   H,C");                                                                 break;
        case 0x62: printf("MOV   H,D");                                                                 break;
        case 0x63: printf("MOV   H,E");                                                                 break;
        case 0x64: printf("MOV   H,H");                                                                 break;
        case 0x65: printf("MOV   H,L");                                                                 break;
        case 0x66: printf("MOV   H,M");                                                                 break;
        case 0x67: printf("MOV   H,A");                                                                 break;
        case 0x68: printf("MOV   L,B");                                                                 break;
        case 0x69: printf("MOV   L,C");                                                                 break;
        case 0x6a: printf("MOV   L,D");                                                                 break;
        case 0x6b: printf("MOV   L,E");                                                                 break;
        case 0x6c: printf("MOV   L,H");                                                                 break;
        case 0x6d: printf("MOV   L,L");                                                                 break;
        case 0x6e: printf("MOV   L,M");                                                                 break;
        case 0x6f: printf("MOV   L,A");                                                                 break;

        // 0x70 - 0x7f
        case 0x70: printf("MOV   M,B");                                                                 break;
        case 0x71: printf("MOV   M,C");                                                                 break;
        case 0x72: printf("MOV   M,D");                                                                 break;
        case 0x73: printf("MOV   M,E");                                                                 break;
        case 0x74: printf("MOV   M,H");                                                                 break;
        case 0x75: printf("MOV   M,L");                                                                 break;
        case 0x76: printf("HLT");                                                                       break;
        case 0x77: printf("MOV   M,A");                                                                 break;
        case 0x78: printf("MOV   A,B");                                                                 break;
        case 0x79: printf("MOV   A,C");                                                                 break;
        case 0x7a: printf("MOV   A,D");                                                                 break;
        case 0x7b: printf("MOV   A,E");                                                                 break;
        case 0x7c: printf("MOV   A,H");                                                                 break;
        case 0x7d: printf("MOV   A,L");                                                                 break;
        case 0x7e: printf("MOV   A,M");                                                                 break;
        case 0x7f: printf("MOV   A,A");                                                                 break;

        // 0x80 - 0x8f
        case 0x80: printf("ADD   B");                                                                   break;
        case 0x81: printf("ADD   C");                                                                   break;
        case 0x82: printf("ADD   D");                                                                   break;
        case 0x83: printf("ADD   E");                                                                   break;
        case 0x84: printf("ADD   H");                                                                   break;
        case 0x85: printf("ADD   L");                                                                   break;
        case 0x86: printf("ADD   M");                                                                   break;
        case 0x87: printf("ADD   A");                                                                   break;
        case 0x88: printf("ADC   B");                                                                   break;
        case 0x89: printf("ADC   C");                                                                   break;
        case 0x8a: printf("ADC   D");                                                                   break;
        case 0x8b: printf("ADC   E");                                                                   break;
        case 0x8c: printf("ADC   H");                                                                   break;
        case 0x8d: printf("ADC   L");                                                                   break;
        case 0x8e: printf("ADC   M");                                                                   break;
        case 0x8f: printf("ADC   A");                                                                   break;

        // 0x90 - 0x9f
        case 0x90: printf("SUB   B");                                                                   break;
        case 0x91: printf("SUB   C");                                                                   break;
        case 0x92: printf("SUB   D");                                                                   break;
        case 0x93: printf("SUB   E");                                                                   break;
        case 0x94: printf("SUB   H");                                                                   break;
        case 0x95: printf("SUB   L");                                                                   break;
        case 0x96: printf("SUB   M");                                                                   break;
        case 0x97: printf("SUB   A");                                                                   break;
        case 0x98: printf("SBB   B");                                                                   break;
        case 0x99: printf("SBB   C");                                                                   break;
        case 0x9a: printf("SBB   D");                                                                   break;
        case 0x9b: printf("SBB   E");                                                                   break;
        case 0x9c: printf("SBB   H");                                                                   break;
        case 0x9d: printf("SBB   L");                                                                   break;
        case 0x9e: printf("SBB   M");                                                                   break;
        case 0x9f: printf("SBB   A");                                                                   break;

        // 0xa0 - 0xaf
        case 0xa0: printf("ANA   B");                                                                   break;
        case 0xa1: printf("ANA   C");                                                                   break;
        case 0xa2: printf("ANA   D");                                                                   break;
        case 0xa3: printf("ANA   E");                                                                   break;
        case 0xa4: printf("ANA   H");                                                                   break;
        case 0xa5: printf("ANA   L");                                                                   break;
        case 0xa6: printf("ANA   M");                                                                   break;
        case 0xa7: printf("ANA   A");                                                                   break;
        case 0xa8: printf("XRA   B");                                                                   break;
        case 0xa9: printf("XRA   C");                                                                   break;
        case 0xaa: printf("XRA   D");                                                                   break;
        case 0xab: printf("XRA   E");                                                                   break;
        case 0xac: printf("XRA   H");                                                                   break;
        case 0xad: printf("XRA   L");                                                                   break;
        case 0xae: printf("XRA   M");                                                                   break;
        case 0xaf: printf("XRA   A");                                                                   break;

        // 0xb0 - 0xbf
        case 0xb0: printf("ORA   B");                                                                   break;
        case 0xb1: printf("ORA   C");                                                                   break;
        case 0xb2: printf("ORA   D");                                                                   break;
        case 0xb3: printf("ORA   E");                                                                   break;
        case 0xb4: printf("ORA   H");                                                                   break;
        case 0xb5: printf("ORA   L");                                                                   break;
        case 0xb6: printf("ORA   M");                                                                   break;
        case 0xb7: printf("ORA   A");                                                                   break;
        case 0xb8: printf("CMP   B");                                                                   break;
        case 0xb9: printf("CMP   C");                                                                   break;
        case 0xba: printf("CMP   D");                                                                   break;
        case 0xbb: printf("CMP   E");                                                                   break;
        case 0xbc: printf("CMP   H");                                                                   break;
        case 0xbd: printf("CMP   L");                                                                   break;
        case 0xbe: printf("CMP   M");                                                                   break;
        case 0xbf: printf("CMP   A");                                                                   break;

        // 0xc0 - 0xcf
        case 0xc0: printf("RNZ");                                                                       break;
        case 0xc1: printf("POP   B");                                                                   break;
        case 0xc2: printf("JNZ   $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xc3: printf("JMP   $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xc4: printf("CNZ   $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xc5: printf("PUSH  B");                                                                   break;
        case 0xc6: printf("ADI   #$%02x", read(pc + 1)); opBytes = 2;                                   break;
        case 0xc7: printf("RST   0");                                                                   break;
        case 0xc8: printf("RZ");                                                                        break;
        case 0xc9: printf("RET");                                                                       break;
        case 0xca: printf("JZ    $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xcb: printf("NOP");                                                                       break;
        case 0xcc: printf("CZ    $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xcd: printf("CALL  $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xce: printf("ACI   #$%02x", read(pc + 1)); opBytes = 2;                                   break;
        case 0xcf: printf("RST   1");                                                                   break;

        // 0xd0 - 0xdf
        case 0xd0: printf("RNC");                                                                       break;
        case 0xd1: printf("POP   D");                                                                   break;
        case 0xd2: printf("JNC   $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xd3: printf("OUT   #$%02x", read(pc + 1)); opBytes = 2;                                   break;
        case 0xd4: printf("CNC   $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xd5: printf("PUSH  D");                                                                   break;
        case 0xd6: printf("SUI   #$%02x", read(pc + 1)); opBytes = 2;                                   break;
        case 0xd7: printf("RST   2");                                                                   break;
        case 0xd8: printf("RC");                                                                        break;
        case 0xd9: printf("NOP");                                                                       break;
        case 0xda: printf("JC    $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xdb: printf("IN    #$%02x", read(pc + 1)); opBytes = 2;                                   break;
        case 0xdc: printf("CC    $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xdd: printf("NOP");                                                                       break;
        case 0xde: printf("SBI   #$%02x", read(pc + 1)); opBytes = 2;                                   break;
        case 0xdf: printf("RST   3");                                                                   break;

        // 0xe0 - 0xef
        case 0xe0: printf("RPO");                                                                       break;
        case 0xe1: printf("POP   H");                                                                   break;
        case 0xe2: printf("JPO   $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xe3: printf("XTHL");                                                                      break;
        case 0xe4: printf("CPO   $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xe5: printf("PUSH  H");                                                                   break;
        case 0xe6: printf("ANI   #$%02x", read(pc + 1)); opBytes = 2;                                   break;
        case 0xe7: printf("RST   4");                                                                   break;
        case 0xe8: printf("RPE");                                                                       break;
        case 0xe9: printf("PCHL");                                                                      break;
        case 0xea: printf("JPE   $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xeb: printf("XCHG");                                                                      break;
        case 0xec: printf("CPE   $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xed: printf("NOP");                                                                       break;
        case 0xee: printf("XRI   #$%02x", read(pc + 1)); opBytes = 2;                                   break;
        case 0xef: printf("RST   5");                                                                   break;

        // 0xf0 - 0xff
        case 0xf0: printf("RP");                                                                        break;
        case 0xf1: printf("POP   PSW");                                                                 break;
        case 0xf2: printf("JP    $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xf3: printf("DI");                                                                        break;
        case 0xf4: printf("CP    $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xf5: printf("PUSH  PSW");                                                                 break;
        case 0xf6: printf("ORI   #$%02x", read(pc + 1)); opBytes = 2;                                   break;
        case 0xf7: printf("RST   6");                                                                   break;
        case 0xf8: printf("RM");                                                                        break;
        case 0xf9: printf("SPHL");                                                                      break;
        case 0xfa: printf("JM    $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xfb: printf("EI");                                                                        break;
        case 0xfc: printf("CM    $%02x%02x", read(pc + 2), read(pc + 1)); opBytes = 3;                  break;
        case 0xfd: printf("NOP");                                                                       break;
        case 0xfe: printf("CPI   #$%02x", read(pc + 1)); opBytes = 2;                                   break;
        case 0xff: printf("RST   7");                                                                   break;
    }

//    printf("\t\t\t\t");
//    printf("a = %d, ", reg8[A]);
//    printf("\t");
//    printf("Z = %d, ", GetFlag(Z));
//    printf("S = %d, ", GetFlag(S));
//    printf("P = %d, ", GetFlag(P));
//    printf("CY = %d, ", GetFlag(CY));
//    printf("AC = %d", GetFlag(AC));
//    pc += opBytes;

    printf("\n");
    return opBytes;
}

// Left here just in case the lookup table fails me
//    while (cycles > 0) {
//        opcode = read(pc++);
//        disassemble(opcode, pc - 1);
//
//        switch (opcode) {
//            // 0x00 - 0x0f
//            case 0x00: NOP();       break; // NOP
//            case 0x01: LXI();       break; // LXI B,d16
//            case 0x02: STAX();      break; // STAX B
//            case 0x03: INX();       break; // INX B
//            case 0x04: INR();       break; // INR B
//            case 0x05: DCR();       break; // DCR B
//            case 0x06: MVI();       break; // MVI B,d8
//            case 0x07: RLC();       break; // RLC
//            case 0x08: NOP();       break; // *NOP
//            case 0x09: DAD();       break; // DAD B
//            case 0x0a: LDAX();      break; // LDAX B
//            case 0x0b: DCX();       break; // DXC B
//            case 0x0c: INR();       break; // INR C
//            case 0x0d: DCR();       break; // DCR C
//            case 0x0e: MVI();       break; // MVI C,d8
//            case 0x0f: RRC();       break; // RRC
//
//            // 0x10 - 0x1f
//            case 0x10: NOP();       break; // *NOP
//            case 0x11: LXI();       break; // LXI D,d16
//            case 0x12: STAX();      break; // STAX D
//            case 0x13: INX();       break; // INX D
//            case 0x14: INR();       break; // INR D
//            case 0x15: DCR();       break; // DCR D
//            case 0x16: MVI();       break; // MVI D,d8
//            case 0x17: RAL();       break; // RAL
//            case 0x18: NOP();       break; // *NOP
//            case 0x19: DAD();       break; // DAD D
//            case 0x1a: LDAX();      break; // LDAX D
//            case 0x1b: DCX();       break; // DCX D
//            case 0x1c: INR();       break; // INR E
//            case 0x1d: DCR();       break; // DCR E
//            case 0x1e: MVI();       break; // MVI E,d8
//            case 0x1f: RAR();       break; // RAR
//
//            // 0x20 - 0x2f
//            case 0x20: NOP();       break; // *NOP
//            case 0x21: LXI();       break; // LXI H,d16
//            case 0x22: SHLD();      break; // SHLD
//            case 0x23: INX();       break; // INX H
//            case 0x24: INR();       break; // INR H
//            case 0x25: DCR();       break; // DCR H
//            case 0x26: MVI();       break; // MVI H,d8
//            case 0x27: DAA();       break; // DAA
//            case 0x28: NOP();       break; // NOP
//            case 0x29: DAD();       break; // DAD H
//            case 0x2a: LHLD();      break; // LHLD
//            case 0x2b: DCX();       break; // DCX H
//            case 0x2c: INR();       break; // INR L
//            case 0x2d: DCR();       break; // DCR L
//            case 0x2e: MVI();       break; // MVI L,d8
//            case 0x2f: CMA();       break; // CMA
//
//            // 0x30 - 0x3f
//            case 0x30: NOP();       break; // *NOP
//            case 0x31: LXI();       break; // LXI SP,d16
//            case 0x32: STA();       break; // STA a16
//            case 0x33: INX();       break; // INX SP
//            case 0x34: INR();       break; // INR M
//            case 0x35: DCR();       break; // DCR M
//            case 0x36: MVI();       break; // MVI M,d8
//            case 0x37: STC();       break; // STC
//            case 0x38: NOP();       break; // *NOP
//            case 0x39: DAD();       break; // DAD SP
//            case 0x3a: LDA();       break; // LDA a16
//            case 0x3b: DCX();       break; // DCX SP
//            case 0x3c: INR();       break; // INR A
//            case 0x3d: DCR();       break; // DCR A
//            case 0x3e: MVI();       break; // MVI A,d8
//            case 0x3f: CMC();       break; // CMC
//
//            // 0x40 - 0x4f (All MOV)
//            case 0x40:
//            case 0x41:
//            case 0x42:
//            case 0x43:
//            case 0x44:
//            case 0x45:
//            case 0x46:
//            case 0x47:
//            case 0x48:
//            case 0x49:
//            case 0x4a:
//            case 0x4b:
//            case 0x4c:
//            case 0x4d:
//            case 0x4e:
//            case 0x4f:
//
//            // 0x50 - 0x5f (All MOV)
//            case 0x50:
//            case 0x51:
//            case 0x52:
//            case 0x53:
//            case 0x54:
//            case 0x55:
//            case 0x56:
//            case 0x57:
//            case 0x58:
//            case 0x59:
//            case 0x5a:
//            case 0x5b:
//            case 0x5c:
//            case 0x5d:
//            case 0x5e:
//            case 0x5f:
//
//            // 0x60 - 0x6f (All MOV)
//            case 0x60:
//            case 0x61:
//            case 0x62:
//            case 0x63:
//            case 0x64:
//            case 0x65:
//            case 0x66:
//            case 0x67:
//            case 0x68:
//            case 0x69:
//            case 0x6a:
//            case 0x6b:
//            case 0x6c:
//            case 0x6d:
//            case 0x6e:
//            case 0x6f:
//
//            // 0x70 - 0x7f
//            case 0x70:
//            case 0x71:
//            case 0x72:
//            case 0x73:
//            case 0x74:
//            case 0x75: MOV();       break;
//            case 0x76: HLT();       break; // HLT
//            case 0x77:
//            case 0x78:
//            case 0x79:
//            case 0x7a:
//            case 0x7b:
//            case 0x7c:
//            case 0x7d:
//            case 0x7e:
//            case 0x7f: MOV();       break; // MOV
//
//            // 0x80 - 0x8f
//            case 0x80:
//            case 0x81:
//            case 0x82:
//            case 0x83:
//            case 0x84:
//            case 0x85:
//            case 0x86:
//            case 0x87: ADD();       break; // ADD B, C, D, E, H, L, M, A
//            case 0x88:
//            case 0x89:
//            case 0x8a:
//            case 0x8b:
//            case 0x8c:
//            case 0x8d:
//            case 0x8e:
//            case 0x8f: ADC();       break; // ADC B, C, D, E, H, L, M, A
//
//            // 0x90 - 0x9f
//            case 0x90:
//            case 0x91:
//            case 0x92:
//            case 0x93:
//            case 0x94:
//            case 0x95:
//            case 0x96:
//            case 0x97: SUB();       break; // SUB B, C, D, E, H, L, M, A
//            case 0x98:
//            case 0x99:
//            case 0x9a:
//            case 0x9b:
//            case 0x9c:
//            case 0x9d:
//            case 0x9e:
//            case 0x9f: SBB();       break; // SBB B, C, D, E, H, L, M, A
//
//
//            // 0Xa0 - 0xaf
//            case 0xa0:
//            case 0xa1:
//            case 0xa2:
//            case 0xa3:
//            case 0xa4:
//            case 0xa5:
//            case 0xa6:
//            case 0xa7: ANA();       break; // ANA B, C, D, E, H, L, M, A
//            case 0xa8:
//            case 0xa9:
//            case 0xaa:
//            case 0xab:
//            case 0xac:
//            case 0xad:
//            case 0xae:
//            case 0xaf: XRA();       break; // XRA B, C, D, E, H, L, M, A
//
//            // 0xb0 - 0xbf
//            case 0xb0:
//            case 0xb1:
//            case 0xb2:
//            case 0xb3:
//            case 0xb4:
//            case 0xb5:
//            case 0xb6:
//            case 0xb7: ORA();       break; // ORA B, C, D, E, H, L, M, A
//            case 0xb8:
//            case 0xb9:
//            case 0xba:
//            case 0xbb:
//            case 0xbc:
//            case 0xbd:
//            case 0xbe:
//            case 0xbf: CMP();       break; // CMP B, C, D, E, H, L, M, A
//
//            // 0xc0 - 0xcf
//            case 0xc0: Rccc();      break; // RNZ = RET if not zero (Z = 0)
//            case 0xc1: POP();       break; // POP B
//            case 0xc2: Jccc();      break; // JNZ a16 = JMP a16 if not zero (Z = 0)
//            case 0xc3: JMP();       break; // JMP a16
//            case 0xc4: Cccc();      break; // CNZ a16 = CALL a16 if not zero (Z = 0)
//            case 0xc5: PUSH();      break; // PUSH B
//            case 0xc6: ADI();       break; // ADI d8
//            case 0xc7: RST();       break; // RST 0
//            case 0xc8: Rccc();      break; // RZ = RET if zero (Z = 1)
//            case 0xc9: RET();       break; // RET
//            case 0xca: Jccc();      break; // JZ a16 = JMP a16 if zero (Z = 1)
//          /*case 0xcb:*/                   // *JMP a16 (Alternative opcode, should not be used)
//            case 0xcc: Cccc();      break; // CZ a16 = CALL a16 if zero (Z = 1)
//            case 0xcd: CALL();      break; // CALL a16
//            case 0xce: ACI();       break; // ACI d8
//            case 0xcf: RST();       break; // RST 1
//
//            // 0xd0 - 0xdf
//            case 0xd0: Rccc();      break; // RNC = RET if no carry (CY = 0)
//            case 0xd1: POP();       break; // POP D
//            case 0xd2: Jccc();      break; // JNC a16 = JMP a16 if no carry (CY = 0)
//            case 0xd3: OUT();       break; // OUT d8 (port)
//            case 0xd4: Cccc();      break; // CNC a16 = CALL a16 if no carry (CY = 0)
//            case 0xd5: PUSH();      break; // PUSH D
//            case 0xd6: SUI();       break; // SUI d8
//            case 0xd7: RST();       break; // RST 2
//            case 0xd8: Rccc();      break; // RET if carry (CY = 1)
//          /*case 0xd9:*/                   // *RET (Alternative opcode, should not be used)
//            case 0xda: Jccc();      break; // JC a16 = JMP a16 if carry (CY = 1)
//            case 0xdb: IN();        break; // IN d8/port
//            case 0xdc: Cccc();      break; // CC a16 = CALL a16 if carry (CY = 1)
//          /*case 0xdd:*/                   // *CALL a16 (Alternative opcode, should not be used)
//            case 0xde: SBI();       break; // SBI d8
//            case 0xdf: RST();       break; // RST 3
//
//            // 0xe0 - 0xef
//            case 0xe0: Rccc();      break; // RPO = RET if parity odd (P = 0)
//            case 0xe1: POP();       break; // POP H
//            case 0xe2: Jccc();      break; // JPO a16 = JMP a16 if parity odd (P = 0)
//            case 0xe3: XTHL();      break; // XTHL
//            case 0xe4: Cccc();      break; // CPO a16 = CALL a16 if parity odd (P = 0)
//            case 0xe5: PUSH();      break; // PUSH H
//            case 0xe6: ANI();       break; // ANI d8
//            case 0xe7: RST();       break; // RST 4
//            case 0xe8: Rccc();      break; // RPE = RET if parity even (P = 1)
//            case 0xe9: PCHL();      break; // PCHL
//            case 0xea: Jccc();      break; // JPE a16 = JMP a16 if parity even (P = 1)
//            case 0xeb: XCHG();      break; // XCHG
//            case 0xec: Cccc();      break; // CPE a16 = CALL a16 if parity even (P = 1)
//          /*case 0xed:*/                   // *CALL a16 (Alternative opcode, should not be used)
//            case 0xee: XRI();       break; // XRI d8
//            case 0xef: RST();       break; // RST 5
//
//            // 0xf0 - 0xff
//            case 0xf0: Rccc();      break; // RP = RET if plus (S = 0)
//            case 0xf1: POP();       break; // POP PSW
//            case 0xf2: Jccc();      break; // JP a16 = JMP a16 if plus (S = 0)
//            case 0xf3: DI();        break; // DI
//            case 0xf4: Cccc();      break; // CP a16 = CALL a16 if plus (S = 0)
//            case 0xf5: PUSH();      break; // PUSH PSW
//            case 0xf6: ORI();       break; // ORI d8
//            case 0xf7: RST();       break; // RST 6
//            case 0xf8: Rccc();      break; // RM = RET if minus (S = 1)
//            case 0xf9: SPHL();      break; // SPHL
//            case 0xfa: Jccc();      break; // JM a16 = JMP a16 if minus (S = 1)
//            case 0xfb: EI();        break; // EI
//            case 0xfc: Cccc();      break; // CM a16 = CALL a16 if minus (S = 1)
//          /*case 0xfd:*/                   // *CALL a16 (Alternative opcode, should not be used)
//            case 0xfe: CPI();       break; // CPI d8
//            case 0xff: RST();       break; // RST 7
//            default  :              printf("UNRECOGNIZED INSTRUCTION @ %04Xh: %02X\n", pc - 1, opcode); exit(0);
//        }
//    }