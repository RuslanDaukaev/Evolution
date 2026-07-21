#pragma once
#include <random>

namespace Random {
    inline std::mt19937& getEngine() {
        thread_local std::random_device rd;
        thread_local std::mt19937 engine(rd());
        return engine;
    }
    inline float getFloat(float min, float max) {
        std::uniform_real_distribution<float> dist(min, max);
        return dist(getEngine());
    }
    inline int getInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(getEngine());
    }
}