#pragma once

#include "entity.h"
#include <memory>
#include <vector>
#include <string>

class Cell : public Entity {
public:
    Cell(int x, int y, World& world);
    Cell(int x, int y, World& world, std::vector<int> genome);
    std::string getSymbol() const override;
    void update(World& world) override;

private:

    int health;

    std::vector<int> genome;

};

class Food : public Entity {
public:
    Food(int x, int y);
    std::string getSymbol() const override;
    void update(World& world) override;
private:
    int value;
};