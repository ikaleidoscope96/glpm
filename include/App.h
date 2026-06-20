#pragma once

#include <SDL3_mixer/SDL_mixer.h>
#include "Grid.h"
#include "Mixer.h"

class App
{
public:
    bool success{false};

    App();
    ~App();

    bool loadMedia();

    void handleEvent(const SDL_Event& event);
    void update();
    void render();

private:
    Grid grid_;
    Mixer mixer_;

    SDL_Window* window_{nullptr};
    SDL_Renderer* renderer_{nullptr};
};
