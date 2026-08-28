#pragma once
#include "../Math/Vector2D.h"

/* Coordinate system
 *     x
 * y| ------->
 *  |
 *  |
 *  v
 *  x into right direction
 *  y into down direction
 */

// Move from tile to tile with some little pause
 struct MovementTile
 {
     float speed = 64.0f;

     int cooldown = 500;
     int pause = 250;

     Vector2D<float> direction{ 0,0 };

     int timeStart = 0;
     int timeEnd = 0;
     int timePause = 0;

     int tileSize = 32;
     Vector2D<float> positionStart {0, 0};
     Vector2D<float> positionEnd{ 0, 0 };
     
     bool onCooldown = false;
     bool onPause = false;
 };