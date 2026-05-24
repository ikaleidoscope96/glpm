#ifndef GIRD_H
#define GRID_H

#include <array>
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include "Constants.h"

class Grid
{
public:
    static constexpr int kGridSize{100};
    static constexpr float kCellSize{static_cast<float>(kScreenHeight)/ kGridSize};

    Grid();

    void handleEvent(const SDL_Event& event);
    void advanceGeneration();
    void update();
    void render(SDL_Renderer* renderer);

private:
    struct Cell {
        bool isAlive{false};
        SDL_FRect rect{0.f, 0.f, kCellSize, kCellSize};
    };

    int countNeighbors (size_t row, size_t col, const auto& grid);

    std::array<std::array<Cell, kGridSize>, kGridSize> grid_;
    bool paused_{false};
};

#endif
