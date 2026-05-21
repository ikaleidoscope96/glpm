#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL_main.h>
#include "App.h"
#include "Constants.h"

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    SDL_SetAppMetadata(kTitle, "1.0", "com.example.renderer-clear");
    App* app = new App;

    if (!app->isSuccessful()) {
        SDL_Log("Couldn't initalize program!\n");
        return SDL_APP_FAILURE;
    }

    *appstate = app;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    App* app = static_cast<App*>(appstate);

    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    app->handleEvents(*event);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    App* app = static_cast<App*>(appstate);

    app->update();
    app->render();

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    App* app = static_cast<App*>(appstate);
    delete app;
    app = nullptr;
}
