#pragma once
#include "../../../Math/Vector2D.h"
#include "Circle.h"

class GeomCircle_Sector : public GeomCircle
{
public:
	GeomCircle_Sector(float inWidth, float inHeight, float inRadius, std::array<float, 2> inDirection, float inAngle);
	~GeomCircle_Sector();

	std::array<float, 4> return_para_CirS() const override;

private:
	std::array<float, 2> mDirection;
	float mAngle;
};

