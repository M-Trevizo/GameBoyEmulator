#include "../include/Processor.hpp"
#include "../include/Cartridge.hpp"

#include <iostream>
#include <chrono>
#include <array>

using namespace std;

Processor::Processor(Cartridge cartridge, uint8_t cartType) {

    for(int i = 0; i < VRAM_START; i++) {
        memory[i] = cartridge.cartROM[i];
    }

    PC = 0x0100;

}

void Processor::tickClock() {
    // 238 nanoseconds
    // Perform single instruction:
        // Take start time
        // Fetch Instruction w/ t-time & m-time
        // Calculate time it should take to complete instruction
        // Decode and Perform instruction
        // Calculate time delta from instructions start to end
        // Sleep for time delta from instruction time taken to calculated completion time.
        
}

// Boot Rom, not sure this is required.
void Processor::bootSequence(Cartridge cart) {

    uint16_t logoStart = 0x0104;
    uint16_t logoEnd = 0x0133;
    uint16_t vramIndex = VRAM_START;
    for(int i = logoStart; i <= logoEnd; i++) {
        memory[vramIndex] = cart.cartROM[i];
        vramIndex++; 
    }
    
}

// Returns array<uint8_t, 2> containing high and low byte of a given 16-bit register.
array<uint8_t, 2> Processor::get8BitRegisters(uint16_t r) {

    uint16_t highMask = 0xFF00;
    uint16_t lowMask = 0x00FF;

    uint8_t highRegister = (r & highMask) >> 8;
    uint8_t lowRegister = r & lowMask;

    array registers = {highRegister, lowRegister};

    return registers;
}

uint16_t Processor::fetch() {
    
    uint8_t opCode = memory[PC];
    PC++;

    return opCode;
}

array<uint8_t, 2> Processor::decode(uint8_t opCode) {

    uint8_t highMask = 0xF0;
    uint8_t lowMask = 0x0F;

    uint8_t highNibble = (highMask & opCode) >> 4;
    uint8_t lowNibble = (lowMask & opCode);

    array<uint8_t, 2> nibbles = {highNibble, lowNibble};
    
    return nibbles;
}

int Processor::execute(array<uint8_t, 2> nibbles) {
    
    uint8_t nibble1 = nibbles[0];
    uint8_t nibble2 = nibbles[1];

    switch(nibble1) {
        case 0x0: 
            switch(nibble2) {
                case 0x0: return NOP();
                case 0x1: return LD_16BIT(BC.word);
                case 0x2: return LD_16BIT_A(BC.word);
                case 0x3: return INC_16BIT(BC.word);
                case 0x4: return INC_8BIT(BC.high);
                case 0x5: return DEC_8BIT(BC.high);
                case 0x6: return LD_8BIT(BC.high);
                case 0x7: return RLCA();
                case 0x8: return LD_16BIT_SP();
                case 0x9: return ADD_HL_R16(BC.word);
                case 0xA: return LD_A_16BIT(BC.word);
                case 0xB: return DEC_16BIT(BC.word);
                case 0xC: return INC_8BIT(BC.low);
                case 0xD: return DEC_8BIT(BC.low);
                case 0xE: return LD_8BIT(BC.low);
                case 0xF: return RRCA();
            }
        break;
        case 0x1:
            switch(nibble2) {
                case 0x0: return STOP();
                case 0x1: return LD_16BIT(DE.word);
                case 0x2: return LD_16BIT_A(DE.word);
                case 0x3: return INC_16BIT(DE.word);
                case 0x4: return INC_8BIT(DE.high);
                case 0x5: return DEC_8BIT(DE.high);
                case 0x6: return LD_8BIT(DE.high);
                case 0x7: return RLA();
                case 0x8: return JR();
                case 0x9: return ADD_HL_R16(DE.word);
                case 0xA: return LD_A_16BIT(DE.word);
                case 0xB: return DEC_16BIT(DE.word);
                case 0xC: return INC_8BIT(DE.low);
                case 0xD: return DEC_8BIT(DE.low);
                case 0xE: return LD_8BIT(DE.low);
                case 0xF: return RRA();
            }
        break;
        case 0x2:
            switch(nibble2) {
                case 0x0: return JRNZ();
                case 0x1: return LD_16BIT(HL.word);
                case 0x2: return LD_HL_INC();
                case 0x3: return INC_16BIT(HL.word);
                case 0x4: return INC_8BIT(HL.high);
                case 0x5: return DEC_8BIT(HL.high);
                case 0x6: return LD_8BIT(HL.high);
                case 0x7: return DAA();
                case 0x8: return JRZ();
                case 0x9: return ADD_HL_R16(HL.word);
                case 0xA: return LD_A_INC();
                case 0xB: return DEC_16BIT(HL.word);
                case 0xC: return INC_8BIT(HL.low);
                case 0xD: return DEC_8BIT(HL.low);
                case 0xE: return LD_8BIT(HL.low);
                case 0xF: return CPL();
            }
        break;
        case 0x3:
            switch(nibble2) {
                case 0x0: return JRNC();
                case 0x1: return LD_16BIT(SP.word);
                case 0x2: return LD_HL_DEC();
                case 0x3: return INC_16BIT(SP.word);
                case 0x4: return INC_8BIT(memory[HL.word], true);
                case 0x5: return DEC_8BIT(memory[HL.word], true);
                case 0x6: return LD_8BIT(memory[HL.word], true);
                case 0x7: return SCF();
                case 0x8: return JRC();
            }
        default: cout << "Instruction not recognized." << endl;
    }

    return -1;
}

