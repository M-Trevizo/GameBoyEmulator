#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <SDL.h>

class Platform {

    public:

        static const int WIDTH = 800;
        static const int HEIGHT = 720;
        static const int FPS_LIMIT = 60;
        static const int TICKS_PER_FRAME = 1000 / FPS_LIMIT;
        SDL_Renderer *renderer;
        SDL_Window *window;
        SDL_Event event;

        Platform();
        ~Platform();
        bool processEvents();
    
};

#endif