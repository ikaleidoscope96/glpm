#include <SDL3/SDL_render.h>
#include "Texture.h"

Texture::~Texture()
{
    destroy();
}

bool Texture::loadFromFile(std::string path, SDL_Renderer* renderer)
{
    destroy();

    if (SDL_Surface* loadedSurface = IMG_Load(path.c_str());
        loadedSurface == nullptr)
    {
        SDL_Log("Unable to load image %s! SDL_image error: %s\n", path.c_str(), SDL_GetError());
    } else {
        if (!SDL_SetSurfaceColorKey(loadedSurface,
                                   true,
                                   SDL_MapSurfaceRGB(loadedSurface, 0x00, 0xFF, 0xFF)))
        {
            SDL_Log( "Unable to color key! SDL error: %s", SDL_GetError() );
        } else {
            if (texture_ = SDL_CreateTextureFromSurface(renderer, loadedSurface);
                texture_ == nullptr)
            {
                SDL_Log("Unable to create texture from loaded pixels! SDL error: %s\n",
                        SDL_GetError());
            } else {
                textureWidth = loadedSurface->w;
                textureHeight = loadedSurface->h;
            }
        }

        SDL_DestroySurface(loadedSurface);
    }

    return texture_ != nullptr;
}

bool Texture::loadFromRenderedText(std::string textureText,
                                   SDL_Color textColor,
                                   TTF_Font* font,
                                   SDL_Renderer* renderer)
{
    destroy();

    if (SDL_Surface* textSurface = TTF_RenderText_Blended(font, textureText.c_str(), 0, textColor);
        textSurface == nullptr)
    {
        SDL_Log("Unable to render text surface! SDL_ttf Error: %s\n", SDL_GetError());
    }
    else
    {
        if (texture_ = SDL_CreateTextureFromSurface(renderer, textSurface);
            texture_ == nullptr)
        {
            SDL_Log("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        } else {
            textureWidth = textSurface->w;
            textureHeight = textSurface->h;
        }

        SDL_DestroySurface(textSurface);
    }

    return texture_ != nullptr;
}

void Texture::destroy()
{
    SDL_DestroyTexture(texture_);
    texture_ = nullptr;
    textureWidth = 0;
    textureHeight = 0;
}

void Texture::render(float x,
                     float y,
                     SDL_Renderer* renderer,
                     SDL_FRect* clip,
                     float width,
                     float height,
                     double degrees,
                     SDL_FPoint* center,
                     SDL_FlipMode flipMode)
{
    SDL_FRect dstRect{x, y,static_cast<float>(textureWidth), static_cast<float>(textureHeight)};

    if( clip != nullptr )
    {
        dstRect.w = clip->w;
        dstRect.h = clip->h;
    }

    if( width > 0 )
    {
        dstRect.w = width;
    }

    if( height > 0 )
    {
        dstRect.h = height;
    }

    SDL_RenderTextureRotated(renderer, texture_, clip, &dstRect, degrees, center, flipMode);
}

void Texture::setColor(Uint8 r, Uint8 g, Uint8 b)
{
    SDL_SetTextureColorMod(texture_, r, g, b);
}

void Texture::setAlpha(Uint8 alpha)
{
    SDL_SetTextureAlphaMod(texture_, alpha);
}

void Texture::setBlending(SDL_BlendMode blendMode)
{
    SDL_SetTextureBlendMode(texture_, blendMode);
}

bool Texture::isLoaded()
{
    return texture_ != nullptr;
}
