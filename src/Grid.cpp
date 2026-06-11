#include <cstddef>
#include <SDL3/SDL_blendmode.h>
#include <SDL3/SDL_keycode.h>
#include <SDL3/SDL_render.h>
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


    // Create example structures
    grid_[20][26].isAlive = true;
    grid_[20][27].isAlive = true;
    grid_[20][28].isAlive = true;

    grid_[80][44].isAlive = true;
    grid_[80][45].isAlive = true;
    grid_[81][44].isAlive = true;
    grid_[81][45].isAlive = true;

    grid_[20][20].isAlive = true;
    grid_[21][21].isAlive = true;
    grid_[22][19].isAlive = true;
    grid_[22][20].isAlive = true;
    grid_[22][21].isAlive = true;

    font_.texts.push_back({"Data"});
    font_.texts.push_back({"Living Cells: ", &data_.livingCells});
    font_.texts.push_back({"Dying Cells: ", &data_.dyingCells});
    font_.texts.push_back({"Constant Cells: ", &data_.constantCells});
    font_.texts.push_back({"Oscillating Cells: ", &data_.oscillatingCells});
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

auto Grid::computeGenerations(int generations, auto& grid)
{
    if (generations == 0) {
        return grid;
    }

    const auto currentGen = grid;
    for (size_t row  = 0; row < currentGen.size(); ++row) {
        for (size_t col  = 0; col < currentGen[row].size(); ++col) {

            const Cell& currentCell{currentGen[row][col]};
            const int livingNeighbors{countNeighbors(row, col, currentGen)};
            Cell& updatedCell{grid[row][col]};

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

    generations--;
    return computeGenerations(generations, grid);
}

void Grid::clear()
{
    for (auto& row : grid_) {
        for (Cell& cell : row) {
            cell.isAlive = false;
        }
    }
}

void Grid::handleEvent(const SDL_Event& event)
{
    if (event.type == SDL_EVENT_KEY_DOWN) {
        switch (event.key.key) {
        case SDLK_SPACE:
            paused_ = !paused_;
            break;
        case SDLK_C:
            clear();
            break;
        case SDLK_N:
            paused_ = false;
            update();
            paused_ = true;
            break;
        }
    } else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        float mouseX,mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        if (mouseX >= (kScreenWidth / 4.f) &&
            mouseX <= (kScreenWidth / 4.f + kScreenHeight))
        {
            size_t x,y;
            x = static_cast<size_t>(((mouseX - (kScreenWidth / 4.f)) / kCellSize));
            y = static_cast<size_t>(mouseY / kCellSize);
            grid_[x][y].isAlive = !(grid_[x][y].isAlive);
        }
    }

}

void Grid::update()
{
    if (paused_) {
        return;
    }

    // Passing copy of grid_ to computeGenerations to prevent excessive copying
    // Since computeGenerations is recusive
    auto copy = grid_;
    auto nextGen{computeGenerations(1, copy)};
    const auto& secondGen{computeGenerations(2, copy)};
    const auto& thirdGen{computeGenerations(3, copy)};

    data_ = {0};
    for (size_t row  = 0; row < grid_.size(); ++row) {
        for (size_t col  = 0; col < grid_[row].size(); ++col) {

            const Cell& currentCell = grid_[row][col];
            Cell& nextCell = nextGen[row][col];
            const Cell& secondGenCell = secondGen[row][col];
            const Cell& thirdGenCell = thirdGen[row][col];

            // Revert any changes to cell colors
            nextCell.r = 0xFF;
            nextCell.g = 0xFF;
            nextCell.b = 0xFF;

            if (nextCell.isAlive) {
                data_.livingCells++;
            }

            if (nextCell.isAlive && secondGenCell.isAlive && thirdGenCell.isAlive) {
                nextCell.r = 0xFF;
                nextCell.g = 0xFF;
                nextCell.b = 0x00;
                data_.constantCells++;
            } else if (nextCell.isAlive && !secondGenCell.isAlive) {
                nextCell.r = 0xFF;
                nextCell.g = 0x30;
                nextCell.b = 0x30;
                data_.dyingCells++;
            } else if (currentCell.isAlive != nextCell.isAlive &&
                       secondGenCell.isAlive != thirdGenCell.isAlive &&
                       nextCell.isAlive == true)
            {
                nextCell.r = 0x00;
                nextCell.g = 0x00;
                nextCell.b = 0xFF;
                data_.oscillatingCells++;
            }
        }
    }

    grid_ = nextGen;
}

void Grid::render(SDL_Renderer* renderer)
{
    // Draw grid rows
    for (size_t row = 0; row <= grid_.size(); ++row) {
        SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0x55);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
        SDL_RenderLine(renderer,
                       kScreenWidth / 4.f,
                       kCellSize * row,
                       kScreenWidth / 4.f + kScreenHeight,
                       kCellSize * row);
    }

    // Draw grid collumns
    for (size_t col = 0; col <= grid_[0].size(); ++col) {
        SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, 0x55);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
        SDL_RenderLine(renderer,
                       kScreenWidth / 4.f + (kCellSize * col),
                       0.f,
                       kScreenWidth / 4.f + (kCellSize * col),
                       kScreenHeight);
    }

    // Render each cell
    for (size_t row = 0; row < grid_.size(); ++row) {
        for (size_t col = 0; col < grid_[row].size(); ++col) {
            Cell& cell = grid_[row][col];
            if (cell.isAlive) {
                SDL_SetRenderDrawColor(renderer, cell.r, cell.g, cell.b, 0xFF);
                SDL_RenderFillRect(renderer, &cell.rect);
            }
        }
    }

    font_.render(renderer);
}
