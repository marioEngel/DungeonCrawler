#pragma once
#include <SDL3/SDL.h>
#include "../../Math/Vector2D.h"

// in CollisionFunc_check.cpp
// | | | | |
// v v v v v

// simple collision test for geometries
bool check_RectVsRect(const SDL_FRect& rect1, const SDL_FRect& rect2);
bool check_RectVsRect(const SDL_Rect& rect1, const SDL_Rect& rect2);
bool check_RectVsPoint(const SDL_FRect& rect, const Vector2D<float>& positon);

// in SDL
// SDL_HasRectIntersection
// SDL_GetRectIntersection
// SDL_PointInRect / SDL_PointInRectFloat
// SDL_GetRectAndLineIntersection
