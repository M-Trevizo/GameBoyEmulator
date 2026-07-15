#include "../include/Platform.hpp"

#include <iostream>

#include <SDL3/SDL_log.h>

using namespace std;

Platform::Platform() {

    SDL_Init(INIT_FLAGS);
    if (!SDL_CreateWindowAndRenderer("GameBoy Emulator", WIDTH, HEIGHT, 0, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindowAndRenderer() failed: %s\n", SDL_GetError());
        return;
    }

    if (!SDL_SetRenderLogicalPresentation(renderer, 160, 144, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE)) {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER, "SDL_SetRenderLogicalPresentation() failed, unable set logical presentation: %s\n", SDL_GetError());
        return;
    }

}

Platform::~Platform() {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

}

bool Platform::processEvents() {
    
    bool quit = false;
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_EVENT_QUIT) {
            quit = true;
        }
    }

    return quit;
}