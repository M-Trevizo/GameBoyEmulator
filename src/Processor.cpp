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

array<uint8_t, 2> Processor::get8BitRegisters(uint16_t r) {

    uint8_t highMask = 0xFF00;
    uint8_t lowMask = 0x00FF;

    uint8_t highRegister = (r & highMask) >> 8;
    uint8_t lowRegister = r & lowMask;

    array<uint8_t, 2> registers = {highRegister, lowRegister};

    return registers;
}

uint16_t Processor::fetch(int PC) {
    
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

void Processor::execute(array<uint8_t, 2> nibbles) {
    
    uint8_t nibble1 = nibbles[0];
    uint8_t nibble2 = nibbles[1];

    switch(nibble1) {
        case 0x0: 
            switch(nibble2) {
                case 0x0: ;
                break;
            }
        break;
        case 0x1:
            switch(nibble2) {
                case 0x0: ;
                break;
            }
    }
}

// 0x00
int Processor::NOP() {
    return 1;
};

// 0x01
int Processor::LD_BC() {
    
    uint8_t highByte = fetch(PC);
    uint8_t lowByte = fetch(PC);
    
    uint16_t word = (highByte << 8) | lowByte;
    
    BC = word;
    
    return 3;
}

// 0x02
int Processor::LD_BC_A() {
    
    BC = (A << 8) | BC;

    return 2;
}

// 0x03
int Processor::INC_BC() {
    
    BC++;

    return 2;
}

// 0x04
int Processor::INC_B() {

    uint16_t mask = 0xFF00;
    uint8_t B = BC & mask;

    B++;

    F = F & 0xBF;
    
    if(B) {

    }

    BC = (B << 8) | BC;

    return 1;
}

// 0x05
int Processor::DEC_B() {
    
    uint16_t mask = 0xFF00;
    uint8_t B = BC & mask;

    B--;

    BC = (B << 8) | BC;

    return 1;
}

// 0x06
int Processor::LD_B() {
    
    uint8_t byte = fetch(PC);
    uint16_t mask = 0xFF00;
    uint8_t B = BC & mask;

    B = byte;

    return 2;
}

// 0x07
int Processor::RLCA() {

}