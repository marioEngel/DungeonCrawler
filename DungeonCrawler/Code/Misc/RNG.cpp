#include "RNG.h"

int RNG::range(const int min, const int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

int RNG::range(const int max) {
    std::uniform_int_distribution<int> dist(0, max);
    return dist(engine);
}

int RNG::rangeEx(const int maxExcluded) {
    std::uniform_int_distribution<int> dist(0, maxExcluded - 1);
    return dist(engine);
}

float RNG::range(const float min, const float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(engine);
}

bool RNG::oneIn(const int n) {
    std::uniform_int_distribution<int> dist(0, n - 1);
    return dist(engine) == 0;
}
