#pragma once

#include "raymath.h"
#include "Random.h"
#include "Entity.h"

class World;

class Plant : public Entity {
public:
    int value = 500;

    Plant() = default;
    Plant(Vector2 pos) : Entity{pos} {
        color = {10, 200, 50, 255};
        type = Type::Plant;
        // value = Random::getInt(50, 150);
        radius = 50;
    }
    Plant(int value) : Entity(), value(value) {}
    Plant(Vector2 pos, int value) : Entity{pos}, value(value) {}
    

    void update(World& world) override {
        int a = 5;
    }

};