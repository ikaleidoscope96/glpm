#ifndef MIXER_H
#define MIXER_H

#include <string>
#include <unordered_map>
#include <SDL3_mixer/SDL_mixer.h>

class Mixer
{
public:
    Mixer();
    ~Mixer();

    bool isSuccessful();

    MIX_Track* createTrack(const std::unordered_map<std::string, MIX_Audio*>& map);

    void loadSounds(const std::string& path,
                    std::unordered_map<std::string, MIX_Audio*>& map,
                    bool precode);

    void play(const std::string& audioName, const std::string& soundName);

    void destroy();

private:
    MIX_Track* musicTrack_{nullptr};
    MIX_Track* soundEffectsTrack_{nullptr};

    std::unordered_map<std::string, MIX_Audio*> songs_;
    std::unordered_map<std::string, MIX_Audio*> soundEffects_;

    MIX_Mixer* mixer_{nullptr};
    bool success_{true};
};

#endif
