#include "GeneralMathStuff.h"
#include <iostream>
#include "Vector2D.h"

bool floatsEqual(float num1, float num2, float epsilon)
{
	return std::fabs(num1 - num2) < epsilon;
}

float convert_DegToRad(float angle)
{
	// transform angle into correct range 0 - 360 deg
	while (angle < 0.0f)
	{
		angle += 360.0f;
	}
	while (angle > 360.0f)
	{
		angle -= 360.0f;
	}

	return (angle / 180.0f) * M_PI;
}

float convert_RadToDeg(float angle)
{
	// transform angle into correct range 0 - 2pi
	while (angle < 0.0f)
	{
		angle += 2.0f * M_PI;
	}
	while (angle > 2.0f * M_PI)
	{
		angle -= 2.0f * M_PI;
	}

	return (angle / M_PI) * 180.0f;
}