#pragma once

#include <string>
#include <vector>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Grid.h"
#include "Texture.h"

class Font {
public:

    bool success{false};

    Font(SDL_Renderer* renderer, Grid::Data* data);
    ~Font();

    void update();
    void render();

private:
    struct Text {
        std::string text;
        int* type{nullptr};
        Texture texture;
    };

    TTF_Font* font_{nullptr};
    SDL_Renderer* renderer_;
    std::vector<Text> texts_;
};
