#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdint>
#include "./Cartridge.hpp"

const uint16_t MEM_SIZE = 65535;
const uint16_t VRAM_START = 0x8000;

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

};

#endif