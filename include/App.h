#ifndef APP_H
#define APP_H

#include <string>
#include <unordered_map>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>

class App
{
public:
    App();
    ~App();

    // SDL_Renderer* getRenderer();

    bool isSuccessful();
    bool loadMedia();

    void handleEvents(SDL_Event event);
    void update();
    void render();

private:
    MIX_Track* musicTrack_{nullptr};
    MIX_Track* effectTrack_{nullptr};

    std::unordered_map<std::string, MIX_Audio*> songs_;
    std::unordered_map<std::string, MIX_Audio*> soundEffects_;

    MIX_Mixer* mixer_{nullptr};
    TTF_Font* font_{nullptr};
    SDL_Window* window_{nullptr};
    SDL_Renderer* renderer_{nullptr};

    bool success_{false};
};

#endif
