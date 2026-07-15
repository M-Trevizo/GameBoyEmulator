#pragma once

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

class Platform {
private:
    static constexpr SDL_InitFlags INIT_FLAGS = SDL_INIT_AUDIO | SDL_INIT_VIDEO;

public:

    static const int WIDTH = 800;
    static const int HEIGHT = 720;
    static const int FPS_LIMIT = 60;
    static const int TICKS_PER_FRAME = 1000 / FPS_LIMIT;
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;

    Platform();
    ~Platform();
    bool processEvents();
    
};
