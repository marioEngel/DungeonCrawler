#pragma once
#include "../../../Math/Vector2D.h"
#include "Geometry.h"

class GeomCircularSector : public Geometry
{
public:
	GeomCircularSector(float inWidth, float inHeight, float inRadius, float inAngle);
	~GeomCircularSector();

	std::array<float, 2> return_para_Sect() const override;

private:
	float mRadius;
	float mAngleDeg;
};

