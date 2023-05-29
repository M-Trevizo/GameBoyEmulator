#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <cstdint>

const unsigned int MEM_SIZE = 65536;

class Processor {

    public:

        uint16_t memory[MEM_SIZE];
        uint16_t AF;
        uint16_t BC;
        uint16_t DE;
        uint16_t HL;
        int PC;
        int SP;

        void initProcessor();

};

#endif