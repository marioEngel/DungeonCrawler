#pragma once
#include <vector>
#include <SDL3/SDL.h>
#include "../../../Math/Matrix.h"
#include "../../../Misc/RNG.h"
#include <set>
#include "Compass.h"

//  source https://journal.stuffwithstuff.com/2014/12/21/rooms-and-mazes/

struct ConnectorInfo {
    Vector2D<int> pos;
    std::set<int> regions;
};

struct FloorGenerationData {
    int numRoomTries;
    int roomExtraSize;
    int windingPercent;
    int extraConnectorChance;
};

class Floor {
public:
    Floor();

    ~Floor();

    void init();

    void init(const FloorGenerationData &inputData);

    Matrix<int> getMap_tileTypes();

    Matrix<int> getMap_objects();

    Matrix<int> getMap_collision();

    void generate(int squareSize);

    void setStartEndPoint();

private:
    // ----- basic functions ---------
    void setTiles(SDL_Rect &room);

    void incrementCurrentRegion();

    void carveFloor(Vector2D<int> pos);

    bool canCarve(Vector2D<int> pos, Vector2D<int> dir);

    SDL_Rect chooseFarRoom(const SDL_Rect &startRoom);

    Vector2D<int> getRandomPointInRoom(const SDL_Rect &room);

    void junction_add(Vector2D<int> &pos);

    void createCollisionMap();

    // ------ generate maze Floor_generate ---------------
    void rooms_add();

    void maze_generate();

    void regions_connect();

    void deadEnds_remove();

    FloorGenerationData mFloorGenerationData;
    int mCurrentRegion = -1;
    SDL_Rect mFloorSize{0, 0, 0, 0};
    Matrix<int> mMap_tileTypes{1};
    Matrix<int> mMap_regions{1};
    Matrix<int> mMap_objects{1};
    Matrix<int> mMap_collision{1};
    std::vector<SDL_Rect> mRoomVector;

    // ------- helper ------------------
    Compass mCompass{};
    RNG mRNG{0};
    uint64_t mSeed;
};


// ---------------- helper funcitons  Floor_helper -------------------------------
SDL_Rect addRoomBorderN(const SDL_Rect &rect, const int &borderSize);

SDL_Rect shrinkRoomByN(const SDL_Rect &rect, int n = 1);

std::vector<Vector2D<int> > getPosListFromRect(const SDL_Rect &rect);

float rectDistance(const SDL_Rect &room_a, const SDL_Rect &room_b);
