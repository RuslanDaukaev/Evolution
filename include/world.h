#pragma once

#include <vector>
#include <memory>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "Random.h"
#include "Entity.h"
#include "Cell.h"
#include "Plant.h"
#include "raymath.h"
#include "WorldSettings.h"

class World {
private:
    int sectionX;
    int sectionY;
    std::vector<std::vector<Entity*>> sections;

    

    int getSectionIndex(int x, int y) const {
        return y * sectionX + x;
    }

public:

int countCell = 0;

    World() {
        sectionX = std::ceil(WORLD_WIDTH / SECTION_SIZE);
        sectionY = std::ceil(WORLD_HEIGHT / SECTION_SIZE);
        sections.resize(sectionX * sectionY);
    }

    std::vector<std::unique_ptr<Entity>> entities;

    std::vector<Entity*> getNearbyEntities(Vector2 pos) const {
        std::vector<Entity*> nearby;
        
        int secX = std::clamp(static_cast<int>(pos.x / SECTION_SIZE), 0, sectionX - 1);
        int secY = std::clamp(static_cast<int>(pos.y / SECTION_SIZE), 0, sectionY - 1);

        for (int y = secY - 1; y <= secY + 1; ++y) {
            for (int x = secX - 1; x <= secX + 1; ++x) {
                if (x >= 0 && x < sectionX && y >= 0 && y < sectionY) {
                    for (Entity* e : sections[getSectionIndex(x, y)]) {
                        nearby.push_back(e);
                    }
                }
            }
        }
        return nearby;
    }

    bool isCellPositionFree(Vector2 pos, float radius, Entity* ignore = nullptr) const {
        for (const auto& entity : entities) {
            if (entity->type != Type::Cell) {
                continue;
            }
            Entity* other = entity.get();
            if (other == ignore) {
                continue;
            }

            const float minDist = radius + other->radius;
            if (Vector2DistanceSqr(pos, other->pos) < minDist * minDist) {
                return false;
            }
        }
        return true;
    }

    void resolveCellCollision(Entity* cell) {
        if (cell->type != Type::Cell) {
            return;
        }

        std::vector<Entity*> nearby = getNearbyEntities(cell->pos);
        for (Entity* other : nearby) {
            if (other == cell || other->type != Type::Cell) {
                continue;
            }

            Vector2 delta = Vector2Subtract(cell->pos, other->pos);
            float distSq = delta.x * delta.x + delta.y * delta.y;
            const float minDist = cell->radius + other->radius;

            if (distSq >= minDist * minDist) {
                continue;
            }

            float dist = std::sqrt(distSq);
            Vector2 normal;
            if (dist < 1e-5f) {
                normal = {1.0f, 0.0f};
                dist = 1e-5f;
            } else {
                normal = Vector2Scale(delta, 1.0f / dist);
            }

            const float overlap = minDist - dist;
            cell->pos = Vector2Add(cell->pos, Vector2Scale(normal, overlap));
        }

        cell->pos.x = std::clamp(cell->pos.x, cell->radius, WORLD_WIDTH - cell->radius);
        cell->pos.y = std::clamp(cell->pos.y, cell->radius, WORLD_HEIGHT - cell->radius);
    }

    void spawnCell(Vector2 pos) {
        if (isCellPositionFree(pos, 50.0f)) {
            entities.push_back(std::make_unique<Cell>(pos));
        }
    }

    void spawnCell(Vector2 pos, Cell& cell) {
        const float radius = cell.radius;
        if (isCellPositionFree(pos, radius)) {
            entities.push_back(std::make_unique<Cell>(pos, cell.genome.genes));
            return;
        }

        for (int i = 0; i < 8; ++i) {
            const float angle = static_cast<float>(i) * (2.0f * PI / 8.0f);
            const float offset = radius * 2.0f * 1.05f;
            Vector2 tryPos = {
                pos.x + std::cos(angle) * offset,
                pos.y + std::sin(angle) * offset
            };
            if (isCellPositionFree(tryPos, radius)) {
                entities.push_back(std::make_unique<Cell>(tryPos, cell.genome.genes));
                return;
            }
        }
    }

    void removeEntity(Entity* target) {
        entities.erase(
            std::remove_if(entities.begin(), entities.end(),
                [target](const std::unique_ptr<Entity>& e) {
                    return e.get() == target;
                }),
            entities.end()
        );
    }

    void spawnPlant(Vector2 pos) {
        if (isEmpty(pos)) {
            entities.push_back(std::make_unique<Plant>(pos));
        }
    }

    bool isEmpty(Vector2 pos) {
        return isCellPositionFree(pos, 50.0f);
    }

    void drawEntities() {
        for (size_t i = 0; i < entities.size(); i++) {
            DrawCircleV(entities[i]->pos, entities[i]->radius, entities[i]->color);
        }
    }

    void updateEntities() {

        countCell = 0;

        for (auto& section : sections) {
            section.clear(); 
        }

        for (const auto& entity : entities) {
            int secX = std::clamp(static_cast<int>(entity->pos.x / SECTION_SIZE), 0, sectionX - 1);
            int secY = std::clamp(static_cast<int>(entity->pos.y / SECTION_SIZE), 0, sectionY - 1);
            sections[getSectionIndex(secX, secY)].push_back(entity.get());
        }

        for (size_t i = 0; i < entities.size(); i++) {
            if (entities[i]->type == Type::Cell) countCell++;
            entities[i]->update(*this);
        }
    }

};