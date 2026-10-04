#include "Floor.h"

SDL_Rect addRoomBorderN(const SDL_Rect &rect, const int &borderSize) {
    return SDL_Rect{
        .x = rect.x - borderSize,
        .y = rect.y - borderSize,
        .w = rect.w + borderSize * 2,
        .h = rect.h + borderSize * 2
    };
}

SDL_Rect shrinkRoomByN(const SDL_Rect &rect, int n) {
    return SDL_Rect{
        .x = rect.x + n,
        .y = rect.y + n,
        .w = rect.w - n * 2,
        .h = rect.h - n * 2
    };
}

std::vector<Vector2D<int> > getPosListFromRect(const SDL_Rect &rect) {
    std::vector<Vector2D<int> > rtnList{};

    for (int row = rect.y; row < rect.y + rect.h; row++) {
        for (int col = rect.x; col < rect.x + rect.w; col++) {
            rtnList.emplace_back(row, col);
        }
    }

    return rtnList;
}

float rectDistance(const SDL_Rect &room_a, const SDL_Rect &room_b) {
    const float centerAx = static_cast<float>(room_a.x) + static_cast<float>(room_a.w) / 2.0f;
    const float centerAy = static_cast<float>(room_a.y) + static_cast<float>(room_a.h) / 2.0f;
    const float centerBx = static_cast<float>(room_b.x) + static_cast<float>(room_b.w) / 2.0f;
    const float centerBy = static_cast<float>(room_b.y) + static_cast<float>(room_b.h) / 2.0f;

    const float dx = centerBx - centerAx;
    const float dy = centerBy - centerAy;
    return std::sqrt(dx * dx + dy * dy);
}
