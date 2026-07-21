#pragma once

#include <iostream>
#include "raymath.h"
#include <vector>
#include <math.h>
#include "Entity.h"

class Cell;

class Sensor {
public:

    float senceRadius = 5000.0f;
    std::vector<float> senceResult;

    Sensor() {};

    std::vector<float>& senceEntity(Cell& cell, std::vector<Entity*> nearby);
    

    

};