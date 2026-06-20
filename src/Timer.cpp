#include "Timer.h"

Timer::Timer()
{
    startTicks_ = SDL_GetTicks();
}

void Timer::update()
{
    time = paused_ ? pausedTicks_ : SDL_GetTicks() - startTicks_;
}

void Timer::pause()
{
    if (!paused_) {
        paused_ = true;
        pausedTicks_ = SDL_GetTicks() - startTicks_;
        startTicks_ = 0;
    }
}

void Timer::unpause()
{
    if (paused_) {
        paused_ = false;
        startTicks_ = SDL_GetTicks() - pausedTicks_;
        pausedTicks_ = 0;
    }
}

void Timer::togglePlaying() {
    if (paused_) {
        unpause();
    } else {
        pause();
    }
}

bool Timer::isPaused() {
    return paused_;
}
