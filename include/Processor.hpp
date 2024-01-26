#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdint>
#include "./Cartridge.hpp"

const uint16_t MEM_SIZE = 65535;
const uint16_t VRAM_START = 0x8000;
const int M_CYCLES_PER_FRAME = 17476;                           // Based on a 60 FPS limit.
int CYCLES_EXECUTED = 0;
const uint8_t Z_FLAG = 0x0080;
const uint8_t N_FLAG = 0x0040;
const uint8_t H_FLAG = 0x0020;
const uint8_t C_FLAG = 0x0010;

class Processor {

    public:

        uint8_t memory[MEM_SIZE];

        // General Registers
        uint16_t AF = 0;
        uint16_t BC = 0;
        uint16_t DE = 0;
        uint16_t HL = 0;

        // Hardware Registers
        uint8_t LCDC = 0;                                      // LCD Control

        // Program Counter
        int PC;

        // Stack Pointer
        int SP;

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
        // 0x01
        int LD_BC();
        // 0x02
        int LD_BC_A();
        // 0x03
        int INC_BC();
        //0x04
        int INC_B();
        // 0x05
        int DEC_B();
        // 0x06
        int LD_B();
        // 0x07
        int RLCA();
        // 0x08
        int LD_16BIT_SP();
        // 0x09
        int ADD_HL_BC();
        // 0x0A
        int LD_A_BC();
        // 0x0B
        int DEC_BC();
        // 0x0C
        int INC_C();
        // 0x0D
        int DEC_C();
        // 0x0E
        int LD_C();
        // 0x0F
        int RRCA();

};

#endif