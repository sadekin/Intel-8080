#include "cpu.hpp"

Intel8080::Intel8080() : intEnable(), pc(), sp(), reg8(), memory(nullptr), ioPorts(nullptr){
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

// Executes a specified number of CPU cycles
int Intel8080::execute(int numCycles) {
    cycles = numCycles;

    while (cycles > 0) {
        opcode = read(pc++);
        disassemble(opcode, pc - 1);
        (this->*lookup[opcode])();
    }

    return cycles;
}

// Reads a byte from memory at the specified address
uint8_t Intel8080::read(uint16_t addr) const {
    return memory->read(addr);
}

// Writes a byte to memory at the specified address
void Intel8080::write(uint16_t addr, uint8_t data) const {
    memory->write(addr, data);
}

// Reads from an input port
uint8_t Intel8080::inport(uint8_t port) const {
    return ioPorts->read(port);
}
// Writes to an output port
void Intel8080::outport(uint8_t port, uint8_t data) const {
    ioPorts->write(port, data);
}

// Triggers an interrupt with the given interrupt number
void Intel8080::interrupt(uint8_t n) {
    if (!intEnable) return;
    push(pc);
    pc = (uint16_t) n << 3;
    intEnable = 0;
}

// Loads a game or program from a file into memory
bool Intel8080::load(const std::string& filePath, uint16_t loadAddress) const {
    return memory->load(filePath, loadAddress);
}