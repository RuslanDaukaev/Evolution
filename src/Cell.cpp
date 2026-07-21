#include "Cell.h"
#include "World.h"

#include <cmath>

void Cell::eat(World& world) {
    std::vector<Entity*> nearby = world.getNearbyEntities(this->pos);

    for (Entity* entity : nearby) {
        if (entity == this || entity->type != Type::Plant) {
            continue;
        }

        Vector2 delta = Vector2Subtract(entity->pos, pos);
        const float distSq = delta.x * delta.x + delta.y * delta.y;
        float touchDist = radius + entity->radius;
        if (distSq > touchDist * touchDist) {
            continue;
        }
        Plant* plant = static_cast<Plant*>(entity);
        health += plant->value;
        world.removeEntity(entity);
        return;
    }
}

void moveCell(Cell& cell, const std::vector<float>& outputs) {
    if (outputs.size() < 2) {
        return;
    }
    const float speed = std::fabs(outputs[0]) * cell.maxVel;
    const float angle = outputs[1] * PI;
    cell.pos.x += std::cos(angle) * speed;
    cell.pos.y += std::sin(angle) * speed;
    cell.curVel = speed;
    cell.pos.x = std::clamp(cell.pos.x, cell.radius, WORLD_WIDTH - cell.radius);
    cell.pos.y = std::clamp(cell.pos.y, cell.radius, WORLD_HEIGHT - cell.radius);
}

void Cell::reproduce(World& world) {
    world.spawnCell(this->pos, *this);
}

void Cell::update(World& world) {
    health -= sensor.senceRadius / 1000.0f + curVel / 10.0f;
    age--;
    radius = health / 20.0f;
    if (health <= 0 || age <= 0) {
        world.removeEntity(this);
        return;
    }

    if (health > 2000) {
        reproduce(world);
        health /= 2;
    }

    std::vector<Entity*> nearby = world.getNearbyEntities(this->pos);
    std::vector<float> sensorData = sensor.senceEntity(*this, nearby);
    sensorData.push_back(static_cast<float>(health / 500.0f));
    sensorData.push_back(static_cast<float>(curVel / maxVel));
    std::vector<float> outputData = think(sensorData);
    eat(world);
    moveCell(*this, outputData);
    world.resolveCellCollision(this);

//     float angle = sensorData[1] * PI;
// pos.x += std::cos(angle) * maxVel;
// pos.y += std::sin(angle) * maxVel;
}

float activate(float x) {
    return std::tanh(x);
}

std::vector<float> Cell::think(const std::vector<float>& inputs) {
    std::vector<float> hiddenNodes(HIDDEN_NEURO_COUNT, 0.0f);
    std::vector<float> outputs(OUTPUT_NEURO_COUNT, 0.0f);
      
    int geneIndex = 0;

    for (int h = 0; h < HIDDEN_NEURO_COUNT; h++) {
        float sum = 0.0f;
        
        for (int i = 0; i < INPUT_NEURO_COUNT; i++) {
            sum += inputs[i] * genome.genes[geneIndex++];        
        }
    
        sum += genome.genes[geneIndex++];
        hiddenNodes[h] = activate(sum);
    }
    for (int o = 0; o < OUTPUT_NEURO_COUNT; o++) {
        float sum = 0.0f;

        for (int h = 0; h < HIDDEN_NEURO_COUNT; h++) {
            sum += hiddenNodes[h] * genome.genes[geneIndex++];
        }
        sum += genome.genes[geneIndex++];
        outputs[o] = activate(sum);
        
    }

    return outputs;
}