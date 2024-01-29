#include <iostream>
#include <chrono>
#include <cstdint>
#include <array>
#include "../include/Processor.hpp"
#include "../include/Cartridge.hpp"

using namespace std;

Processor::Processor(Cartridge cartridge, uint8_t cartType = 0) {

    for(int i = 0; i < VRAM_START; i++) {
        cartridge.cartROM[i] = memory[i];
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

    uint8_t highMask = 0xFF00;
    uint8_t lowMask = 0x00FF;

    uint8_t highRegister = (r & highMask) >> 8;
    uint8_t lowRegister = r & lowMask;

    array<uint8_t, 2> registers = {highRegister, lowRegister};

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
                break;
                case 0x1: return LD_BC();
                break;
                case 0x2: return LD_BC_A();
                break;
                case 0x3: return INC_BC();
                break;
                case 0x4: return INC_B();
                break;
                case 0x5: return DEC_B();
                break;
                case 0x6: return LD_B();
                break;
                case 0x7: return RLCA();
                break;
                case 0x8: return LD_16BIT_SP();
                break;
                case 0x9: return ADD_HL_BC();
                break;
                case 0xA: return LD_A_BC();
                break;
                case 0xB: return DEC_BC();
                break;
                case 0xC: return INC_C();
                break;
                case 0xD: return DEC_C();
                break;
                case 0xE: return LD_C();
                break;
                case 0xF: return RRCA();
                break;
            }
        break;
        case 0x1:
            switch(nibble2) {
                case 0x0: return STOP();
                case 0x1: return LD_DE();
                case 0x2: return LD_DE_A();
                case 0x3: return INC_DE();
                case 0x4: return INC_D();
                case 0x5: return DEC_D();
                case 0x6: return LD_D();
                case 0x7: return RLA();
                case 0x8: return JR();
                case 0x9: return ADD_HL_DE();
                case 0xA: return LD_A_DE();
                case 0xB: return DEC_DE();
                case 0xC: return INC_E();
                case 0xD: return DEC_E();
                case 0xE: return LD_E();
                case 0xF: return RRA();
            }
        break;
        case 0x2:
            switch(nibble2) {
                case 0x0: ;
                break;
            }
        break;
        default: cout << "Instruction not recognized." << endl;
    }
}

// 0x00
int Processor::NOP() {
    return 1;
};

// 0x01
int Processor::LD_BC() {
    
    uint8_t highByte = fetch();
    uint8_t lowByte = fetch();
    
    uint16_t word = (highByte << 8) | lowByte;
    
    BC = word;
    
    return 3;
}

// 0x02
int Processor::LD_BC_A() {
    
    uint8_t A = get8BitRegisters(AF)[0];
    memory[BC] = A;

    return 2;
}

// 0x03
int Processor::INC_BC() {
    
    BC++;

    return 2;
}

// 0x04
int Processor::INC_B() {

    uint8_t B = get8BitRegisters(BC)[0];
    B++;

    uint8_t lowNibble = B & 0x0F;
    if(B == 0) {
        AF &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF &= H_FLAG;
    }
    AF &= ~N_FLAG;
    
    BC = (B << 8) | BC;

    return 1;
}

// 0x05
int Processor::DEC_B() {
    
    uint8_t B = get8BitRegisters(BC)[0];
    B--;

    uint8_t lowNibble = B & 0x0F;
    if(B == 0) {
        AF &= Z_FLAG;
    }
    if(lowNibble == 0xF) {
        AF &= H_FLAG;
    }
    AF &= N_FLAG;

    BC = (B << 8) | BC;
    
    return 1;
}

// 0x06
int Processor::LD_B() {
    
    uint8_t byte = fetch();
    uint8_t C = get8BitRegisters(BC)[1];

    BC = (byte << 8) | C;

    return 2;
}

