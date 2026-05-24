#pragma once
#include "Geometry.h"

class GeomLine : public Geometry
{
public:
	GeomLine(float inWidth, float inHeight, float inSlope, float mOffset);
	~GeomLine();

	void print_Stuff() const override;
	std::array<float, 2> return_para_Line() const override;

private:
	float mSlope;
	float mOffset;
};

