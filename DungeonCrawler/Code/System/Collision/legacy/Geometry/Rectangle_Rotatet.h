#pragma once
#include "Rectangle.h"

class GeomRectangle_Rotatet : public GeomRectangle
{
public:
	GeomRectangle_Rotatet(float inWidth, float inHeight, float inRectWidth, float inRectHeight, float inAngle);
	~GeomRectangle_Rotatet();

	void print_Stuff() const override;
	std::array<float, 3> return_para_RecR() const override;

private:
	float mAngle;
};

