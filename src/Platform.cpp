// This will be for SDL2
#include <iostream>
#include "../include/Platform.hpp"

using namespace std;

void Platform::initPlatform() {

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("GBM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    if(NULL == window) {
        printf("Could not create window: %s \n", SDL_GetError());
        return;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, 160, 144);
    
}