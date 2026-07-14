#pragma once
#include <memory>
#include <vector>
#include <random>
#include "entity.h"

class World {
public:

    World(int width, int height);
    void spawnCell(int x, int y);
    bool isEmpty(int x, int y);
    void draw() const;

    void update();
    int getRandomInt(int min, int max);
    std::vector<std::vector<std::unique_ptr<Entity>>> grid;

private:
    int width, height;
    

    std::mt19937 rng;
};