// 0x00
int Processor::NOP() {
    return 1;
}

// 0x10
int Processor::STOP() {
    // Not sure if this opCode should do anything
    return 1;
}

// Branch/Control instructions
// Jump Relative
int Processor::JR() {

    // Get next byte without incrementing
    // Have already incremented PC in previous fetch call that read jump opcode.
    int8_t byte = memory[PC];
    PC += byte - 1;

    return 3;
}

// Jump Relative if Zero flag is not set
int Processor::JRNZ() {
    
    // Jump if Z flag is NOT set
    if((AF.low & Z_FLAG) != Z_FLAG) {
        int8_t byte = memory[PC];
        PC += byte - 1;
        return 3;
    }

    // Increment PC if no jump
    PC++;

    return 2;
}

int Processor::JRZ() {

    // Jump if Z flag is set
    if((AF.low & Z_FLAG) == Z_FLAG) {
        int8_t byte = memory[PC];
        PC += byte - 1;
        return 3;
    }

    // Increment PC if no jump
    PC++;

    return 2;
}

int Processor::JRNC() {

    // Jump if C flag is NOT set
    if ((AF.low & C_FLAG) != C_FLAG) {
        const int8_t byte = memory[PC];
        PC += byte - 1;
        return 3;
    }

    // Increment PC if no jump
    PC++;
    return 2;
}

int Processor::JRC() {

    // Jump if C flag is set
    if ((AF.low & C_FLAG) == C_FLAG) {
        const auto byte = static_cast<int8_t>(memory[PC]);
        PC += byte - 1;
        return 3;
    }

    PC++;
    return 2;
}

// 16-bit Loads
int Processor::LD_16BIT(uint16_t &reg) {

    uint8_t high = fetch();
    uint8_t low = fetch();
    
    reg = (high << 8) | low;
    return 3;
}

int Processor::LD_16BIT_A(uint16_t &reg) {
    
    uint8_t A = AF.high;
    memory[reg] = A;

    return 2;
}

// 8-bit Loads
int Processor::LD_8BIT(uint8_t &reg, bool is_pointer) {

    reg = fetch();

    if (is_pointer) {
        return 3;
    }

    return 2;
}

int Processor::LD_A_16BIT(uint16_t &reg) {

    AF.high = memory[reg];

    return 2;
}

int Processor::LD_HL_INC() {

    memory[HL.word] = AF.high;
    HL.word++;

    return 2;
}

int Processor::LD_HL_DEC() {

    memory[HL.word] = AF.high;
    HL.word--;

    return 2;
}

int Processor::LD_A_INC() {

    AF.high = memory[HL.word];
    HL.word++;

    return 2;
}

// 16-bit Increment
int Processor::INC_16BIT(uint16_t &reg) {

    reg++;

    return 2;
}

// 16-bit Decrement
int Processor::DEC_16BIT(uint16_t &reg) {
    
    reg--;

    return 2;
}

// Increment 8-bit register
int Processor::INC_8BIT(uint8_t &reg, bool is_pointer) {
    reg++;

    uint8_t lowNibble = reg & 0x0F;
    if(reg == 0) {
        AF.low |= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF.low |= H_FLAG;
    }
    AF.low &= ~N_FLAG;

    if (is_pointer) {
        return 3;
    }

    return 1;
}

