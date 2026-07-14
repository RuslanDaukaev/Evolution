#include "entities.h"
#include <iostream>
#include "world.h"

Cell::Cell(int x, int y) : Entity{x, y} {}

void Cell::update(World& world) {
    hasMoveThisTurn = true;

    int dir = world.getRandomInt(0, 3);
    switch(dir) {
        case 0:
            if (world.isEmpty(this->x + 1, this->y)) {
                world.grid[this->y][this->x + 1] = std::move(world.grid[this->y][this->x]);
                this->x += 1;
            }
        break;

        case 1:
        if (world.isEmpty(this->x - 1, this->y)) {
            world.grid[this->y][this->x - 1] = std::move(world.grid[this->y][this->x]);
            this->x -= 1;
        }
        break;

        case 2:
        if (world.isEmpty(this->x, this->y + 1)) {
            world.grid[this->y + 1][this->x] = std::move(world.grid[this->y][this->x]);
            this->y += 1;
        }
        break;

        case 3:
        if (world.isEmpty(this->x, this->y - 1)) {
            world.grid[this->y - 1][this->x] = std::move(world.grid[this->y][this->x]);
            this->y -= 1;
        }
        break;
    }
}

char Cell::getSymbol() const {
    return '0';
}