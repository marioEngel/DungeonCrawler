#include "Dungeon.h"
#include "../../ECS/Coordinator.h"
#include <SDL3/SDL.h>
#include "../../Component/Comp_TileMap.h"

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
	Matrix renderMap = this->generateCave(500, 500, 0.5);
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