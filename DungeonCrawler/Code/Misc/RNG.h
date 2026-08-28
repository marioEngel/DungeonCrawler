#pragma once
#include <random>

class RNG
{
public:
	explicit RNG(uint64_t seed) : engine(seed) {};

	int range(int min, int max = 0);
	float range(float min, float max = 0);

	bool oneIn(int n);

private:
	std::mt19937_64 engine;
};

