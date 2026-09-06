#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "../../../Math/Matrix.h"
#include "../../../Misc/RNG.h"
#include <set>
#include "Compass.h"

//  source https://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/

struct ConnectorInfo
{
	Vector2D<int> pos;
	std::set<int> regions;
};

struct FloorGenerationData
{
	int numRoomTries;
	int roomExtraSize;
	int windingPercent;
	int extraConnectorChance;
};

class Floor
{
public:
	Floor();
	~Floor();

	void init();

	// ----- basic functions ---------
	void setTiles(SDL_Rect& room);
	void incrementCurrentRegion();
	void carveFloor(Vector2D<int> pos);
	bool canCarve(Vector2D<int> pos, Vector2D<int> dir);

	// ------ generate maze
	void generate();
	void rooms_add();
	void maze_generate();
	void maze_grow(Vector2D<int> start);
	void regions_connect();
	void junction_add(Vector2D<int> pos);
	void deadEnds_remove();
private:
	FloorGenerationData floorGenerationData;
	int currentRegion = -1;
	SDL_Rect floorSize;
	Matrix<int> floorTileMap{ 1 };
	Matrix<int> regionMap{ 1 };
	std::vector<SDL_Rect> roomVector;

	// ------- helper ------------------
	Compass mCompass{};
	RNG mRNG{ 0 };
	uint64_t mSeed;
};


// ---------------- helper funcitons -------------------------------
SDL_Rect addRoomBorder(const SDL_Rect& rect);
SDL_Rect addRoomBorder_doubled(const SDL_Rect& rect);
std::vector<Vector2D<int>> getPosListFromRect(const SDL_Rect& rect);