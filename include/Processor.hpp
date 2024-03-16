#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdint>
#include "./Cartridge.hpp"

const uint16_t MEM_SIZE = 65535;
const uint16_t VRAM_START = 0x8000;
const int M_CYCLES_PER_FRAME = 17476;                           // Based on a 60 FPS limit.
//int CYCLES_EXECUTED = 0;
const uint8_t Z_FLAG = 0x80;
const uint8_t N_FLAG = 0x40;
const uint8_t H_FLAG = 0x20;
const uint8_t C_FLAG = 0x10;

union Register {
    
    uint16_t word;
    
    struct {
        uint8_t low;
        uint8_t high;
    };

};

class Processor {

    public:

        uint8_t memory[MEM_SIZE];

        // General Registers
        Register AF;
        Register BC;
        Register DE;
        Register HL;

        // Hardware Registers
        uint8_t LCDC = 0;                                      // LCD Control

        // Program Counter
        int PC;

        // Stack Pointer
        // Not a register, using reg union to get high and low bytes
        Register SP;

        bool isRunning = false;

        Processor(Cartridge cartridge, uint8_t cartType = 0);
        void tickClock();
        void bootSequence(Cartridge cart);
        array<uint8_t, 2> get8BitRegisters(uint16_t r);
        uint16_t fetch();
        array<uint8_t, 2> decode(uint8_t opCode);
        int execute(array<uint8_t, 2> nibbles);
        
        // 0x00
        int NOP();
        // 0x10
        int STOP();
        // Control/Branch instructions
        int JR();
        int JRNZ();
        
        // Load instructions
        // 16-bit Loads
        int LD_16BIT(uint16_t &reg);
        int LD_16BIT_A(uint16_t &reg);
        // 8-bit Loads
        int LD_8BIT(uint8_t &reg);
        int LD_8BIT_H(uint8_t &reg);
        int LD_8BIT_L(uint8_t &reg);
        int LD_A_16BIT(uint16_t &reg);
        // Load A into HL then increment/decrement HL
        int LD_HL_INC();
        // Load HL into A then increment/decrement HL
        int LD_A_INC();
        
        // Arithmetic instructions
        // Add instructions
        int ADD_HL_R16(uint16_t &reg);

        // Inc/Dec instructions
        // 16-bit Increment
        int INC_16BIT(uint16_t &reg);
        // 16-bit Decrement
        int DEC_16BIT(uint16_t &reg);
        // 8-bit Increment
        int INC_8BIT(uint8_t &reg);
        // 8-bit Decrement
        int DEC_8BIT(uint8_t &reg);
        
        // Bit operations
        int RLCA();
        int RRCA();
        int RLA();
        int RRA();

        // Stack(SP) Operations

        int LD_16BIT_SP();
};

#endif