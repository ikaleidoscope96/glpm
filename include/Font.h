#pragma once

#include <string>
#include <vector>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "Texture.h"

class Font {
public:
    struct Text {
        std::string text;
        int* type{nullptr};
        Texture texture;
    };

    bool success{false};
    std::vector<Text> texts;

    Font();
    ~Font();

    void render(SDL_Renderer* renderer);

private:
    TTF_Font* font_{nullptr};
};
