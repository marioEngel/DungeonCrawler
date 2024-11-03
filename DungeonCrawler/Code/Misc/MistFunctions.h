#pragma once
#include "../Math/Vector2D.h"
#include <SDL.h>
#include "../Game.h"
#include "../Component/TextureComp.h"
#include "../Component/TextureLightComp.h"
#include "../Component/PositionComp.h"

void printMousePosition();

SDL_Rect rtnScreenRect();

Vector2D<float> rtnCenter(Position& position, Texture& texture);
Vector2D<float> rtnCenter(Position& position, TextureLight& texture);