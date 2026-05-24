#include "Circle_Sector.h"

GeomCircle_Sector::GeomCircle_Sector(float inWidth, float inHeight, float inRadius, std::array<float, 2> inDirection, float inAngle) :
	GeomCircle(inWidth, inHeight, inRadius, eGeomType::CIRCLE_SECTOR),
	mDirection{ inDirection },
	mAngle{ inAngle }
{
}

GeomCircle_Sector::~GeomCircle_Sector()
{
}

std::array<float, 4> GeomCircle_Sector::return_para_CirS() const
{
	float tmpRadius = this->return_para_Circ()[0];
	return std::array<float, 4> {tmpRadius, mDirection[0], mDirection[1], mAngle };
}