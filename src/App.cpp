#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_render.h>
#include "App.h"
#include "Constants.h"
#include "Mixer.h"

App::App()
{
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        SDL_Log("SDL could not initialize! SDL error: %s\n", SDL_GetError());
        return;
    }

    if (!SDL_CreateWindowAndRenderer(kTitle,
                                     kScreenWidth,
                                     kScreenHeight,
                                     SDL_WINDOW_FULLSCREEN |
                                     SDL_WINDOW_RESIZABLE,
                                     &window_,
                                     &renderer_))
    {
        SDL_Log("Window could not be created! SDL error: %s\n", SDL_GetError());
        return;
    }

    mixer_ = new Mixer();

    if (!mixer_->isSuccessful()) {
        SDL_Log("Mixer could not initialize! SDL error: %s\n", SDL_GetError());
        return;
    }

    success_ = true;
}

App::~App()
{

    delete mixer_;
    mixer_ = nullptr;

    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
    SDL_DestroyWindow(window_);
    window_ = nullptr;

    MIX_Quit();
    SDL_Quit();
}

// SDL_Renderer* App::getRenderer()
// {
//     return renderer_;
// }

bool App::isSuccessful()
{
    return success_;
}

void App::handleEvents(SDL_Event event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_1:
            mixer_->play("soundEffects_", "pew");
            break;

        case SDLK_2:
            mixer_->play("soundEffects_", "interrupt");
            break;

        case SDLK_3:
            mixer_->play("songs_", "Airport Lounge");
            break;

        case SDLK_4:
            mixer_->play("songs_", "Aurea Carmina");
            break;

        case SDLK_5:
            mixer_->play("songs_", "Disco Lounge");
            break;
        }
    }
}

void App::update(){}

void App::render()
{
    const double now = ((double)SDL_GetTicks()) / 1000.f;
    const float red = (float) (0.5 + 0.5 * SDL_sin(now));
    const float green = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue = (float) (0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));

    SDL_SetRenderDrawColorFloat(renderer_,red, green, blue, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer_);
    SDL_RenderPresent(renderer_);
}
