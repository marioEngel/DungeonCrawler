#include "Room.h"

Room::Room(const int inX, const int inY, const int inWidth, const int inHeight) {
    geom = SDL_Rect{.x = inX, .y = inY, .w = inWidth, .h = inHeight};
}

Room::~Room() {
}

void Room::setTileType() {
    tileType = eTileType::FLOOR;
}
