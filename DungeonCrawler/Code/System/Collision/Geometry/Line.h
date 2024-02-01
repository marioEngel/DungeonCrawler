#pragma once
#include "../../../Math/Vector2D.h"

enum eLineStyle
{
	NORMAL = 0,			// something y = m*x + t
	HORIZONTAL = 2,		// something y = c
	VERTICAL = 4		// something x = c
};

class Line
{
public:
	Line();
	Line(Vector2D<float> startPoint, Vector2D<float> stoppPoint);
	~Line();

	// getter
	eLineStyle return_Linestyle() const;
	Vector2D<float> return_StartPoint() const;
	Vector2D<float> return_StoppPoint() const;
	float return_Slope_m() const;
	float return_Offset_t() const;

	void printData() const;

private:
	Vector2D<float> mStartPoint;
	Vector2D<float> mStoppPoint;
	eLineStyle mLinestyle;
	float mSlope_m;
	float mOffset_t;
};
