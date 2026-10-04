#include "Floor.h"
#include "../Enums/TileType.h"
#include "../Collision/CollisionFunc.h"
#include <algorithm>

void Floor::generate(int squareSize) {
    mMap_tileTypes = Matrix<int>(squareSize);
    mMap_regions = Matrix<int>(squareSize);
    mMap_objects = Matrix<int>(squareSize);
    mMap_collision = Matrix<int>(squareSize);

    matrixFillWithElement(mMap_tileTypes, eTileType::ROCK);
    matrixFillWithElement(mMap_regions, -1);

    mCurrentRegion = -1;

    mFloorSize.w = squareSize;
    mFloorSize.h = squareSize;

    rooms_add();
    maze_generate();
    regions_connect();
    deadEnds_remove();
    createCollisionMap();
}

void Floor::rooms_add() {
    for (size_t i = 0; i < mFloorGenerationData.numRoomTries; i++) {
        const int size = mRNG.range(1, 3 + mFloorGenerationData.roomExtraSize) * 2 + 1;
        const int rectangularity = mRNG.range(0, 1 + size / 2) * 2 + 1;
        int width = size;
        int height = size;
        if (mRNG.oneIn(2)) {
            width += rectangularity;
        } else {
            height += rectangularity;
        }

        const int x = mRNG.range((mFloorSize.w - width) / 2) * 2;
        const int y = mRNG.range((mFloorSize.h - height) / 2) * 2;

        SDL_Rect tmpRoom{.x = x, .y = y, .w = width, .h = height};
        SDL_Rect tmpRoomWithBorder = addRoomBorderN(tmpRoom, 1);
        SDL_Rect tmpRoomWithBorderAdditional = addRoomBorderN(tmpRoom, 2);

        if (tmpRoomWithBorder.x < 0 || tmpRoomWithBorder.y < 0 ||
            tmpRoomWithBorder.x + tmpRoomWithBorder.w > mFloorSize.w ||
            tmpRoomWithBorder.y + tmpRoomWithBorder.h > mFloorSize.h) {
            continue;
        }

        bool overlaps = false;
        for (const SDL_Rect &room: mRoomVector) {
            SDL_Rect roomWithBorderAdditional = addRoomBorderN(room, 2);

            overlaps = check_RectVsRect(tmpRoomWithBorderAdditional, roomWithBorderAdditional);
            if (overlaps) {
                break;
            }
        }

        if (!overlaps) {
            incrementCurrentRegion();
            setTiles(tmpRoom);
            mRoomVector.emplace_back(tmpRoom);
        }
    }
}

void Floor::maze_generate() {
    for (int row = 1; row < mFloorSize.h; row += 2) {
        for (int col = 1; col < mFloorSize.w; col += 2) {
            Vector2D<int> pos{row, col};
            if (mMap_tileTypes[pos] != eTileType::ROCK) {
                continue;
            }

            std::vector<Vector2D<int> > cells{};
            Vector2D<int> lastDir{0, 0};

            incrementCurrentRegion();
            carveFloor(pos);

            cells.push_back(pos);

            while (!cells.empty()) {
                Vector2D<int> cell = cells.back();

                std::vector<Vector2D<int> > unmadeCells{};
                for (const Vector2D<int> &dir: mCompass.directions) {
                    if (canCarve(cell, dir)) {
                        unmadeCells.push_back(dir);
                    }
                }

                if (!unmadeCells.empty()) {
                    Vector2D<int> dir;

                    if (std::find(unmadeCells.begin(), unmadeCells.end(), lastDir) != unmadeCells.end()
                        && mRNG.range(100) > mFloorGenerationData.windingPercent) {
                        dir = lastDir;
                    } else {
                        dir = unmadeCells[mRNG.rangeEx(static_cast<int>(unmadeCells.size()))];
                    }

                    const Vector2D<int> doorPos = cell + dir;
                    Vector2D<int> nextCell = cell + dir + dir;

                    carveFloor(doorPos);
                    carveFloor(nextCell);

                    cells.push_back(nextCell);
                    lastDir = dir;
                } else {
                    cells.pop_back();
                    lastDir = Vector2D<int>{0, 0};
                }
            }
        }
    }
}

