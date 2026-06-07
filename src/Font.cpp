#include "Font.h"

Font::Font(SDL_Renderer* renderer, Grid::Data* data):
renderer_{renderer}
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

    SDL_Color textColor{ 0xFF, 0xFF, 0xFF, 0xFF };

    texts_.push_back({"Data"});
    texts_.push_back({"Living Cells: ", &data->livingCells});
    texts_.push_back({"Dying Cells: ", &data->dyingCells});
    texts_.push_back({"Constant Cells: ", &data->constantCells});
    texts_.push_back({"Oscillating Cells: ", &data->oscillatingCells});

    success = true;
}

Font::~Font()
{
    for (auto& text: texts_) {
        text.texture.destroy();
    }

    TTF_CloseFont(font_);
    font_ = nullptr;
}

void Font::update()
{
    SDL_Color textColor{ 0xFF, 0xFF, 0xFF, 0xFF };
    std::string path{"../assets/fonts/VT323-Regular.ttf"};

    for (auto& text: texts_) {
        auto tmp = text.text;
        text.texture.destroy();

        if (text.type != nullptr) {
            tmp.append(std::to_string(*text.type));
        }

        if(!text.texture.loadFromRenderedText(tmp,
                                              textColor,
                                              font_,
                                              renderer_))
        {
            SDL_Log( "Could not load text texture %s! SDL_ttf Error: %s\n", path.c_str(), SDL_GetError() );
            success = false;
        }
    }
}

void Font::render()
{
    for (float y{0}; auto& text : texts_) {
        text.texture.render(0, y, renderer_);
        y += 32;
    }
}
