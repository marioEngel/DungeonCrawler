#pragma once
#include "../../ECS/System.h"
#include "../../Math/Matrix.h"

class DungeonSystem 
{
public:
	void init();
	Matrix generateCave(int width, int height, float fillProb=0.45f, int steps=5);
	void step(Matrix& mat, int height, int width);
};

int countNeighbours(int x, int y, Matrix& mat, int height, int width);