#include "Dungeon.h"
#include "../../../ECS/Coordinator.h"
#include <SDL3/SDL.h>
#include "../../../Component/Comp_TileMap.h"
#include "Chunk.h"

extern Coordinator gCoordinator;

void Dungeon::init()
{
	int tileMapEntity = gCoordinator.CreateEntity();
	std::vector<SDL_Texture*> emptyTexture{};
	std::vector<const char*> tmpTileTextures =
	{
		"Picture/TileNormal.png",
		"Picture/TileGround.png"
	};
	Matrix<int> renderMap = this->generateCave(128, 128, 0.5);
	renderMap = transposeSquare(renderMap, 2);
	gCoordinator.AddComponent<TileMap>(tileMapEntity, TileMap{ tmpTileTextures, emptyTexture, renderMap });
}

Matrix<int> Dungeon::generateCave(int width, int height, float fillProb, int steps)
{
	Matrix<int> grid{ width, height };
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			grid(y, x) = ((float)rand() / RAND_MAX) < fillProb ? 1 : 0;
		}
	}

	for (size_t i = 0; i < steps; i++)
	{
		step(grid, height, width);
	}

	return grid;
}

void Dungeon::step(Matrix<int>& mat, int height, int width)
{
	Matrix<int> next = mat;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int n = countNeighbours(x, y, mat, height, width);
			if (mat(y, x) == 1)
			{
				next(y, x) = (n >= 4) ? 1 : 0;
			}
			else
			{
				next(y, x) = (n >= 5) ? 1 : 0;
			}
		}
	}
	mat = next;
}

int countNeighbours(int x, int y, Matrix<int>& mat, int height, int width)
{
	int count = 0;

	for (int dy = -1; dy <= 1; dy++)
	{
		for (int dx = -1; dx <= 1; dx++)
		{
			if (dx == 0 && dy == 0)
			{
				continue;
			}
			int nx = x + dx;
			int ny = y + dy;
			if (nx < 0 || ny < 0 || nx >= width || ny >= height)
			{
				count++;
			}
			else
			{
				if (mat(ny, nx) > 0)
				{
					count++;
				}
			}
		}
	}
	return count;
}

Matrix<int> transposeSquare(Matrix<int>& matrix, int multipyDimension)
{
	int newSize = matrix.cols() * multipyDimension;
	Matrix<int> rtnMatrix{ newSize,  };

	for (size_t y = 0; y < matrix.cols(); y++)
	{
		for (size_t x = 0; x < matrix.rows(); x++)
		{
			int newY = y * multipyDimension;
			int newX = x * multipyDimension;
			int currentMatrixValue = matrix(y, x);

			for (size_t sub_y = 0; sub_y < multipyDimension; sub_y++)
			{
				for (size_t sub_x = 0; sub_x < multipyDimension; sub_x++)
				{
					rtnMatrix(newY + sub_y, newX + sub_x) = currentMatrixValue;
				}
			}
		}
	}

	return rtnMatrix;
}

Matrix<int> slice(Matrix<int>& matrix, int squareDimension)
{
	Matrix<int> rtnMatrix{ squareDimension };

	for (size_t y = 0; y < squareDimension; y++)
	{
		for (size_t x = 0; x < squareDimension; x++)
		{
			rtnMatrix(y, x) = matrix(y, x);
		}
	}

	return rtnMatrix;
}