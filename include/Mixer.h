#pragma once

#include <string>
#include <unordered_map>
#include <SDL3_mixer/SDL_mixer.h>

class Mixer
{
public:
    bool success{true};

    Mixer();
    ~Mixer();

    void handleEvent(const SDL_Event& event);
    void destroy();

private:
    std::unordered_map<std::string, MIX_Audio*> songs_;
    std::unordered_map<std::string, MIX_Audio*> soundEffects_;

    MIX_Track* musicTrack_{nullptr};
    MIX_Track* soundEffectsTrack_{nullptr};
    MIX_Mixer* mixer_{nullptr};

    MIX_Track* createTrack(const std::unordered_map<std::string, MIX_Audio*>& map);
    void play(const std::string& audioName, const std::string& soundName);
    void loadSounds(const std::string& path,
                    std::unordered_map<std::string, MIX_Audio*>& map,
                    bool precode);
};
