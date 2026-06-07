#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

class Texture
{
public:
    static constexpr float kOriginalSize = -1.f;
    int textureWidth{0};
    int textureHeight{0};

    ~Texture();

    bool loadFromFile(std::string path, SDL_Renderer* renderer);
    bool loadFromRenderedText(std::string textureText,
                              SDL_Color textColor,
                              TTF_Font* font,
                              SDL_Renderer* renderer);

    void destroy();
    void render(float x,
                float y,
                SDL_Renderer* renderer,
                SDL_FRect* clip = nullptr,
                float width = kOriginalSize,
                float height = kOriginalSize,
                double degrees = 0.0,
                SDL_FPoint* center = nullptr,
                SDL_FlipMode flipMode = SDL_FLIP_NONE);

    void setColor(Uint8 r, Uint8 g, Uint8 b);
    void setAlpha(Uint8 alpha);
    void setBlending(SDL_BlendMode blendMode);

    bool isLoaded();

private:
    SDL_Texture* texture_{nullptr};
};
