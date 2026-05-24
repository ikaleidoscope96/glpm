#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <SDL3/SDL_init.h>
#include <SDL3_mixer/SDL_mixer.h>
#include "Mixer.h"

Mixer::Mixer()
{
    if (!MIX_Init()) {
        SDL_Log("SDL_Mixer could not initialize! SDL error: %s\n", SDL_GetError());
        success_ = false;
    }

    if (mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
        mixer_ == nullptr)
    {
        SDL_Log("SDL_mixer could not create mixer! SDL_mixer Error: %s\n", SDL_GetError());
        success_ = false;
    }

    loadSounds("../assets/music", songs_, false);
    musicTrack_ = createTrack(songs_);

    loadSounds("../assets/sound-effects", soundEffects_, true);
    soundEffectsTrack_ = createTrack(soundEffects_);
}

Mixer::~Mixer()
{
    destroy();
}

bool Mixer::isSuccessful()
{
    return success_;
}

void Mixer::loadSounds(const std::string& path,
                       std::unordered_map<std::string, MIX_Audio*>& map,
                       bool precode)
{
    namespace fs = std::filesystem;

    try {
        for (const auto& entry : fs::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                const std::string name{entry.path().stem()};
                map.emplace(name, nullptr);

                if (map[name] = MIX_LoadAudio(mixer_, entry.path().c_str(), precode);
                    map[name] == nullptr)
                {
                    SDL_Log("Unable to load sound: %s! SDL_mixer error: %s\n",
                            name.c_str(), SDL_GetError());
                    success_ = false;
                }
            }

        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
        success_ = false;
    }
}

MIX_Track* Mixer::createTrack(const std::unordered_map<std::string, MIX_Audio*>& map)
{
    MIX_Track* track{nullptr};

    for (const auto& [name, sound] : map) {
        if (sound == nullptr) {
            std::cerr << "Error null sound: " << name << '\n';
            success_ = false;
            return track;
        }
    }
    if (track = MIX_CreateTrack(mixer_); track == nullptr) {
        SDL_Log("Failed to create track! SDL_mixer Error: %s\n", SDL_GetError());
        success_ = false;
    }
    return track;
}

void Mixer::play(const std::string& audioName, const std::string& soundName)
{
    MIX_Track* track{nullptr};
    std::unordered_map<std::string, MIX_Audio*> audio;

    if (audioName == "songs_") {
        track = musicTrack_;
        audio = songs_;
    } else if (audioName == "soundEffects_") {
        track = soundEffectsTrack_;
        audio = soundEffects_;
    } else {
        std::cerr << "Invaild audioName: " << audioName << '\n';
        return;
    }

    if (!audio.contains(soundName)) {
        throw std::invalid_argument("Invaild audioName: " + soundName);
    } else if (audio.at(soundName) == nullptr) {
        std::cerr << "Error sound " << soundName
                  << " is null and cannot be played" << '\n';
    }

    MIX_SetTrackAudio(track, audio[soundName]);
    MIX_PlayTrack(track, 0);
}

void Mixer::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_1:
            play("soundEffects_", "pew");
            break;

        case SDLK_2:
            play("soundEffects_", "interrupt");
            break;

        case SDLK_3:
            play("songs_", "Airport Lounge");
            break;

        case SDLK_4:
            play("songs_", "Aurea Carmina");
            break;

        case SDLK_5:
            play("songs_", "Disco Lounge");
            break;
        }
    }
}

void Mixer::destroy()
{
    MIX_DestroyTrack(musicTrack_);
    musicTrack_ = nullptr;

    MIX_DestroyTrack(soundEffectsTrack_);
    soundEffectsTrack_ = nullptr;

    for (auto& [name, sound] : songs_) {
        MIX_DestroyAudio(songs_[name]);
        songs_[name] = nullptr;
    }
    songs_.clear();

    for (auto& [name, sound] : soundEffects_) {
        MIX_DestroyAudio(soundEffects_[name]);
        soundEffects_[name] = nullptr;
    }
    soundEffects_.clear();

    MIX_DestroyMixer(mixer_);
    mixer_ = nullptr;
}
