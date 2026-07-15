#pragma once

#include <memory>

class World;

class Entity {
public:

    int x, y;
    bool hasMoveThisTurn = false;

    Entity(int x, int y): x(x), y(y) {}
    virtual ~Entity() = default;

    virtual void update(World& world) = 0;
    virtual std::string getSymbol() const = 0;

private:
    
};