#include <iostream>
#include "../include/Platform.hpp"
#include "../include/Cartridge.hpp"

using namespace std;

int main(int argc, char** argv) {

    string path = argv[1];

    Cartridge cartridge(path);
    Platform platform;
    
    bool quit = false;
    while(!quit) {

        quit = platform.processEvents();
        
    }

    return 1;

}