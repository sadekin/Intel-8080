#include "cpu.hpp"

uint8_t Intel8080::read(uint16_t addr) { return memory->read(addr); }

void Intel8080::write(uint16_t addr, uint8_t data) {
    memory->write(addr, data);
}

uint8_t Intel8080::inport(uint8_t port) {
    return ioPorts->read(port);
}

void Intel8080::outport(uint8_t port, uint8_t data) {
    ioPorts->write(port, data);
}

void Intel8080::interrupt(uint8_t n) {
    if (!INTE) return;
    push(pc);
    pc = (uint16_t) n << 3;
    INTE = 0;
}


int Intel8080::Execute(int numCycles) {
    cycles = numCycles;

    while (cycles > 0) {
        opcode = read(pc++);
        disassemble(opcode, pc - 1);

        switch (opcode) {
            // 0x00 - 0x0f
            case 0x00: NOP();       break; // NOP
            case 0x01: LXI();       break; // LXI B,d16
            case 0x02: STAX();      break; // STAX B
            case 0x03: INX();       break; // INX B
            case 0x04: INR();       break; // INR B
            case 0x05: DCR();       break; // DCR B
            case 0x06: MVI();       break; // MVI B,d8
            case 0x07: RLC();       break; // RLC
            case 0x08: NOP();       break; // *NOP
            case 0x09: DAD();       break; // DAD B
            case 0x0a: LDAX();      break; // LDAX B
            case 0x0b: DCX();       break; // DXC B
            case 0x0c: INR();       break; // INR C
            case 0x0d: DCR();       break; // DCR C
            case 0x0e: MVI();       break; // MVI C,d8
            case 0x0f: RRC();       break; // RRC

            // 0x10 - 0x1f
            case 0x10: NOP();       break; // *NOP
            case 0x11: LXI();       break; // LXI D,d16
            case 0x12: STAX();      break; // STAX D
            case 0x13: INX();       break; // INX D
            case 0x14: INR();       break; // INR D
            case 0x15: DCR();       break; // DCR D
            case 0x16: MVI();       break; // MVI D,d8
            case 0x17: RAL();       break; // RAL
            case 0x18: NOP();       break; // *NOP
            case 0x19: DAD();       break; // DAD D
            case 0x1a: LDAX();      break; // LDAX D
            case 0x1b: DCX();       break; // DCX D
            case 0x1c: INR();       break; // INR E
            case 0x1d: DCR();       break; // DCR E
            case 0x1e: MVI();       break; // MVI E,d8
            case 0x1f: RAR();       break; // RAR

            // 0x20 - 0x2f
            case 0x20: NOP();       break; // *NOP
            case 0x21: LXI();       break; // LXI H,d16
            case 0x22: SHLD();      break; // SHLD
            case 0x23: INX();       break; // INX H
            case 0x24: INR();       break; // INR H
            case 0x25: DCR();       break; // DCR H
            case 0x26: MVI();       break; // MVI H,d8
            case 0x27: DAA();       break; // DAA
            case 0x28: NOP();       break; // NOP
            case 0x29: DAD();       break; // DAD H
            case 0x2a: LHLD();      break; // LHLD
            case 0x2b: DCX();       break; // DCX H
            case 0x2c: INR();       break; // INR L
            case 0x2d: DCR();       break; // DCR L
            case 0x2e: MVI();       break; // MVI L,d8
            case 0x2f: CMA();       break; // CMA

            // 0x30 - 0x3f
            case 0x30: NOP();       break; // *NOP
            case 0x31: LXI();       break; // LXI SP,d16
            case 0x32: STA();       break; // STA a16
            case 0x33: INX();       break; // INX SP
            case 0x34: INR();       break; // INR M
            case 0x35: DCR();       break; // DCR M
            case 0x36: MVI();       break; // MVI M,d8
            case 0x37: STC();       break; // STC
            case 0x38: NOP();       break; // *NOP
            case 0x39: DAD();       break; // DAD SP
            case 0x3a: LDA();       break; // LDA a16
            case 0x3b: DCX();       break; // DCX SP
            case 0x3c: INR();       break; // INR A
            case 0x3d: DCR();       break; // DCR A
            case 0x3e: MVI();       break; // MVI A,d8
            case 0x3f: CMC();       break; // CMC

            // 0x40 - 0x4f (All MOV)
            case 0x40:
            case 0x41:
            case 0x42:
            case 0x43:
            case 0x44:
            case 0x45:
            case 0x46:
            case 0x47:
            case 0x48:
            case 0x49:
            case 0x4a:
            case 0x4b:
            case 0x4c:
            case 0x4d:
            case 0x4e:
            case 0x4f:

            // 0x50 - 0x5f (All MOV)
            case 0x50:
            case 0x51:
            case 0x52:
            case 0x53:
            case 0x54:
            case 0x55:
            case 0x56:
            case 0x57:
            case 0x58:
            case 0x59:
            case 0x5a:
            case 0x5b:
            case 0x5c:
            case 0x5d:
            case 0x5e:
            case 0x5f:

            // 0x60 - 0x6f (All MOV)
            case 0x60:
            case 0x61:
            case 0x62:
            case 0x63:
            case 0x64:
            case 0x65:
            case 0x66:
            case 0x67:
            case 0x68:
            case 0x69:
            case 0x6a:
            case 0x6b:
            case 0x6c:
            case 0x6d:
            case 0x6e:
            case 0x6f:

            // 0x70 - 0x7f
            case 0x70:
            case 0x71:
            case 0x72:
            case 0x73:
            case 0x74:
            case 0x75: MOV();       break;
            case 0x76: HLT();       break; // HLT
            case 0x77:
            case 0x78:
            case 0x79:
            case 0x7a:
            case 0x7b:
            case 0x7c:
            case 0x7d:
            case 0x7e:
            case 0x7f: MOV();       break; // MOV

            // 0x80 - 0x8f
            case 0x80:
            case 0x81:
            case 0x82:
            case 0x83:
            case 0x84:
            case 0x85:
            case 0x86:
            case 0x87: ADD();       break; // ADD B, C, D, E, H, L, M, A
            case 0x88:
            case 0x89:
            case 0x8a:
            case 0x8b:
            case 0x8c:
            case 0x8d:
            case 0x8e:
            case 0x8f: ADC();       break; // ADC B, C, D, E, H, L, M, A

            // 0x90 - 0x9f
            case 0x90:
            case 0x91:
            case 0x92:
            case 0x93:
            case 0x94:
            case 0x95:
            case 0x96:
            case 0x97: SUB();       break; // SUB B, C, D, E, H, L, M, A
            case 0x98:
            case 0x99:
            case 0x9a:
            case 0x9b:
            case 0x9c:
            case 0x9d:
            case 0x9e:
            case 0x9f: SBB();       break; // SBB B, C, D, E, H, L, M, A


            // 0Xa0 - 0xaf
            case 0xa0:
            case 0xa1:
            case 0xa2:
            case 0xa3:
            case 0xa4:
            case 0xa5:
            case 0xa6:
            case 0xa7: ANA();       break; // ANA B, C, D, E, H, L, M, A
            case 0xa8:
            case 0xa9:
            case 0xaa:
            case 0xab:
            case 0xac:
            case 0xad:
            case 0xae:
            case 0xaf: XRA();       break; // XRA B, C, D, E, H, L, M, A

            // 0xb0 - 0xbf
            case 0xb0:
            case 0xb1:
            case 0xb2:
            case 0xb3:
            case 0xb4:
            case 0xb5:
            case 0xb6:
            case 0xb7: ORA();       break; // ORA B, C, D, E, H, L, M, A
            case 0xb8:
            case 0xb9:
            case 0xba:
            case 0xbb:
            case 0xbc:
            case 0xbd:
            case 0xbe:
            case 0xbf: CMP();       break; // CMP B, C, D, E, H, L, M, A

            // 0xc0 - 0xcf
            case 0xc0: Rccc();      break; // RNZ = RET if not zero (Z = 0)
            case 0xc1: POP();       break; // POP B
            case 0xc2: Jccc();      break; // JNZ a16 = JMP a16 if not zero (Z = 0)
            case 0xc3: JMP();       break; // JMP a16
            case 0xc4: Cccc();      break; // CNZ a16 = CALL a16 if not zero (Z = 0)
            case 0xc5: PUSH();      break; // PUSH B
            case 0xc6: ADI();       break; // ADI d8
            case 0xc7: RST();       break; // RST 0
            case 0xc8: Rccc();      break; // RZ = RET if zero (Z = 1)
            case 0xc9: RET();       break; // RET
            case 0xca: Jccc();      break; // JZ a16 = JMP a16 if zero (Z = 1)
          /*case 0xcb:*/                   // *JMP a16 (Alternative opcode, should not be used)
            case 0xcc: Cccc();      break; // CZ a16 = CALL a16 if zero (Z = 1)
            case 0xcd: CALL();      break; // CALL a16
            case 0xce: ACI();       break; // ACI d8
            case 0xcf: RST();       break; // RST 1

            // 0xd0 - 0xdf
            case 0xd0: Rccc();      break; // RNC = RET if no carry (CY = 0)
            case 0xd1: POP();       break; // POP D
            case 0xd2: Jccc();      break; // JNC a16 = JMP a16 if no carry (CY = 0)
            case 0xd3: OUT();       break; // OUT d8 (port)
            case 0xd4: Cccc();      break; // CNC a16 = CALL a16 if no carry (CY = 0)
            case 0xd5: PUSH();      break; // PUSH D
            case 0xd6: SUI();       break; // SUI d8
            case 0xd7: RST();       break; // RST 2
            case 0xd8: Rccc();      break; // RET if carry (CY = 1)
          /*case 0xd9:*/                   // *RET (Alternative opcode, should not be used)
            case 0xda: Jccc();      break; // JC a16 = JMP a16 if carry (CY = 1)
            case 0xdb: IN();        break; // IN d8/port
            case 0xdc: Cccc();      break; // CC a16 = CALL a16 if carry (CY = 1)
          /*case 0xdd:*/                   // *CALL a16 (Alternative opcode, should not be used)
            case 0xde: SBI();       break; // SBI d8
            case 0xdf: RST();       break; // RST 3

            // 0xe0 - 0xef
            case 0xe0: Rccc();      break; // RPO = RET if parity odd (P = 0)
            case 0xe1: POP();       break; // POP H
            case 0xe2: Jccc();      break; // JPO a16 = JMP a16 if parity odd (P = 0)
            case 0xe3: XTHL();      break; // XTHL
            case 0xe4: Cccc();      break; // CPO a16 = CALL a16 if parity odd (P = 0)
            case 0xe5: PUSH();      break; // PUSH H
            case 0xe6: ANI();       break; // ANI d8
            case 0xe7: RST();       break; // RST 4
            case 0xe8: Rccc();      break; // RPE = RET if parity even (P = 1)
            case 0xe9: PCHL();      break; // PCHL
            case 0xea: Jccc();      break; // JPE a16 = JMP a16 if parity even (P = 1)
            case 0xeb: XCHG();      break; // XCHG
            case 0xec: Cccc();      break; // CPE a16 = CALL a16 if parity even (P = 1)
          /*case 0xed:*/                   // *CALL a16 (Alternative opcode, should not be used)
            case 0xee: XRI();       break; // XRI d8
            case 0xef: RST();       break; // RST 5

            // 0xf0 - 0xff
            case 0xf0: Rccc();      break; // RP = RET if plus (S = 0)
            case 0xf1: POP();       break; // POP PSW
            case 0xf2: Jccc();      break; // JP a16 = JMP a16 if plus (S = 0)
            case 0xf3: DI();        break; // DI
            case 0xf4: Cccc();      break; // CP a16 = CALL a16 if plus (S = 0)
            case 0xf5: PUSH();      break; // PUSH PSW
            case 0xf6: ORI();       break; // ORI d8
            case 0xf7: RST();       break; // RST 6
            case 0xf8: Rccc();      break; // RM = RET if minus (S = 1)
            case 0xf9: SPHL();      break; // SPHL
            case 0xfa: Jccc();      break; // JM a16 = JMP a16 if minus (S = 1)
            case 0xfb: EI();        break; // EI
            case 0xfc: Cccc();      break; // CM a16 = CALL a16 if minus (S = 1)
          /*case 0xfd:*/                   // *CALL a16 (Alternative opcode, should not be used)
            case 0xfe: CPI();       break; // CPI d8
            case 0xff: RST();       break; // RST 7
            default  :              printf("UNRECOGNIZED INSTRUCTION @ %04Xh: %02X\n", pc - 1, opcode); exit(0);
        }
    }
    return cycles;
}