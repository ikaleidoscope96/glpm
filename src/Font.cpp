#include "Font.h"
#include <SDL3/SDL_timer.h>
#include <cstdint>
#include <sstream>

Font::Font()
{
    if(!TTF_Init()) {
        SDL_Log( "SDL_ttf could not initialize! SDL error: %s\n", SDL_GetError() );
        return;
    }

    std::string path{"../assets/fonts/VT323-Regular.ttf"};
    if (font_ = TTF_OpenFont(path.c_str(), 32); font_ == nullptr) {
        SDL_Log("Could not load %s! SDL_ttf Error: %s\n", path.c_str(), SDL_GetError());
        return;
    }

    success = true;
}

Font::~Font()
{
    TTF_CloseFont(font_);
}

void Font::render(SDL_Renderer* renderer)
{
    SDL_Color textColor{ 0xFF, 0xFF, 0xFF, 0xFF };
    std::string path{"../assets/fonts/VT323-Regular.ttf"};

    for (auto& text: texts) {
        text.texture.destroy();

        if (!text.update) {
            if(!text.texture.loadFromRenderedText(text.text,
                                                  textColor,
                                                  font_,
                                                  renderer))
            {
                SDL_Log("Could not load text texture %s! SDL_ttf Error: %s\n", path.c_str(), SDL_GetError());
                success = false;
            }
        } else {
            std::stringstream tmp;
            if (auto ptr{std::get_if<int*>(&text.info)}) {
                tmp << text.text << **(ptr);
            } else if (auto ptr{std::get_if<uint64_t*>(&text.info)}) {
                uint64_t seconds = **(ptr) / 1000;
                uint64_t minutes = seconds / 60;
                uint64_t hours = minutes / 60;

                tmp << text.text;
                if (hours > 0) {
                    tmp << hours << ':';
                }
                if (minutes > 0) {
                    tmp << minutes << ':';
                }
                if ( minutes > 0 && seconds % 60 < 10) {
                    tmp << "0";
                }
                tmp << seconds % 60;
            }

            if(!text.texture.loadFromRenderedText(tmp.str(),
                                                  textColor,
                                                  font_,
                                                  renderer))
            {
                SDL_Log("Could not load text texture %s! SDL_ttf Error: %s\n", path.c_str(), SDL_GetError());
                success = false;
            }
        }
    }

    for (float y{0}; auto& text : texts) {
        text.texture.render(0, y, renderer);
        y += 32;
    }
}
