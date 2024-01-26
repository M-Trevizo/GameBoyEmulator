#include <iostream>
#include <array>
#include "../include/Platform.hpp"
#include "../include/Cartridge.hpp"
#include "../include/Processor.hpp"

using namespace std;

int main(int argc, char** argv) {

    if(argc != 2) {
        cout << "Please provide file path argument." << endl;
        return 0;
    }
    
    string path = argv[1];

    Cartridge cartridge(path);
    Platform platform;
    Processor processor(cartridge);
    
    bool quit = false;
    while(!quit) {
        
        quit = platform.processEvents();
        
        // Emulation Loop
        int cycleCount = 0;
        while(cycleCount <= M_CYCLES_PER_FRAME) {
            uint8_t opCode = processor.fetch();
            array<uint8_t, 2> nibbles = processor.decode(opCode);
            cycleCount += processor.execute(nibbles);
            // TODO: make sure to execute correct # of cycles
        }
        

    }

    return 1;

}