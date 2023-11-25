#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdint>
#include "./Cartridge.hpp"

const uint16_t MEM_SIZE = 65535;
const uint16_t VRAM_START = 0x8000;
const int M_CYCLES_PER_FRAME = 17476;                           // Based on a 60 FPS limit.

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

        int PC;
        int SP;
        bool isRunning = false;

        Processor(Cartridge cartridge, uint8_t cartType = 0);
        void tickClock();
        void bootSequence(Cartridge cart);
        uint16_t fetch(int PC);
        array<uint8_t, 2> decode(uint8_t opCode);
        void execute(array<uint8_t, 2> nibbles);
        
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
        int ADD_BC_LC();
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