// 0x07
int Processor::RLCA() {

    uint8_t A = get8BitRegisters(AF)[0];
    uint8_t bit = A >> 7;

    A = (A << 1) | bit;
    AF = (A << 8) | AF;

    // Unset ZNH flags
    AF &= ~Z_FLAG;
    AF &= ~N_FLAG;
    AF &= ~H_FLAG;

    // Set C flag appropriatly
    if(bit) {
        AF &= C_FLAG;
    }

    return 1;
}

// 0x08
int Processor::LD_16BIT_SP() {

    uint8_t addressHigh = fetch();
    uint8_t addressLow = fetch();
    uint16_t address = (addressHigh << 8) | addressLow;

    array<uint8_t, 2> stackPointer = get8BitRegisters(SP);
    uint8_t highByte = stackPointer[0];
    uint8_t lowByte = stackPointer[1];
    
    memory[address] = highByte;
    memory[address + 1] = lowByte;

    return 5;
}

// 0x09
int Processor::ADD_HL_BC() {
    
    if((int)HL + (int)BC > 0xFFFF) {
        AF &= C_FLAG;
    }
    if((int)HL + (int)BC > 0xFFF) {
        AF &= H_FLAG;
    }
    AF &= ~N_FLAG;

    HL += BC;

    return 2;
}

// 0x0A
int Processor::LD_A_BC() {

    uint8_t A = get8BitRegisters(AF)[0];
    A = memory[BC];
    
    AF = (A << 8) | AF;

    return 2;
}

// 0x0B
int Processor::DEC_BC() {
    
    BC--;

    return 2;
}

// 0x0C
int Processor::INC_C() {

    array<uint8_t, 2> registers = get8BitRegisters(BC);
    uint8_t B = registers[0];
    uint8_t C = registers[1];
    C++;

    uint8_t lowNibble = C & 0x0F;
    if(C == 0) {
        AF &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF &= H_FLAG;
    }
    AF &= ~N_FLAG;
    
    BC = (B << 8) | C;

    return 1;
}

// 0x0D
int Processor::DEC_C() {

    array<uint8_t, 2> registers = get8BitRegisters(BC);
    uint8_t B = registers[0];
    uint8_t C = registers[1];
    C--;

    uint8_t lowNibble = C & 0x0F;
    if(C == 0) {
        AF &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF &= H_FLAG;
    }
    AF &= N_FLAG;
    
    BC = (B << 8) | C;

    return 1;
}

// 0x0E
int Processor::LD_C() {

    uint8_t B = get8BitRegisters(BC)[0];
    uint8_t C = fetch();

    BC = (B << 8) | C;

    return 2;
}

// 0x0F
int Processor::RRCA() {

    uint8_t A = get8BitRegisters(AF)[0];
    uint8_t bit = A << 7;
    
    A = A >> 1;
    A |= bit;
    AF = (A << 8) | AF;

    if(bit == 0) {
        AF &= ~C_FLAG;
    }
    else {
        AF &= C_FLAG;
    }

    return 1;
}

// 0x10
int Processor::STOP() {
    // Not sure if this opCode should do anything
    return 1;
}

// 0x11
int Processor::LD_DE() {
    
    uint8_t highByte = fetch();
    uint8_t lowByte = fetch();
    
    uint16_t word = (highByte << 8) | lowByte;
    
    DE = word;

    return 3;
}

// 0x12
int Processor::LD_DE_A() {

    uint8_t A = get8BitRegisters(AF)[0];
    memory[DE] = A;

    return 2;
}

// 0x13
int Processor::INC_DE() {

    DE++;

    return 2;
}

// 0x14
int Processor::INC_D() {

    uint8_t D = get8BitRegisters(DE)[0];
    D++;

    uint8_t lowNibble = D & 0x0F;
    if(D == 0) {
        AF &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF &= H_FLAG;
    }
    AF &= ~N_FLAG;
    
    DE = (D << 8) | DE;

    return 1;
}

// 0x15
int Processor::DEC_D() {

    uint8_t D = get8BitRegisters(DE)[0];
    D--;

    uint8_t lowNibble = D & 0x0F;
    if(D == 0) {
        AF &= Z_FLAG;
    }
    if(lowNibble == 0xF) {
        AF &= H_FLAG;
    }
    AF &= N_FLAG;

    DE = (D << 8) | DE;
    
    return 1;
}

