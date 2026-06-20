#pragma once

#include <array>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_rect.h>
#include "Constants.h"
#include "Font.h"
#include "Timer.h"

class Grid
{
public:
    static constexpr int kGridSize{100};
    static constexpr float kCellSize{static_cast<float>(kScreenHeight)/ kGridSize};

    Grid();

    void handleEvent(const SDL_Event& event);
    void update();
    void render(SDL_Renderer* renderer);

private:
    struct Cell {
        bool isAlive{false};
        SDL_FRect rect{0.f, 0.f, kCellSize, kCellSize};
        uint8_t r{0xFF};
        uint8_t g{0xFF};
        uint8_t b{0xFF};
    };

    struct Data {
        int livingCells{0};
        int dyingCells{0};
        int constantCells{0};
        int oscillatingCells{0};
        int tickSpeed{1};
    };

    Data data_;
    Font font_;
    Timer timer_;
    std::array<std::array<Cell, kGridSize>, kGridSize> grid_;

    int countNeighbors (size_t row, size_t col, const auto& grid);
    auto computeGenerations(int generations, auto& grid);
    void advanceGeneration();
    void clear();
};
