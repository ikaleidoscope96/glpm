#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include "Grid.h"

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
}

Grid::~Grid()
{
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

void Grid::update()
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

void Grid::render(SDL_Renderer* renderer)
{
    for (size_t row = 0; row < grid_.size(); ++row) {
        for (size_t col = 0; col < grid_[row].size(); ++col) {

            const Cell& cell = grid_[row][col];

            SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0x01);
            SDL_RenderRect(renderer, &cell.rect);

            if (cell.isAlive) {
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(renderer, &cell.rect);
            }
        }
    }
}
