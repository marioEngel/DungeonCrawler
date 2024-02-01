#include "Circle.h"

GeomCircle::GeomCircle(float inWidth, float inHeight, float inRadius) :
	Geometry(inWidth, inHeight, eGeomType::CIRCLE),
	mRadius{ inRadius }
{
}

GeomCircle::~GeomCircle()
{
}

void GeomCircle::print_Stuff() const
{
	std::cout << "Circle\n";
}

std::array<float, 1> GeomCircle::return_para_Circ() const
{
	return std::array<float, 1> {mRadius};
}
