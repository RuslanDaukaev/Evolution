#include "world.h"
#include <iostream>
#include "entities.h"

World::World(int width, int height) :
    width(width),
    height(height)
{
    grid.resize(height);

    for (int i = 0; i < height; i++) {
        grid[i].resize(width);
    }

    std::random_device rd; 
    rng.seed(rd());
}

void World::draw() const {

    std::cout << "\033[H";

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] == nullptr)
                std::cout << ' ';
            else
                std::cout << grid[i][j]->getSymbol();
        }
        std::cout << '\n';
    }
    std::cout << std::flush;
}

void World::spawnCell(int x, int y) {
    if (!isEmpty(x, y))
        return;
    else
        grid[y][x] = std::make_unique<Cell>(x, y);
}

bool World::isEmpty(int x, int y) {
    if (x < 0 || y < 0 || x >= width || y >= height)
        return false;
    else
        return grid[y][x] == nullptr;
}

int World::getRandomInt(int min, int max) {

    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng); 
}

void World::update() {

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] != nullptr)
                if (grid[i][j]->hasMoveThisTurn == false)
                    grid[i][j]->update(*this);
        }
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (grid[i][j] != nullptr)
                grid[i][j]->hasMoveThisTurn = false;
        }
    }
}