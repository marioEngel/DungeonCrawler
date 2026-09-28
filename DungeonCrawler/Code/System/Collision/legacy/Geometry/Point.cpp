#include "Point.h"

GeomPoint::GeomPoint(float inWidth, float inHeight, float inPositionX, float inPositionY) :
	Geometry(inWidth, inHeight, eGeomType::POINT), 
	mPositionX{ inPositionX }, 
	mPositionY{ inPositionY }
{
}

GeomPoint::~GeomPoint()
{
}

void GeomPoint::print_Stuff() const
{
	std::cout << "Point\n";
}

std::array<float, 2> GeomPoint::return_para_Poin() const
{
	return std::array<float, 2> {mPositionX, mPositionY};
}