#include <SDL.h>

class Platform {

    public:

    static const int WIDTH = 800;
    static const int HEIGHT = 720;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Event event;

    void initPlatform();

};