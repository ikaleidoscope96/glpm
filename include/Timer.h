#pragma once

#include <SDL3/SDL.h>
#include <cstdint>

class Timer
{
public:
    uint64_t time{0};
    uint64_t lastUpdate{0};

    Timer();

    void update();
    void togglePlaying();
    bool isPaused();

private:
    uint64_t startTicks_;
    uint64_t pausedTicks_{0};
    bool paused_{false};

    void pause();
    void unpause();
};