// Decrement 8-bit register
int Processor::DEC_8BIT(uint8_t &reg, bool is_pointer) {

    reg--;

    uint8_t lowNibble = reg & 0x0F;
    if(reg == 0) {
        AF.low |= Z_FLAG;
    }
    if(lowNibble == 0xF) {
        AF.low |= H_FLAG;
    }
    AF.low |= N_FLAG;

    if (is_pointer) {
        return 3;
    }

    return 1;
}

// Add Instructions
int Processor::ADD_HL_R16(uint16_t &reg) {

    if(static_cast<int>(HL.word) + static_cast<int>(reg) > 0xFFFF) {
        AF.low |= C_FLAG;
    }
    if(static_cast<int>(HL.word) + static_cast<int>(reg) > 0xFFF) {
        AF.low |= H_FLAG;
    }
    AF.low &= ~N_FLAG;

    HL.word += reg;

    return 2;
}

// Stack(SP) Operations

int Processor::LD_16BIT_SP() {

    uint8_t addressHigh = fetch();
    uint8_t addressLow = fetch();
    uint16_t address = (addressHigh << 8) | addressLow;
    
    memory[address] = SP.low;
    memory[address + 1] = SP.high;

    return 5;
}

// Bitwise Operations

int Processor::RLCA() {

    uint8_t bit = AF.high >> 7;
    AF.high = (AF.high << 1) | bit;
    
    AF.low &= ~Z_FLAG;
    AF.low &= ~N_FLAG;
    AF.low &= ~H_FLAG;

    if(bit) {
        AF.low |= C_FLAG;
    }
    else {
        AF.low &= ~C_FLAG;
    }

    return 1;
}

int Processor::RRCA() {

    uint8_t bit = AF.high & 0x01;
    AF.high = (AF.high >> 1) | (AF.high << 7);

    AF.low &= ~Z_FLAG;
    AF.low &= ~N_FLAG;
    AF.low &= ~H_FLAG;

    if(bit) {
        AF.low |= C_FLAG;
    }
    else {
        AF.low &= ~C_FLAG;
    }

    return 1;
}

int Processor::RLA() {
    
    uint8_t bit = AF.high >> 7;

    AF.high = (AF.high << 1) | ((AF.low & C_FLAG) >> 4);

    // Unset ZNH flags
    AF.low &= ~Z_FLAG;
    AF.low &= ~N_FLAG;
    AF.low &= ~H_FLAG;

    // Set C flag appropriatly
    if(bit) {
        AF.low |= C_FLAG;
    }
    else {
        AF.low &= ~C_FLAG;
    }

    return 1;
}

int Processor::RRA() {

    uint8_t bit = AF.high & 0x01;

    AF.high = AF.high >> 1;
    AF.high |= ((AF.low & C_FLAG) << 3);

    AF.low &= ~Z_FLAG;
    AF.low &= ~N_FLAG;
    AF.low &= ~H_FLAG;

    if(bit) {
        AF.low |= C_FLAG;
    }
    else {
        AF.low &= ~C_FLAG;
    }

    return 1;
}

// Decimal Adjust Accumulator
int Processor::DAA() {
    
    uint8_t lowNibble = AF.high & 0xF;
    uint8_t highNibble = (AF.high >> 4) & 0xF;

    if(highNibble > 0x9 && lowNibble > 0x9) {
        AF.high += 0x66;
    }
    else if(highNibble > 0x9) {
        AF.high += 0x60;
    }
    else if(lowNibble > 0x9) {
        AF.high += 0x06;
    }

    // Set or Unset Z-flag
    if(AF.high == 0) {
        AF.low |= Z_FLAG;
    }
    else {
        AF.low &= ~Z_FLAG;
    }

    // Unset H-flag
    AF.low &= ~H_FLAG;

    // Set or Unset C_flag
    if(highNibble > 0x9) {
        AF.low |= C_FLAG;
    }
    else {
        AF.low &= ~C_FLAG;
    }

    return 1;
}

// Complement accumulator
int Processor::CPL() {
    
    ~AF.high;

    // Set N-flag
    AF.low |= N_FLAG;

    // Set H-flag
    AF.low |= H_FLAG; 

    return 1;
}

// Set carry flag
int Processor::SCF() {

    AF.low |= C_FLAG;
    AF.low &= ~N_FLAG;
    AF.low &= ~H_FLAG;

    return 1;
}
