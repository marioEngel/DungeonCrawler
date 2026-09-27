#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "../../../Math/Matrix.h"
#include "../../../Misc/RNG.h"
#include <set>
#include "Compass.h"
#include "ObjectType.h"

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
	void init(const FloorGenerationData& inputData);

	void generate(int squareSize);
	Matrix<int> getTileMap();
	Matrix<int> getObjectMap();
	void setStartEndPoint();

private:
	// ----- basic functions ---------
	void setTiles(SDL_Rect& room);
	void incrementCurrentRegion();
	void carveFloor(Vector2D<int> pos);
	bool canCarve(Vector2D<int> pos, Vector2D<int> dir);
	SDL_Rect chooseFarRoom(const SDL_Rect& startRoom);
	Vector2D<int> getRandomPointInRoom(const SDL_Rect& room);

	// ------ generate maze ---------------
	void rooms_add();
	void maze_generate();
	void maze_grow(Vector2D<int> start);
	void regions_connect();
	void junction_add(Vector2D<int> pos);
	void deadEnds_remove();

	FloorGenerationData mFloorGenerationData;
	int mCurrentRegion = -1;
	SDL_Rect mFloorSize;
	Matrix<int> mTileMap{ 1 };
	Matrix<int> mRegionMap{ 1 };
	Matrix<int> mObjectMap{ 1 };
	std::vector<SDL_Rect> mRoomVector;

	// ------- helper ------------------
	Compass mCompass{};
	RNG mRNG{ 0 };
	uint64_t mSeed;
};


// ---------------- helper funcitons -------------------------------
SDL_Rect addRoomBorder(const SDL_Rect& rect);
SDL_Rect addRoomBorder_doubled(const SDL_Rect& rect);
std::vector<Vector2D<int>> getPosListFromRect(const SDL_Rect& rect);
float roomDistance(const SDL_Rect& room_a, const SDL_Rect& room_b);