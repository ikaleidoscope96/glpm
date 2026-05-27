#include <cstddef>
#include <iostream>
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include "Grid.h"

Grid::Grid()
{
    for (size_t row = 0; row < grid_.size(); ++row) {
        for (size_t col = 0; col < grid_[row].size(); ++col) {
            Cell& cell{grid_[row][col]};
            cell.rect.x = row * kCellSize + (kScreenWidth / 4.f);
            cell.rect.y = col * kCellSize;
        }
    }

    grid_[20][20].isAlive = true;
    grid_[21][21].isAlive = true;
    grid_[22][19].isAlive = true;
    grid_[22][20].isAlive = true;
    grid_[22][21].isAlive = true;
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
    float mouseX,mouseY;
    size_t x,y;

    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_SPACE:
            paused_ = !paused_;
            break;
        }
    }

    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= (kScreenWidth / 4.f) &&
            mouseX <= (kScreenWidth / 4.f + kScreenHeight))
        {
            x = static_cast<size_t>(((mouseX - (kScreenWidth / 4.f)) / kCellSize));
            y = static_cast<size_t>(mouseY / kCellSize);
            grid_[x][y].isAlive = !(grid_[x][y].isAlive);
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
