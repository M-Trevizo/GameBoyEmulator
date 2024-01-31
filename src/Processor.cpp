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
                case 0x1: return LD_16BIT(BC);
                case 0x2: return LD_16BIT_A(BC);
                case 0x3: return INC_16BIT(BC);
                case 0x4: return INC_8BIT_H(BC);
                case 0x5: return DEC_8BIT_H(BC);
                case 0x6: return LD_8BIT_H(BC);
                case 0x7: return RLCA();
                case 0x8: return LD_16BIT_SP();
                case 0x9: return ADD_HL_R16(BC);
                case 0xA: return LD_A_16BIT(BC);
                case 0xB: return DEC_16BIT(BC);
                case 0xC: return INC_8BIT_L(BC);
                case 0xD: return DEC_8BIT_L(BC);
                case 0xE: return LD_8BIT_L(BC);
                case 0xF: return RRCA();
            }
        break;
        case 0x1:
            switch(nibble2) {
                case 0x0: return STOP();
                case 0x1: return LD_16BIT(DE);
                case 0x2: return LD_16BIT_A(DE);
                case 0x3: return INC_16BIT(DE);
                case 0x4: return INC_8BIT_H(DE);
                case 0x5: return DEC_8BIT_H(DE);
                case 0x6: return LD_8BIT_H(DE);
                case 0x7: return RLA();
                case 0x8: return JR();
                case 0x9: return ADD_HL_R16(DE);
                case 0xA: return LD_A_16BIT(DE);
                case 0xB: return DEC_16BIT(DE);
                case 0xC: return INC_8BIT_L(DE);
                case 0xD: return DEC_8BIT_L(DE);
                case 0xE: return LD_8BIT_L(DE);
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
}

// 0x10
int Processor::STOP() {
    // Not sure if this opCode should do anything
    return 1;
}

// 0x18
int Processor::JR() {

    int8_t byte = fetch();
    PC += byte;

    return 3;
}

// 16-bit Loads
int Processor::LD_16BIT(Register reg) {

    reg.high = fetch();
    reg.low = fetch();
    
    return 3;
}

int Processor::LD_16BIT_A(Register reg) {
    
    uint8_t A = AF.high;
    memory[BC.word] = A;

    return 2;
}

// 8-bit Loads
// High
int Processor::LD_8BIT_H(Register reg) {
    
    reg.high = fetch();

    return 2;
}

// Low
int Processor::LD_8BIT_L(Register reg) {

    reg.low = fetch();

    return 2;
}

int Processor::LD_A_16BIT(Register reg) {

    AF.high = memory[BC.word];

    return 2;
}

// 16-bit Increment
int Processor::INC_16BIT(Register reg) {

    reg.word++;

    return 2;
}

// 16-bit Decrement
int Processor::DEC_16BIT(Register reg) {
    
    reg.word--;

    return 2;
}

// Increment high byte of 16-bit register
int Processor::INC_8BIT_H(Register reg) {
    
    reg.high++;

    uint8_t lowNibble = reg.high & 0x0F;
    if(reg.high == 0) {
        AF.low &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF.low &= H_FLAG;
    }
    AF.low &= ~N_FLAG;

    return 1;
}

// Increment low byte of 16-bit register
int Processor::INC_8BIT_L(Register reg) {

    reg.low++;

    uint8_t lowNibble = reg.low & 0x0F;
    if(reg.low == 0) {
        AF.low &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF.low &= H_FLAG;
    }
    AF.low &= ~N_FLAG;

    return 1;
}

// Decrement high byte of 16-bit register
int Processor::DEC_8BIT_H(Register reg) {

    reg.high--;

    uint8_t lowNibble = reg.high & 0x0F;
    if(reg.high == 0) {
        AF.low &= Z_FLAG;
    }
    if(lowNibble == 0xF) {
        AF.low &= H_FLAG;
    }
    AF.low &= N_FLAG;
    
    return 1;
}

// Decrement low byte of 16-bit register
int Processor::DEC_8BIT_L(Register reg) {

    reg.low--;

    uint8_t lowNibble = reg.low & 0x0F;
    if(reg.low == 0) {
        AF.low &= Z_FLAG;
    }
    if(lowNibble == 0) {
        AF.low &= H_FLAG;
    }
    AF.low &= N_FLAG;

    return 1;
}

// Add Instructions
int Processor::ADD_HL_R16(Register reg) {

    if((int)HL.word + (int)reg.word> 0xFFFF) {
        AF.low &= C_FLAG;
    }
    if((int)HL.word + (int)DE.word > 0xFFF) {
        AF.low &= H_FLAG;
    }
    AF.low &= ~N_FLAG;

    HL.word += reg.word;

    return 2;
}

// Stack(SP) Operations

int Processor::LD_16BIT_SP() {

    uint8_t addressHigh = fetch();
    uint8_t addressLow = fetch();
    uint16_t address = (addressHigh << 8) | addressLow;
    
    memory[address] = SP.high;
    memory[address + 1] = SP.low;

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
        AF.low &= C_FLAG;
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
        AF.low &= C_FLAG;
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
        AF.low &= C_FLAG;
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
        AF.low &= C_FLAG;
    }
    else {
        AF.low &= ~C_FLAG;
    }

    return 1;
}