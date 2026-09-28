#pragma once
#include "Geometry.h"

class GeomPoint : public Geometry
{
public:
	GeomPoint(float inWidth, float inHeight, float inPositionX, float inPositionY);
	~GeomPoint();

	void print_Stuff() const override;
	std::array<float, 2> return_para_Poin() const override;

private:
	float mPositionX;
	float mPositionY;
};

