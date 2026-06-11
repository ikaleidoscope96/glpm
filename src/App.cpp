#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "App.h"
#include "Constants.h"

App::App()
{
    SDL_SetAppMetadata(kTitle, kVersion, kIdentifer);

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

    SDL_SetRenderLogicalPresentation(renderer_, kScreenWidth, kScreenHeight,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    success = true;
}

App::~App()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

void App::handleEvent(const SDL_Event& event)
{
    grid_.handleEvent(event);
    mixer_.handleEvent(event);
}

void App::update()
{
    grid_.update();
}

void App::render()
{
    SDL_SetRenderDrawColor(renderer_, 0x12, 0x12, 0x1E, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(renderer_);

    grid_.render(renderer_);

    SDL_RenderPresent(renderer_);
}
