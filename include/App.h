#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "Mixer.h"

class App
{
public:
    App();
    ~App();

    // SDL_Renderer* getRenderer();

    bool isSuccessful();
    bool loadMedia();

    void handleEvents(SDL_Event event);
    void update();
    void render();

private:
    Mixer* mixer_{nullptr};
    SDL_Window* window_{nullptr};
    SDL_Renderer* renderer_{nullptr};

    bool success_{false};
};

#endif
