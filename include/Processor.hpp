#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdint>

const unsigned int MEM_SIZE = 65536;

class Processor {

    public:

        uint16_t memory[MEM_SIZE];
        uint16_t AF = 0;
        uint16_t BC = 0;
        uint16_t DE = 0;
        uint16_t HL = 0;
        int PC;
        int SP;
        bool isRunning = false;

        Processor();
        void tickClock();

};

#endif