#include "Dungeon.h"
#include "../../ECS/Coordinator.h"
#include <SDL3/SDL.h>
#include "../../Component/Comp_TileMap.h"
#include "Chunk.h"

extern Coordinator gCoordinator;

void DungeonSystem::init()
{
	int tileMapEntity = gCoordinator.CreateEntity();
	std::vector<SDL_Texture*> emptyTexture{};
	std::vector<const char*> tmpTileTextures =
	{
		"Picture/TileNormal.png",
		"Picture/TileGround.png"
	};
	Matrix renderMap = this->generateCave(256, 256, 0.5);
	renderMap = transposeSquare(renderMap, 2);
	gCoordinator.AddComponent<TileMap>(tileMapEntity, TileMap{ renderMap, tmpTileTextures, emptyTexture });
}

Matrix DungeonSystem::generateCave(int width, int height, float fillProb, int steps)
{
	Matrix grid;
	grid.resize(height, std::vector<int>(width));
	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			grid[y][x] = ((float)rand() / RAND_MAX) < fillProb ? 1 : 0;
		}
	}

	for (size_t i = 0; i < steps; i++)
	{
		step(grid, height, width);
	}

	return grid;
}

void DungeonSystem::step(Matrix& mat, int height, int width)
{
	Matrix next = mat;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int n = countNeighbours(x, y, mat, height, width);
			if (mat[y][x] == 1)
			{
				next[y][x] = (n >= 4) ? 1 : 0;
			}
			else
			{
				next[y][x] = (n >= 5) ? 1 : 0;
			}
		}
	}
	mat = next;
}

int countNeighbours(int x, int y, Matrix& mat, int height, int width)
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
				if (mat[ny][nx] > 0)
				{
					count++;
				}
			}
		}
	}
	return count;
}

Matrix transposeSquare(Matrix& matrix, int multipyDimension)
{
	Matrix rtnMatrix;
	int newSize = matrix.size() * multipyDimension;
	rtnMatrix.resize(newSize, std::vector<int>(newSize));

	for (size_t y = 0; y < matrix.size(); y++)
	{
		for (size_t x = 0; x < matrix.size(); x++)
		{
			int newY = y * multipyDimension;
			int newX = x * multipyDimension;
			int currentMatrixValue = matrix[y][x];

			for (size_t sub_y = 0; sub_y < multipyDimension; sub_y++)
			{
				for (size_t sub_x = 0; sub_x < multipyDimension; sub_x++)
				{
					rtnMatrix[newY + sub_y][newX + sub_x] = currentMatrixValue;
				}
			}
		}
	}

	return rtnMatrix;
}

Matrix slice(Matrix& matrix, int squareDimension)
{
	Matrix rtnMatrix;
	rtnMatrix.resize(squareDimension, std::vector<int>(squareDimension));

	for (size_t y = 0; y < squareDimension; y++)
	{
		for (size_t x = 0; x < squareDimension; x++)
		{
			rtnMatrix[y][x] = matrix[y][x];
		}
	}

	return rtnMatrix;
}