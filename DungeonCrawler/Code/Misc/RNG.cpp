#include "RNG.h"

int RNG::range(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(engine);
}

float RNG::range(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(engine);
}

bool RNG::oneIn(int n)
{
    std::uniform_int_distribution<int> dist(0, n - 1);
    return dist(engine) == 0;
}