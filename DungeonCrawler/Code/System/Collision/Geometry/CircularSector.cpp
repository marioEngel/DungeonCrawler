#include "CircularSector.h"

GeomCircularSector::GeomCircularSector(float inWidth, float inHeight, float inRadius, float inAngle) :
	Geometry(inWidth, inHeight, eGeomType::CIRCULAR_SECTOR),
	mRadius{ inRadius },
	mAngleDeg{ inAngle }
{
}

GeomCircularSector::~GeomCircularSector()
{
}

std::array<float, 2> GeomCircularSector::return_para_Sect() const
{
	return std::array<float, 2> {mRadius, mAngleDeg};
}