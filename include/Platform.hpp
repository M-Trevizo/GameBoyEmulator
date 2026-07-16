#pragma once

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

class Platform {
private:
    static constexpr SDL_InitFlags INIT_FLAGS = SDL_INIT_AUDIO | SDL_INIT_VIDEO;
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 720;
    static constexpr int FPS_LIMIT = 60;
    static constexpr int TICKS_PER_FRAME = 1000 / FPS_LIMIT;
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;

public:
    Platform();
    ~Platform();
    bool processEvents();
};
