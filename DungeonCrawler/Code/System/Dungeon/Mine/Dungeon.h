#pragma once
#include "../../../ECS/System.h"
#include "../../../Math/Matrix.h"
#include "Chunk.h"
#include <map>

class Dungeon
{
public:
	void init();

private:
	Matrix<int> generateCave(int width, int height, float fillProb=0.45f, int steps=5);
	void step(Matrix<int>& mat, int height, int width);
	std::map<std::pair<int, int>, Chunk> mChuncks;
};

int countNeighbours(int x, int y, Matrix<int>& mat, int height, int width);
Matrix<int> transposeSquare(Matrix<int>& matrix, int multipyDimension = 2);

Matrix<int> slice(Matrix<int>& matrix, int squareDimension);