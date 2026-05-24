#include "GeomLine.h"

GeomLine::GeomLine(float inWidth, float inHeight, float inRectWidht, float inRectHeight) :
	Geometry(inWidth, inHeight, eGeomType::LINE),
	mSlope{ inRectWidht },
	mOffset{ inRectHeight }
{
}
GeomLine::~GeomLine()
{
}

void GeomLine::print_Stuff() const
{
	std::cout << "Line\n";
}

std::array<float, 2> GeomLine::return_para_Line() const
{
	return std::array<float, 2> {mSlope, mOffset};
}