// 0x16
int Processor::LD_D() {

    uint8_t byte = fetch();
    uint8_t E = get8BitRegisters(DE)[1];

    DE = (byte << 8) | E;

    return 2;
}

// 0x17
int Processor::RLA() {
    
    uint8_t A = get8BitRegisters(AF)[0];
    uint8_t bit = A >> 7;

    A = (A << 1) | (C_FLAG >> 4);
    AF = (A << 8) | AF;

    // Unset ZNH flags
    AF &= ~Z_FLAG;
    AF &= ~N_FLAG;
    AF &= ~H_FLAG;

    // Set C flag appropriatly
    if(bit) {
        AF &= C_FLAG;
    }
    else {
        AF &= ~C_FLAG;
    }

    return 1;
}

// 0x18
int Processor::JR() {

    int8_t byte = fetch();
    PC += byte;

    return 3;
}

// 0x19
int Processor::ADD_HL_DE() {

    if((int)HL + (int)DE > 0xFFFF) {
        AF &= C_FLAG;
    }
    if((int)HL + (int)DE > 0xFFF) {
        AF &= H_FLAG;
    }
    AF &= ~N_FLAG;

    HL += DE;

    return 2;
}

// 0x1A
int Processor::LD_A_DE() {

    uint8_t A = get8BitRegisters(AF)[0];
    A = memory[DE];
    
    AF = (A << 8) | AF;

    return 2;
}

// 0x1B
int Processor::DEC_DE() {

    DE--;

    return 2;
}

// 0x1C
int Processor::INC_E() {
    
    array<uint8_t, 2> registers = get8BitRegisters(DE);
    uint8_t D = registers[0];
    uint8_t E = registers[1];
    E++;

    uint8_t lowNibble = E & 0x0F;
    if(E == 0) {
        AF &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF &= H_FLAG;
    }
    AF &= ~N_FLAG;
    
    DE = (D << 8) | E;

    return 1;
}

// 0x1D
int Processor::DEC_E() {

    array<uint8_t, 2> registers = get8BitRegisters(DE);
    uint8_t D = registers[0];
    uint8_t E = registers[1];
    E--;

    uint8_t lowNibble = E & 0x0F;
    if(E == 0) {
        AF &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF &= H_FLAG;
    }
    AF &= N_FLAG;
    
    DE = (D << 8) | E;

    return 1;
}

// 0x1E
int Processor::LD_E() {

    uint8_t D = get8BitRegisters(DE)[0];
    uint8_t E = fetch();

    DE = (D << 8) | E;

    return 2;
}

// 0x1F
int Processor::RRA() {

    uint8_t A = get8BitRegisters(AF)[0];
    uint8_t bit = A & 0x01;

    A = A >> 1;
    A |= (C_FLAG << 3);
    AF = (A << 8) | AF;

    if(bit) {
        AF &= C_FLAG;
    }
    else {
        AF &= ~C_FLAG;
    }

    return 1;
}

// Increment high byte of 16-bit register
int Processor::INC_R8_H(uint16_t reg) {
    
    uint8_t eightBitRegister = get8BitRegisters(reg)[0];
    eightBitRegister++;

    uint8_t lowNibble = eightBitRegister & 0x0F;
    if(eightBitRegister == 0) {
        AF &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF &= H_FLAG;
    }
    AF &= ~N_FLAG;
    
    reg = (eightBitRegister << 8) | reg;

    return 1;
}

// Increment low byte of 16-bit register
int Processor::INC_R8_L(uint16_t reg) {

    array<uint8_t, 2> registers = get8BitRegisters(reg);
    uint8_t highByte = registers[0];
    uint8_t lowByte = registers[1];
    lowByte++;

    uint8_t lowNibble = lowByte & 0x0F;
    if(lowByte == 0) {
        AF &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF &= H_FLAG;
    }
    AF &= ~N_FLAG;
    
    reg = (highByte << 8) | lowByte;

    return 1;
}