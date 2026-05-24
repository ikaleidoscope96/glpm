#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include <cstddef>
#include "Grid.h"
#include "Constants.h"

Grid::Grid()
{
    for (size_t row = 0; row < grid_.size(); ++row) {
        for (size_t col = 0; col < grid_[row].size(); ++col) {
            Cell& cell{grid_[row][col]};
            cell.rect.x = col * kCellSize + (kScreenWidth / 4.f);
            cell.rect.y = row * kCellSize;
        }
    }

    grid_[50][50].isAlive = true;
    grid_[51][51].isAlive = true;
    grid_[52][49].isAlive = true;
    grid_[52][50].isAlive = true;
    grid_[52][51].isAlive = true;

    grid_[98][10].isAlive = true;
    grid_[98][11].isAlive = true;
    grid_[99][10].isAlive = true;
    grid_[99][11].isAlive = true;
}

int Grid::countNeighbors(size_t row, size_t col, const auto& grid)
{
    int livingNeighbors{0};

    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (i == 0 && j == 0) {
                continue;
            }

            const int x{(static_cast<int>(row) + i + kGridSize) % kGridSize};
            const int y{(static_cast<int>(col) + j + kGridSize) % kGridSize};
            const Cell& neighbor{grid[x][y]};

            if (neighbor.isAlive) {
                ++livingNeighbors;
            }
        }
    }
    return livingNeighbors;
}

void Grid::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_SPACE:
            paused_ = !paused_;
        }
    }
}

void Grid::advanceGeneration()
{
    const auto currentGen = grid_;
    for (size_t row  = 0; row < currentGen.size(); ++row) {
        for (size_t col  = 0; col < currentGen[row].size(); ++col) {

            const Cell& currentCell{currentGen[row][col]};
            const int livingNeighbors{countNeighbors(row, col, currentGen)};
            Cell& updatedCell{grid_[row][col]};

            if (currentCell.isAlive && (livingNeighbors < 2 || livingNeighbors > 3)) {
                updatedCell.isAlive = false;
            } else if (currentCell.isAlive && (livingNeighbors == 2 || livingNeighbors == 3)) {
                updatedCell.isAlive = true;
            } else if (!currentCell.isAlive && livingNeighbors == 3) {
                updatedCell.isAlive = true;
            } else {
                updatedCell.isAlive = false;
            }

        }
    }
}

void Grid::update()
{
    if (!paused_) {
        advanceGeneration();
    }
}

void Grid::render(SDL_Renderer* renderer)
{
    for (size_t row = 0; row <= grid_.size(); ++row) {
        SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0x55);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
        SDL_RenderLine(renderer,
                       kScreenWidth / 4.f,
                       kCellSize * row,
                       kScreenWidth / 4.f + kScreenHeight,
                       kCellSize * row);
    }

    for (size_t col = 0; col <= grid_[0].size(); ++col) {
        SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0x55);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
        SDL_RenderLine(renderer,
                       kScreenWidth / 4.f + (kCellSize * col),
                       0.f,
                       kScreenWidth / 4.f + (kCellSize * col),
                       kScreenHeight);
    }

    for (size_t row = 0; row < grid_.size(); ++row) {
        for (size_t col = 0; col < grid_[row].size(); ++col) {
            const Cell& cell = grid_[row][col];
            if (cell.isAlive) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer, &cell.rect);
            }
        }
    }
}
