#include <iostream>
#include <fstream>
#include "../include/Cartridge.hpp"

Cartridge::Cartridge(string path) {

    ifstream stream;
    stream.open(path, ios::binary);

    if(!stream.is_open()) {

        cout << "Unable to open file." << endl;
        return;

    }

    filebuf *pBuf = stream.rdbuf();
    while(pBuf->sgetc() != EOF) {

        int i = 0;
        uint8_t byte = pBuf->sbumpc();
        cartROM[i] = byte;
        i++;

    }

    cartType = cartROM[0x0147];
    
}