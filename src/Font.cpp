#include "Font.h"

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

        if (text.type == nullptr) {
            if(!text.texture.loadFromRenderedText(text.text,
                                                  textColor,
                                                  font_,
                                                  renderer))
            {
                SDL_Log("Could not load text texture %s! SDL_ttf Error: %s\n", path.c_str(), SDL_GetError());
                success = false;
            }
        } else {
            auto tmp = text.text;
            tmp.append(std::to_string(*text.type));

            if(!text.texture.loadFromRenderedText(tmp,
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
