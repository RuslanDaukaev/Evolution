#pragma once

#include <vector>
#include <random>
#include <memory>
#include "Random.h"
#include <iostream> 
#include <algorithm>
#include <cmath>

constexpr float MUTATION_CHANCE = 0.1;
constexpr float MUTATION_POWER = 0.1;

constexpr int INPUT_NEURO_COUNT = 4;
constexpr int HIDDEN_NEURO_COUNT = 6;
constexpr int OUTPUT_NEURO_COUNT = 2;

constexpr int GENOME_SIZE = (INPUT_NEURO_COUNT * HIDDEN_NEURO_COUNT) + 
HIDDEN_NEURO_COUNT + (HIDDEN_NEURO_COUNT * OUTPUT_NEURO_COUNT) + OUTPUT_NEURO_COUNT;

class Genome {
public:
    std::vector<float> genes;

    Genome() {
        genes.resize(GENOME_SIZE, 0.0f);
        randomGenome();
    }

    Genome(std::vector<float> parentGenes) {
        genes = parentGenes;
        mutate();
    }

    void mutate() {
        if (Random::getFloat(0.0, 1.0) <= MUTATION_CHANCE) {
            int temp = Random::getInt(0, GENOME_SIZE - 1);
            genes[temp] += Random::getFloat(-MUTATION_POWER, MUTATION_POWER);
            std::clamp(static_cast<float>(genes[temp]), -1.0f, 1.0f);
        }
    }

    void randomGenome() {
        for (size_t i = 0; i < GENOME_SIZE; i++) {
            genes[i] = Random::getFloat(-1.0, 1.0);
        }
    }
};