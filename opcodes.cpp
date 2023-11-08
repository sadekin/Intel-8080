#include "cpu.h"
#include "iostream"

void Intel8080::Emulate8080() {
    uint8_t* opcode = &memory[pc];

    switch (*opcode) {
        // 0x00 - 0x0f
        case 0x00:                                      break; // NOP
        case 0x01: LXI(&b, &c, opcode);                 break;
        case 0x02: STAX(getBC());                       break;
        case 0x03: INX(&b, &c);                         break;
        case 0x04: INR(&b);                             break;
        case 0x05: DCR(&b);                             break;
        case 0x06: MVI(&b, opcode[1]);                  break;
        case 0x07: RLC();                               break;
        case 0x08:                                      break; // NOP
        case 0x09: DAD(getBC());                        break;
        case 0x0a: LDAX(getBC());                       break;
        case 0x0b: DCX(&b, &c);                         break;
        case 0x0c: INR(&c);                             break;
        case 0x0d: DCR(&c);                             break;
        case 0x0e: MVI(&c, opcode[1]);                  break;
        case 0x0f: RRC();                               break;

        // 0x10 - 0x1f
        case 0x10:                                      break; // NOP
        case 0x11: LXI(&d, &e, opcode);                 break;
        case 0x12: STAX(getDE());                       break;
        case 0x13: INX(&d, &e);                         break;
        case 0x14: INR(&d);                             break;
        case 0x15: DCR(&d);                             break;
        case 0x16: MVI(&d, opcode[1]);                  break;
        case 0x17: RAL();                               break;
        case 0x18:                                      break; // NOP
        case 0x19: DAD(getDE());                        break;
        case 0x1a: LDAX(getDE());                       break;
        case 0x1b: DCX(&d, &e);                         break;
        case 0x1c: INR(&e);                             break;
        case 0x1d: DCR(&e);                             break;
        case 0x1e: MVI(&e, opcode[1]);                  break;
        case 0x1f: RAR();                               break;

        // 0x20 - 0x2f
        case 0x20: printf("RIM");                       break; // TODO: special
        case 0x21: LXI(&h, &l, opcode);                 break;
        case 0x22: SHLD(opcode);                        break;
        case 0x23: INX(&h, &l);                         break;
        case 0x24: INR(&h);                             break;
        case 0x25: DCR(&h);                             break;
        case 0x26: MVI(&h, opcode[1]);                  break;
        case 0x27: printf("DAA");                       break; // TODO: special
        case 0x28:                                      break; // NOP
        case 0x29: DAD(getHL());                        break;
        case 0x2a: LHLD(opcode);                        break;
        case 0x2b: DCX(&h, &l);                         break;
        case 0x2c: INR(&l);                             break;
        case 0x2d: DCR(&l);                             break;
        case 0x2e: MVI(&l, opcode[1]);                  break;
        case 0x2f: CMA();                               break;

        // 0x30 - 0x3f
        case 0x30: printf("SIM");                       break; // TODO: special
        case 0x31: LXI_SP(opcode);                      break;
        case 0x32: STA(opcode);                         break;
        case 0x33: INX_SP();                            break;
        case 0x34: INR(&memory[getHL()]);               break;
        case 0x35: DCR(&memory[getHL()]);               break;
        case 0x36: MVI(&memory[getHL()], opcode[1]);    break;
        case 0x37: STC();                               break;
        case 0x38:                                      break; // NOP
        case 0x39: DAD(sp);                             break;
        case 0x3a: LDA(opcode);                         break;
        case 0x3b: DCX_SP();                            break;
        case 0x3c: INR(&a);                             break;
        case 0x3d: DCR(&a);                             break;
        case 0x3e: MVI(&a, opcode[1]);                  break;
        case 0x3f: CMC();                               break;

        // 0x40 - 0x4f
        case 0x40: MOV(&b, &b);                         break;
        case 0x41: MOV(&b, &c);                         break;
        case 0x42: MOV(&b, &d);                         break;
        case 0x43: MOV(&b, &e);                         break;
        case 0x44: MOV(&b, &h);                         break;
        case 0x45: MOV(&b, &l);                         break;
        case 0x46: MOV(&b, &memory[getHL()]);           break;
        case 0x47: MOV(&b, &a);                         break;
        case 0x48: MOV(&c, &b);                         break;
        case 0x49: MOV(&c, &c);                         break;
        case 0x4a: MOV(&c, &d);                         break;
        case 0x4b: MOV(&c, &e);                         break;
        case 0x4c: MOV(&c, &h);                         break;
        case 0x4d: MOV(&c, &l);                         break;
        case 0x4e: MOV(&c, &memory[getHL()]);           break;
        case 0x4f: MOV(&c, &a);                         break;

        // 0x50 - 0x5f
        case 0x50: MOV(&d, &b);                         break;
        case 0x51: MOV(&d, &c);                         break;
        case 0x52: MOV(&d, &d);                         break;
        case 0x53: MOV(&d, &e);                         break;
        case 0x54: MOV(&d, &h);                         break;
        case 0x55: MOV(&d, &l);                         break;
        case 0x56: MOV(&d, &memory[getHL()]);           break;
        case 0x57: MOV(&d, &a);                         break;
        case 0x58: MOV(&e, &b);                         break;
        case 0x59: MOV(&e, &c);                         break;
        case 0x5a: MOV(&e, &d);                         break;
        case 0x5b: MOV(&e, &e);                         break;
        case 0x5c: MOV(&e, &h);                         break;
        case 0x5d: MOV(&e, &l);                         break;
        case 0x5e: MOV(&e, &memory[getHL()]);           break;
        case 0x5f: MOV(&e, &a);                         break;

        // 0x60 - 0x6f
        case 0x60: MOV(&h, &b);                         break;
        case 0x61: MOV(&h, &c);                         break;
        case 0x62: MOV(&h, &d);                         break;
        case 0x63: MOV(&h, &e);                         break;
        case 0x64: MOV(&h, &h);                         break;
        case 0x65: MOV(&h, &l);                         break;
        case 0x66: MOV(&h, &memory[getHL()]);           break;
        case 0x67: MOV(&h, &a);                         break;
        case 0x68: MOV(&l, &b);                         break;
        case 0x69: MOV(&l, &c);                         break;
        case 0x6a: MOV(&l, &d);                         break;
        case 0x6b: MOV(&l, &e);                         break;
        case 0x6c: MOV(&l, &h);                         break;
        case 0x6d: MOV(&l, &l);                         break;
        case 0x6e: MOV(&l, &memory[getHL()]);           break;
        case 0x6f: MOV(&l, &a);                         break;

        // 0x70 - 0x7f
        case 0x70: MOV(&memory[getHL()], &b);           break;
        case 0x71: MOV(&memory[getHL()], &c);           break;
        case 0x72: MOV(&memory[getHL()], &d);           break;
        case 0x73: MOV(&memory[getHL()], &e);           break;
        case 0x74: MOV(&memory[getHL()], &h);           break;
        case 0x75: MOV(&memory[getHL()], &l);           break;
        case 0x76: printf("HLT");                       break; // TODO: HLT
        case 0x77: MOV(&memory[getHL()], &a);           break;
        case 0x78: MOV(&a, &b);                         break;
        case 0x79: MOV(&a, &c);                         break;
        case 0x7a: MOV(&a, &d);                         break;
        case 0x7b: MOV(&a, &e);                         break;
        case 0x7c: MOV(&a, &h);                         break;
        case 0x7d: MOV(&a, &l);                         break;
        case 0x7e: MOV(&a, &memory[getHL()]);           break;
        case 0x7f: MOV(&a, &a);                         break;

        // 0x80 - 0x8f
        case 0x80: ADD(b, false);                       break; // ADD B
        case 0x81: ADD(c, false);                       break; // ADD C
        case 0x82: ADD(d, false);                       break; // ADD D
        case 0x83: ADD(e, false);                       break; // ADD E
        case 0x84: ADD(h, false);                       break; // ADD H
        case 0x85: ADD(l, false);                       break; // ADD L
        case 0x86: ADD(memory[getHL()], false);         break; // ADD M
        case 0x87: ADD(a, false);                       break; // ADD A
        case 0x88: ADD(b, flag.cy);                     break; // ADC B
        case 0x89: ADD(c, flag.cy);                     break; // ADC C
        case 0x8a: ADD(d, flag.cy);                     break; // ADC D
        case 0x8b: ADD(e, flag.cy);                     break; // ADC E
        case 0x8c: ADD(h, flag.cy);                     break; // ADC H
        case 0x8d: ADD(l, flag.cy);                     break; // ADC L
        case 0x8e: ADD(memory[getHL()], flag.cy);       break; // ADC M
        case 0x8f: ADD(a, flag.cy);                     break; // ADC A

        // 0x90 - 0x9f
        case 0x90: SUB(b, false);                       break; // SUB B
        case 0x91: SUB(c, false);                       break; // SUB C
        case 0x92: SUB(d, false);                       break; // SUB D
        case 0x93: SUB(e, false);                       break; // SUB E
        case 0x94: SUB(h, false);                       break; // SUB H
        case 0x95: SUB(l, false);                       break; // SUB L
        case 0x96: SUB(memory[getHL()], false);         break; // SUB M
        case 0x97: SUB(a, false);                       break; // SUB A
        case 0x98: SUB(b, flag.cy);                     break; // SBC B
        case 0x99: SUB(c, flag.cy);                     break; // SBC C
        case 0x9a: SUB(d, flag.cy);                     break; // SBC D
        case 0x9b: SUB(e, flag.cy);                     break; // SBC E
        case 0x9c: SUB(h, flag.cy);                     break; // SBC H
        case 0x9d: SUB(l, flag.cy);                     break; // SBC L
        case 0x9e: SUB(memory[getHL()], flag.cy);       break; // SBC M
        case 0x9f: SUB(a, flag.cy);                     break; // SBC A


        // 0Xa0 - 0xaf
        case 0xa0: ANA(b);                              break;
        case 0xa1: ANA(c);                              break;
        case 0xa2: ANA(d);                              break;
        case 0xa3: ANA(e);                              break;
        case 0xa4: ANA(h);                              break;
        case 0xa5: ANA(l);                              break;
        case 0xa6: ANA(memory[getHL()]);                break;
        case 0xa7: ANA(a);                              break;
        case 0xa8: XRA(b);                              break;
        case 0xa9: XRA(c);                              break;
        case 0xaa: XRA(d);                              break;
        case 0xab: XRA(e);                              break;
        case 0xac: XRA(h);                              break;
        case 0xad: XRA(l);                              break;
        case 0xae: XRA(memory[getHL()]);                break;
        case 0xaf: XRA(a);                              break;

        // 0xb0 - 0xbf
        case 0xb0: ORA(b);                              break;
        case 0xb1: ORA(c);                              break;
        case 0xb2: ORA(d);                              break;
        case 0xb3: ORA(e);                              break;
        case 0xb4: ORA(h);                              break;
        case 0xb5: ORA(l);                              break;
        case 0xb6: ORA(memory[getHL()]);                break;
        case 0xb7: ORA(a);                              break;
        case 0xb8: CMP(b);                              break;
        case 0xb9: CMP(c);                              break;
        case 0xba: CMP(d);                              break;
        case 0xbb: CMP(e);                              break;
        case 0xbc: CMP(h);                              break;
        case 0xbd: CMP(l);                              break;
        case 0xbe: CMP(memory[getHL()]);                break;
        case 0xbf: CMP(a);                              break;

        // 0xc0 - 0xcf
        case 0xc0: RET(flag.z == 0);                    break; // TODO: RNZ check
        case 0xc1: POP(&b, &c);                         break; // POP B
        case 0xc2: JMP(opcode, flag.z == 0);            break; // JNZ a16 = JMP a16 if not zero (Z = 0)
        case 0xc3: JMP(opcode);                         break; // JMP a16
        case 0xc4: CALL(opcode, flag.z == 0);           break; // TODO: CNZ check
        case 0xc5: PUSH(&b, &c);                        break; // PUSH B
        case 0xc6: ADD(opcode[1], false); pc++;         break; // ADI d8
        case 0xc7: RST(0);                              break; // RST 0
        case 0xc8: RET(flag.z == 1);                    break; // RZ = RET if zero (Z = 1)
        case 0xc9: RET();                               break; // RET
        case 0xca: JMP(opcode, flag.z == 1);            break; // JZ a16 = JMP a16 if zero (Z = 1)
        case 0xcb:                                      break; // NOP (should not be used)
        case 0xcc: CALL(opcode, flag.z == 1);           break; // CZ a16 = CALL a16 if zero (Z = 1)
        case 0xcd: CALL(opcode);                        break; // CALL a16
        case 0xce: ADD(opcode[1], flag.cy); pc++;       break; // ACI d8
        case 0xcf: RST(1);                              break; // RST 1

        // 0xd0 - 0xdf
        case 0xd0: RET(flag.cy == 0);                   break; // RNC
        case 0xd1: POP(&d, &e);                         break; // POP D
        case 0xd2: JMP(opcode, flag.cy == 0);           break; // JNC a16 = JMP a16 if no carry (CY = 0)
        case 0xd3:                                      break; // TODO: printf("OUT    #$%02x", buffer[pc+1]); opcodeBytes = 2;
        case 0xd4: CALL(opcode, flag.cy == 0);          break; // CNC a16 = CALL a16 if no carry (CY = 0)
        case 0xd5: PUSH(&d, &e);                        break; // PUSH D
        case 0xd6: SUB(opcode[1], false); pc++;         break; // SUI d8
        case 0xd7: RST(2);                              break; // RST 2
        case 0xd8: RET(flag.cy == 1);                   break; // RET if carry (CY = 1)
        case 0xd9:                                      break; // NOP (should not be used)
        case 0xda: JMP(opcode, flag.cy == 1);           break; // JC a16 = JMP a16 if carry (CY = 1)
        case 0xdb:                                      break; // TODO: printf("IN    #$%02x", buffer[pc+1]); opcodeBytes = 2;
        case 0xdc: CALL(opcode, flag.cy == 1);          break; // CC a16 = CALL a16 if carry (CY = 1)
        case 0xdd:                                      break; // NOP (should not be used)
        case 0xde: SUB(opcode[1], flag.cy); pc++;       break; // SBI d8
        case 0xdf: RST(3);                              break; // RST 3

        // 0xe0 - 0xef
        case 0xe0: RET(flag.p == 0);                    break; // RPO = RET if parity odd (P = 0)
        case 0xe1: POP(&h, &l);                         break; // POP H
        case 0xe2: JMP(opcode, flag.p == 0);            break; // JPO a16 = JMP a16 if parity odd (P = 0)
        case 0xe3: XTHL();                              break; // XTHL
        case 0xe4: CALL(opcode, flag.p == 0);           break; // CPO a16 = CALL a16 if parity odd (P = 0)
        case 0xe5: PUSH(&h, &l);                        break; // PUSH H
        case 0xe6: ANI(opcode[1]); pc++;                break; // ANI d8
        case 0xe7: RST(4);                              break; // RST 4
        case 0xe8: RET(flag.p == 1);                    break; // RPE = RET if parity even (P = 1)
        case 0xe9: PCHL();                              break; // PCHL
        case 0xea: JMP(opcode, flag.p == 1);            break; // JPE a16 = JMP a16 if parity even (P = 1)
        case 0xeb: XCHG();                              break; // XCHG
        case 0xec: CALL(opcode, flag.p == 1);           break; // CPE a16 = CALL a16 if parity even (P = 1)
        case 0xed:                                      break; // NOP (should not be used)
        case 0xee: XRA(opcode[1]); pc++;                break; // XRI d8
        case 0xef: RST(5);                              break; // RST 5

        // 0xf0 - 0xff
        case 0xf0: RET(flag.s == 0);                    break; // RP = RET if plus (S = 0)
        case 0xf1: POP_PSW();                           break; // POP PSW
        case 0xf2: JMP(opcode, flag.s == 0);            break; // JP a16 = JMP a16 if plus (S = 0)
        case 0xf3: printf("DI");                        break; // TODO: special
        case 0xf4: CALL(opcode, flag.s == 0);           break; // CP a16 = CALL a16 if plus (S = 0)
        case 0xf5: PUSH_PSW();                          break; // PUSH PSW
        case 0xf6: ORA(opcode[1]); pc++;                break; // ORI d8
        case 0xf7: RST(6);                              break; // RST 6
        case 0xf8: RET(flag.s == 1);                    break; // RM = RET if minus (S = 1)
        case 0xf9: SPHL();                              break; // SPHL
        case 0xfa: JMP(opcode, flag.s == 1);            break; // JM a16 = JMP a16 if minus (S = 1)
        case 0xfb: printf("EI");                        break; // TODO: special
        case 0xfc: CALL(opcode, flag.s == 1);           break; // CM a16 = CALL a16 if minus (S = 1)
        case 0xfd:                                      break; // NOP (should not be used)
        case 0xfe: CMP(opcode[1]); pc++;                break; // CPI d8
        case 0xff: RST(7);                              break; // RST 7
    }

    pc++;
}



