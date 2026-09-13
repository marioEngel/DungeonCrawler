#pragma once
#include <random>

class RNG
{
public:
	explicit RNG(uint64_t seed) : engine(seed) {};

	int range(int min, int max);
	int range(int max);
	int rangeEx(int maxExcludeded);
	float range(float max, float min = 0);

	bool oneIn(int n);

private:
	std::mt19937_64 engine;
};

