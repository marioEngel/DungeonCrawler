#pragma once
#include "../../ECS/System.h"
#include "../../Math/Matrix.h"
#include "Chunk.h"
#include <map>

class Dungeon
{
public:
	void init();

private:
	Matrix generateCave(int width, int height, float fillProb=0.45f, int steps=5);
	void step(Matrix& mat, int height, int width);
	std::map<std::pair<int, int>, Chunk> mChuncks;
};

int countNeighbours(int x, int y, Matrix& mat, int height, int width);
Matrix transposeSquare(Matrix& matrix, int multipyDimension = 2);

Matrix slice(Matrix& matrix, int squareDimension);