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

    SDL_SetRenderLogicalPresentation(renderer_, kScreenWidth, kScreenHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    grid_ = new Grid();
    mixer_ = new Mixer();

    if (!mixer_->isSuccessful()) {
        SDL_Log("Mixer could not initialize! SDL error: %s\n", SDL_GetError());
        return;
    }

    success_ = true;
}

App::~App()
{
    delete grid_;
    grid_ = nullptr;

    delete mixer_;
    mixer_ = nullptr;

    SDL_DestroyRenderer(renderer_);
    renderer_ = nullptr;
    SDL_DestroyWindow(window_);
    window_ = nullptr;

    MIX_Quit();
    SDL_Quit();
}

SDL_Renderer* const App::getRenderer()
{
    return renderer_;
}

bool App::isSuccessful()
{
    return success_;
}

void App::handleEvent(const SDL_Event& event)
{
    grid_->handleEvent(event);
    mixer_->handleEvent(event);
}

void App::update()
{
    grid_->update();
}

void App::render()
{
    SDL_SetRenderDrawColor(renderer_, 0x12, 0x12, 0x1E, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer_);

    grid_->render(getRenderer());

    SDL_RenderPresent(renderer_);
}
