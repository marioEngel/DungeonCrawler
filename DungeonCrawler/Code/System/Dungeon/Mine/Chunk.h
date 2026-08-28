#pragma once
#include "../../../Math/Matrix.h"

struct Chunk {
	const int SIZE = 64;
	bool generated = false;
	Matrix<int> grid;
	Entity entity;
};