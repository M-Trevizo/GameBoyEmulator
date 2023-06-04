#include <iostream>
#include <chrono>
#include <cstdint>
#include "../include/Processor.hpp"
#include "../include/Cartridge.hpp"

using namespace std;

const uint16_t VRAM_START = 0x8000;

Processor::Processor(uint8_t cartType) {

}

void Processor::tickClock() {
    // 238 nanoseconds
}

void Processor::bootSequence(Cartridge cart) {

    uint16_t logoStart = 0x0104;
    uint16_t logoEnd = 0x0133;
    uint16_t vramIndex = VRAM_START;
    for(int i = logoStart; i <= logoEnd; i++) {
        memory[vramIndex] = cart.cartROM[i];
        vramIndex++; 
    }

}