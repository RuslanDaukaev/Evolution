#include "Cell.h"
#include "World.h"
#include "Sensors.h"
#include "Genome.h"

#include <cmath>

struct PolarSignal {
    float distance = 0.0f;
    float angle = 0.0f;
    bool valid = false;
};

Vector2 centerOfMass(const std::vector<Vector2>& positions) {
    if (positions.empty()) {
        return {0.0f, 0.0f};
    }

    Vector2 sum{0.0f, 0.0f};
    for (const Vector2& pos : positions) {
        sum = Vector2Add(sum, pos);
    }
    return Vector2Scale(sum, 1.0f / static_cast<float>(positions.size()));
}

Vector2 nearestPosition(Vector2 self, const std::vector<Vector2>& positions) {
    if (positions.empty()) {
        return {0.0f, 0.0f};
    }

    Vector2 nearest = positions[0];
    float nearestDistSq = Vector2DistanceSqr(self, nearest);
    for (size_t i = 1; i < positions.size(); ++i) {
        const float distSq = Vector2DistanceSqr(self, positions[i]);
        if (distSq < nearestDistSq) {
            nearestDistSq = distSq;
            nearest = positions[i];
        }
    }
    return nearest;
}

PolarSignal polarFromTarget(Vector2 self, Vector2 target, bool hasTarget) {
    if (!hasTarget) {
        return {};
    }

    Vector2 dir = Vector2Subtract(target, self);
    return {
        Vector2Length(dir),
        std::atan2(dir.y, dir.x),
        true
    };
}

float normalizeDistance(float distance, float maxDistance) {
    if (maxDistance <= 0.0f) {
        return 0.0f;
    }
    return std::clamp(distance / maxDistance, 0.0f, 1.0f);
}

float normalizeAngle(float angle) {
    return std::clamp(angle / PI, -1.0f, 1.0f);
}

std::vector<float>& Sensor::senceEntity(Cell& cell, std::vector<Entity*> nearby) {
    std::vector<Vector2> posPlant;
    std::vector<Vector2> posCell;

    const float senseRadiusSq = senceRadius * senceRadius;

    for (Entity* entity : nearby) {
        if (entity == static_cast<Entity*>(&cell)) {
            continue;
        }

        Vector2 delta = Vector2Subtract(entity->pos, cell.pos);
        const float distSq = delta.x * delta.x + delta.y * delta.y;
        if (distSq > senseRadiusSq) {
            continue;
        }

        if (entity->type == Type::Cell) {
            posCell.push_back(entity->pos);
        } else if (entity->type == Type::Plant) {
            posPlant.push_back(entity->pos);
        }
    }

    const Vector2 plantCom = centerOfMass(posPlant);
    const Vector2 cellCom = centerOfMass(posCell);

    const PolarSignal plantSignal = polarFromTarget(cell.pos, nearestPosition(cell.pos, posPlant), !posPlant.empty());
    const PolarSignal cellSignal = polarFromTarget(cell.pos, cellCom, !posCell.empty());

    senceResult.assign(2, 0.0f);
    senceResult[0] = 1.0f - normalizeDistance(plantSignal.distance, senceRadius);
    senceResult[1] = plantSignal.valid ? normalizeAngle(plantSignal.angle) : 0.0f;
    // senceResult[2] = 0.0;
    // senceResult[3] = 0.0;
    // senceResult[2] = normalizeDistance(cellSignal.distance, senceRadius);
    // senceResult[3] = cellSignal.valid ? normalizeAngle(cellSignal.angle) : 0.0f;

    return senceResult;
}