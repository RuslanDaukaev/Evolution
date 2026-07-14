#pragma once

#include "entity.h"
#include <memory>

class Cell : public Entity {
public:
    Cell(int x, int y);
    char getSymbol() const override;
    void update(World& world) override;
private:

};

class Food : public Entity {

};