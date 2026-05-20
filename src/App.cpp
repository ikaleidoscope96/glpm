#include <filesystem>
#include <iostream>
#include <string>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3/SDL_render.h>
#include "App.h"
#include "Constants.h"

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

    if (!MIX_Init()) {
        SDL_Log("SDL_Mixer could not initialize! SDL error: %s\n", SDL_GetError());
        return;
    }

    if (mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        mixer_ == nullptr)
    {
        SDL_Log("SDL_mixer could not create mixer! SDL_mixer Error: %s\n", SDL_GetError());
        return;
    }

    if (!loadMedia()) {
        return;
    }

    success_ = true;
}

App::~App()
{
    MIX_DestroyTrack(musicTrack_);
    musicTrack_ = nullptr;
    MIX_DestroyTrack(effectTrack_);
    effectTrack_ = nullptr;

    for (auto& [name, song] : songs_) {
        MIX_DestroyAudio(song);
        song = nullptr;
    }
    songs_.clear();

    for (auto& [name, soundEffect] : soundEffects_) {
        MIX_DestroyAudio(soundEffect);
        soundEffect = nullptr;
    }
    soundEffects_.clear();

    MIX_DestroyMixer(mixer_);
    mixer_ = nullptr;

    TTF_CloseFont(font_);
    font_ = nullptr;

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

bool App::loadMedia()
{
    namespace fs = std::filesystem;

    bool success{ true };

    try {
        const std::string path{"../assets/music"};

        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                const std::string songName{entry.path().stem()};
                songs_.emplace(songName, nullptr);

                if (songs_[songName] = MIX_LoadAudio(mixer_, entry.path().c_str(), false);
                    songs_[songName] == nullptr)
                {
                    SDL_Log("Unable to load song: %s! SDL_mixer error: %s\n",
                            songName.c_str(), SDL_GetError());
                    success = false;
                }
            }

        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        success = false;
    }

    try {
        const std::string path{"../assets/sound-effects"};

        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                const std::string soundEffectName{entry.path().stem()};
                soundEffects_.emplace(soundEffectName, nullptr);

                if (soundEffects_[soundEffectName] = MIX_LoadAudio(mixer_, entry.path().c_str(), false);
                    soundEffects_[soundEffectName] == nullptr)
                {
                    SDL_Log("Unable to load sound effect: %s! SDL_mixer error: %s\n",
                            soundEffectName.c_str(), SDL_GetError());
                    success = false;
                }
            }

        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
        success = false;
    }

    bool hasNullSoundEffect{false};
    for (const auto& [soundEffectName, soundEffect] : soundEffects_) {
        if (soundEffect == nullptr) {
            hasNullSoundEffect = true;
        }
    }
    if (!hasNullSoundEffect) {
        if (effectTrack_ = MIX_CreateTrack(mixer_); effectTrack_ == nullptr) {
            SDL_Log("Failed to create sound effect track! SDL_mixer Error: %s\n", SDL_GetError());
            success = false;
        }
    }

    bool hasNullSongs{false};
    for (const auto& [songName, song] : songs_) {
        if (song == nullptr) {
            hasNullSongs = true;
        }
    }
    if (!hasNullSoundEffect) {
        if (musicTrack_ = MIX_CreateTrack(mixer_); musicTrack_ == nullptr) {
            SDL_Log("Failed to create songs track! SDL_mixer Error: %s\n", SDL_GetError() );
            success = false;
        }
    }
    return success;
}

void App::handleEvents(SDL_Event event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_1:
            MIX_SetTrackAudio(effectTrack_, soundEffects_["pew"]);
            MIX_PlayTrack(effectTrack_, 0);
            break;

        case SDLK_2:
            MIX_SetTrackAudio(effectTrack_, soundEffects_["interrupt"]);
            MIX_PlayTrack(effectTrack_, 0);
            break;

        case SDLK_3:
            MIX_SetTrackAudio(musicTrack_, songs_["Airport Lounge"]);
            MIX_PlayTrack(musicTrack_, 0);
            break;

        case SDLK_4:
            MIX_SetTrackAudio(musicTrack_, songs_["Aurea Carmina"]);
            MIX_PlayTrack(musicTrack_, 0);
            break;

        case SDLK_5:
            MIX_SetTrackAudio(musicTrack_, songs_["Disco Lounge"]);
            MIX_PlayTrack(musicTrack_, 0);
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
