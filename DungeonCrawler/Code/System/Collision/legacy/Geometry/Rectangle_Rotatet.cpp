#include "Rectangle_Rotatet.h"

GeomRectangle_Rotatet::GeomRectangle_Rotatet(float inWidth, float inHeight, float inRectWidth, float inRectHeight, float inAngle) :
	GeomRectangle(inWidth, inHeight, inRectHeight, inRectHeight, eGeomType::RECTANGLE_ROTATET),
	mAngle{ inAngle }
{
}
GeomRectangle_Rotatet::~GeomRectangle_Rotatet()
{
}

void GeomRectangle_Rotatet::print_Stuff() const
{
	std::cout << "Rectangle Rotatet\n";
}

std::array<float, 3> GeomRectangle_Rotatet::return_para_RecR() const
{
	std::array<float, 2> tmpRectVariables = this->return_para_Rect();
	return std::array<float, 3> {tmpRectVariables[0], tmpRectVariables[1], mAngle};
}