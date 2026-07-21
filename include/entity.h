#pragma once
#include "raylib.h"
#include "Random.h"
#include "WorldSettings.h"

class World;

enum class Type {
    Plant,
    Cell
};

class Entity {
public:
    Vector2 pos;
    Type type;
    float radius = 50.0f;
    Color color = {255, 255, 255, 150};
    Entity() {
        pos.x = WORLD_WIDTH * 0.5f;
        pos.y = WORLD_HEIGHT * 0.5f;
    }
    Entity(Vector2 pos) : pos(pos) {}
    virtual ~Entity() = default;

    virtual void update(World& world) = 0;
};