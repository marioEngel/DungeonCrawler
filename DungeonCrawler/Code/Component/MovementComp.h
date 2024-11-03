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


//struct Movement
//{
//	int coldown = 500; // in ms
//	int pause = 250;
//	int startTime = 0;
//	int endTime = 0;
//	int pauseTime = 0;
//	bool onColdown = false;
//	bool onPause = false;
//	float speed = 64.0f;
//	int direction = 0; /* 1: Up 
//						  2: Down
//						  3: Left
//						  4: Right
//					   */
//};

struct Movement
{
	float speed = 128.0f;
	Vector2D<float> direction{ 0, 0 };
	// currently if true: shoult never move
	bool isStationary = false;
};