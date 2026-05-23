#ifndef APP_H
#define APP_H

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "Grid.h"
#include "Mixer.h"

class App
{
public:
    App();
    ~App();

    SDL_Renderer* const getRenderer();

    bool isSuccessful();
    bool loadMedia();

    void handleEvents(SDL_Event event);
    void update();
    void render();

private:
    Grid* grid_{nullptr};
    Mixer* mixer_{nullptr};
    SDL_Window* window_{nullptr};
    SDL_Renderer* renderer_{nullptr};

    bool success_{false};
};

#endif
