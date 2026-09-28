#pragma once
#include "../../../Math/Vector2D.h"
#include "Geometry.h"

class GeomCircle : public Geometry
{
public:
	GeomCircle(float inWidth, float inHeight, float inRadius);
	GeomCircle(float inWidth, float inHeight, float inRadius, eGeomType inGeomType);
	~GeomCircle();

	void print_Stuff() const override;
	std::array<float, 1> return_para_Circ() const override;

private:
	float mRadius;
};