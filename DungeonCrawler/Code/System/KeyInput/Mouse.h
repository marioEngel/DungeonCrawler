#pragma once
#include "../../Math/Vector2D.h"

class Mouse
{
public:
	Mouse();
	~Mouse();

	void update();
	void printMousePosition();
	void printMousePosition_color();
	void click();

private:
	Vector2D<float> mMousePosition;
	int mMouseClickBuffer = 150;
	int mMouseLastClickTime = 0;
};

