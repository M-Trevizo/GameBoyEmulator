#include <iostream>
#include "../include/Platform.hpp"

using namespace std;

int main(int argc, char** argv) {

    Platform platform;

    bool quit = false;
    while(!quit) {

        quit = platform.processEvents();
        
    }

    return 1;

}