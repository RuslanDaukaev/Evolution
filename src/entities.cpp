#include "entities.h"
#include <iostream>
#include "world.h"

const int dx[8] = { 1, -1,  0,  0,  1,  1, -1, -1 };
const int dy[8] = { 0,  0,  1, -1,  1, -1,  1, -1 };

Cell::Cell(int x, int y, World& world) : Entity{x, y}, health(50), genome(16) {
    for (size_t i = 0; i < genome.size(); i++) {
        genome[i] = world.getRandomInt(0, 16);
    }

    
}

Cell::Cell(int x, int y, World& world, std::vector<int> genome) : Entity{x, y}, health(50), genome(16) {
    this->genome = genome;
    if (world.getRandomInt(0, 10) == 10)
        this->genome[world.getRandomInt(0, 15)] = world.getRandomInt(0, 16);
}

void Cell::update(World& world) {
    hasMoveThisTurn = true;

    if (health > 0) {
        int dir = world.getRandomInt(0, 15);
        int act = genome[dir];

        switch(act) {
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
            case 4:
                if (world.isEmpty(this->x + 1, this->y + 1)) {
                    world.grid[this->y + 1][this->x + 1] = std::move(world.grid[this->y][this->x]);
                    this->x += 1;
                    this->y += 1;
                }
            break;
            case 5:
                if (world.isEmpty(this->x + 1, this->y - 1)) {
                    world.grid[this->y - 1][this->x + 1] = std::move(world.grid[this->y][this->x]);
                    this->x += 1;
                    this->y -= 1;
                }
            break;
            case 6:
                if (world.isEmpty(this->x - 1, this->y + 1)) {
                    world.grid[this->y + 1][this->x - 1] = std::move(world.grid[this->y][this->x]);
                    this->x -= 1;
                    this->y += 1;
                }
            break;
            case 7:
                if (world.isEmpty(this->x - 1, this->y - 1)) {
                    world.grid[this->y - 1][this->x - 1] = std::move(world.grid[this->y][this->x]);
                    this->x -= 1;
                    this->y -= 1;
                }
            break;

            case 9:
            if (health >= 200) {
                if (world.isEmpty(this->x + 1, this->y + 1)) {
                    world.spawnCell(this->x + 1, this->y + 1, this->genome);
                }
            }
            
            break;
            case 10:
                health += 10;
            break;
        }





    }
    health--;

    if (health <= 0) {
        world.grid[y][x] = nullptr;
    }

}

std::string Cell::getSymbol() const {
    return "o";
}





Food::Food(int x, int y) : Entity{x, y}, value(5) {}

std::string Food::getSymbol() const {
    return "\033[32mx\033[0m";
}

void Food::update(World& world) {

}