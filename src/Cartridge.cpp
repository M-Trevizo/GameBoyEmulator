#include "../include/Cartridge.hpp"

#include <iostream>
#include <fstream>
#include <vector>


Cartridge::Cartridge(string path) {

    ifstream stream;
    stream.open(path, ios::binary);
    if(!stream.is_open()) {

        cout << "Unable to open file." << endl;
        return;

    }

    filebuf *pBuf = stream.rdbuf();
    while(pBuf->sgetc() != EOF) {

        uint8_t byte = pBuf->sbumpc();
        cartROM.push_back(byte);

    }

    cartType = cartROM[0x0147];
    
}