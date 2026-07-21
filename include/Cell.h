#pragma once

#include "raylib.h"
#include "Entity.h"
#include "Genome.h"
#include <vector>
#include <iostream>
#include "Sensors.h"

class World;

class Cell : public Entity {
public:
    Genome genome;
    Sensor sensor;
    float health = 1000.0f;
    int age = 2000;
    
    float maxVel = 30.0f;
    float curVel = 0.0f;

    Cell() = default;
    Cell(int health): Entity(), health(health) {}
    Cell(std::vector<float> parentsGenome): Entity(), genome(parentsGenome) {} 

    Cell(Vector2 pos): Entity{pos} {
        color = {100, 180, 255, 255};
        type = Type::Cell;
        radius = health / 20.0f;
    }
    Cell(Vector2 pos, std::vector<float> parentsGenome): Entity{pos} {
        color = {100, 180, 255, 255};
        type = Type::Cell;
        radius += health / 20.0f;
        genome.genes = parentsGenome;
        genome.mutate();
    }

    Cell(Vector2 pos, int health, std::vector<float> parentsGenome): Entity{pos}, health(health), genome(parentsGenome) {}

    void update(World& world) override;
    std::vector<float> think(const std::vector<float>& inputs);
    void eat(World& world);
    void reproduce(World& world);
    
};