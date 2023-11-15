#include "cpu.hpp"

Intel8080::Intel8080() : INTE(), pc(), sp(), reg8(), memory(nullptr), ioPorts(nullptr){
    memory = new Memory();
    ioPorts = new IOPorts();

    using a = Intel8080;
    lookup = {
            &a::NOP,  &a::LXI, &a::STAX, &a::INX,  &a::INR,  &a::DCR,  &a::MVI, &a::RLC, &a::XXX,  &a::DAD,  &a::LDAX, &a::DCX, &a::INR,  &a::DCR,  &a::MVI, &a::RRC,
            &a::XXX,  &a::LXI, &a::STAX, &a::INX,  &a::INR,  &a::DCR,  &a::MVI, &a::RAL, &a::XXX,  &a::DAD,  &a::LDAX, &a::DCX, &a::INR,  &a::DCR,  &a::MVI, &a::RAR,
            &a::XXX,  &a::LXI, &a::SHLD, &a::INX,  &a::INR,  &a::DCR,  &a::MVI, &a::DAA, &a::XXX,  &a::DAD,  &a::LHLD, &a::DCX, &a::INR,  &a::DCR,  &a::MVI, &a::CMA,
            &a::XXX,  &a::LXI, &a::STA,  &a::INX,  &a::INR,  &a::DCR,  &a::MVI, &a::STC, &a::XXX,  &a::DAD,  &a::LDA,  &a::DCX, &a::INR,  &a::DCR,  &a::MVI, &a::CMC,
            &a::MOV,  &a::MOV, &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV, &a::MOV, &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV, &a::MOV,  &a::MOV,  &a::MOV, &a::MOV,
            &a::MOV,  &a::MOV, &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV, &a::MOV, &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV, &a::MOV,  &a::MOV,  &a::MOV, &a::MOV,
            &a::MOV,  &a::MOV, &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV, &a::MOV, &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV, &a::MOV,  &a::MOV,  &a::MOV, &a::MOV,
            &a::MOV,  &a::MOV, &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV,  &a::HLT, &a::MOV, &a::MOV,  &a::MOV,  &a::MOV,  &a::MOV, &a::MOV,  &a::MOV,  &a::MOV, &a::MOV,
            &a::ADD,  &a::ADD, &a::ADD,  &a::ADD,  &a::ADD,  &a::ADD,  &a::ADD, &a::ADD, &a::ADC,  &a::ADC,  &a::ADC,  &a::ADC, &a::ADC,  &a::ADC,  &a::ADC, &a::ADC,
            &a::SUB,  &a::SUB, &a::SUB,  &a::SUB,  &a::SUB,  &a::SUB,  &a::SUB, &a::SUB, &a::SBB,  &a::SBB,  &a::SBB,  &a::SBB, &a::SBB,  &a::SBB,  &a::SBB, &a::SBB,
            &a::ANA,  &a::ANA, &a::ANA,  &a::ANA,  &a::ANA,  &a::ANA,  &a::ANA, &a::ANA, &a::XRA,  &a::XRA,  &a::XRA,  &a::XRA, &a::XRA,  &a::XRA,  &a::XRA, &a::XRA,
            &a::ORA,  &a::ORA, &a::ORA,  &a::ORA,  &a::ORA,  &a::ORA,  &a::ORA, &a::ORA, &a::CMP,  &a::CMP,  &a::CMP,  &a::CMP, &a::CMP,  &a::CMP,  &a::CMP, &a::CMP,
            &a::Rccc, &a::POP, &a::Jccc, &a::JMP,  &a::Cccc, &a::PUSH, &a::ADI, &a::RST, &a::Rccc, &a::RET,  &a::Jccc, &a::XXX, &a::Cccc, &a::CALL, &a::ACI, &a::RST,
            &a::Rccc, &a::POP, &a::Jccc, &a::OUT,  &a::Cccc, &a::PUSH, &a::SUI, &a::RST, &a::Rccc, &a::XXX,  &a::Jccc, &a::IN,  &a::Cccc, &a::XXX,  &a::SBI, &a::RST,
            &a::Rccc, &a::POP, &a::Jccc, &a::XTHL, &a::Cccc, &a::PUSH, &a::ANI, &a::RST, &a::Rccc, &a::PCHL, &a::Jccc, &a::XCHG,&a::Cccc, &a::XXX,  &a::XRI, &a::RST,
            &a::Rccc, &a::POP, &a::Jccc, &a::DI,   &a::Cccc, &a::PUSH, &a::ORI, &a::RST, &a::Rccc, &a::SPHL, &a::Jccc, &a::EI,  &a::Cccc, &a::XXX,  &a::CPI, &a::RST
    };
}


uint8_t Intel8080::read(uint16_t addr) const { return memory->read(addr); }

void Intel8080::write(uint16_t addr, uint8_t data) const {
    memory->write(addr, data);
}

uint8_t Intel8080::inport(uint8_t port) const {
    return ioPorts->read(port);
}

void Intel8080::outport(uint8_t port, uint8_t data) const {
    ioPorts->write(port, data);
}

void Intel8080::interrupt(uint8_t n) {
    if (!INTE) return;
    push(pc);
    pc = (uint16_t) n << 3;
    INTE = 0;
}


int Intel8080::execute(int numCycles) {
    cycles = numCycles;

    while (cycles > 0) {
        opcode = read(pc++);
        disassemble(opcode, pc - 1);
        if (opcode < 0 || opcode >= 256) XXX();
        (this->*lookup[opcode])();
    }

    return cycles;
}