void Floor::regions_connect() {
    std::vector<ConnectorInfo> connectorRegions{};
    for (int row = mFloorSize.y + 1; row < mFloorSize.y + mFloorSize.w - 1; row++) {
        for (int col = mFloorSize.x + 1; col < mFloorSize.x + mFloorSize.h - 1; col++) {
            Vector2D<int> pos{row, col};

            if (mMap_tileTypes[pos] != eTileType::WALL) {
                continue;
            }

            std::set<int> surroundingRegions{};
            for (const Vector2D<int> &direction: mCompass.directions) {
                int region = mMap_regions[pos + direction];
                if (region != -1) {
                    surroundingRegions.insert(region);
                }
            }

            if (surroundingRegions.size() < 2) {
                continue;
            }

            connectorRegions.push_back({pos, surroundingRegions});
        }
    }

    std::vector<int> merged;
    std::set<int> openRegions;
    for (int i = 0; i <= mCurrentRegion; i++) {
        merged.push_back(i);
        openRegions.insert(i);
    }

    while (openRegions.size() > 1) {
        ConnectorInfo chosenConnector = connectorRegions[mRNG.rangeEx(connectorRegions.size())];
        junction_add(chosenConnector.pos);

        std::set<int> mappedRegions{};
        for (const int region: chosenConnector.regions) {
            mappedRegions.insert(merged[region]);
        }

        const int dest = *mappedRegions.begin();
        std::vector<int> sources(std::next(mappedRegions.begin()), mappedRegions.end());

        for (size_t i = 0; i <= mCurrentRegion; i++) {
            if (std::find(sources.begin(), sources.end(), merged[i]) != sources.end()) {
                merged[i] = dest;
            }
        }
        for (int source: sources) {
            openRegions.erase(source);
        }
        connectorRegions.erase(
            std::remove_if(connectorRegions.begin(), connectorRegions.end(),
                           [&](ConnectorInfo &c) {
                               std::set<int> cMappedRegions{};
                               for (const int region: c.regions) {
                                   cMappedRegions.insert(merged[region]);
                               }

                               if (cMappedRegions.size() > 1) {
                                   return false;
                               }

                               if ((chosenConnector.pos - c.pos).calc_amountSquared() < 4) {
                                   return true;
                               }

                               if (mRNG.oneIn(mFloorGenerationData.extraConnectorChance)) {
                                   junction_add(c.pos);
                               }

                               return true;
                           }),
            connectorRegions.end()
        );
    }
}

void Floor::deadEnds_remove() {
    bool done = false;

    while (!done) {
        done = true;

        std::vector<Vector2D<int> > rectToPos = getPosListFromRect(
            SDL_Rect{mFloorSize.x + 1, mFloorSize.y + 1, mFloorSize.w - 2, mFloorSize.h - 2});
        for (const Vector2D<int> &pos: rectToPos) {
            if (mMap_tileTypes[pos] == eTileType::ROCK ||
                mMap_tileTypes[pos] == eTileType::WALL ||
                mMap_tileTypes[pos] == eTileType::DOOR) {
                continue;
            }

            int exits = 0;
            int roomNr = 0;
            for (const Vector2D<int> &dir: mCompass.directions) {
                if (mMap_tileTypes[pos + dir] == eTileType::FLOOR ||
                    mMap_tileTypes[pos + dir] == eTileType::DOOR) {
                    exits++;
                }
                if (mMap_tileTypes[pos + dir] == eTileType::WALL) {
                    roomNr++;
                }
            }

            if (exits != 1 || roomNr == 3) {
                continue;
            }


            done = false;
            mMap_tileTypes[pos] = eTileType::ROCK;
        }
    }
}
