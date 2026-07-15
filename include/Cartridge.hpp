#pragma once

#include <cstdint>
#include <vector>
#include <iostream>

using namespace std;

class Cartridge {

    public:
    
        vector<uint8_t> cartROM;
        uint8_t cartType;
        
        Cartridge(string path);
};
