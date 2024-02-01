#include "Rectangle.h"


GeomRectangle::GeomRectangle(float inWidth, float inHeight, float inRectWidht, float inRectHeight) :
	Geometry(inWidth, inHeight, eGeomType::RECTANGLE),
	mRectWidth{ inRectWidht },
	mRectHeight{ inRectHeight }
{
}

GeomRectangle::~GeomRectangle()
{
}

void GeomRectangle::print_Stuff() const 
{
	std::cout << "Rectangle\n";
}

std::array<float, 2> GeomRectangle::return_para_Rect() const
{
	return std::array<float, 2> {mRectWidth, mRectHeight};
}

//SDL_Rect GeomRectangle::returnRectangle(Vector2D<float> position)
//{
//	{
//		int tmpX = int(position[0] - mRectWidht);
//		int tmpY = int(position[1] - mRectHeight);
//		int tmpW = int(mRectWidht);
//		int tmpH = int(mRectHeight);
//		return SDL_Rect{ tmpX, tmpY, tmpW, tmpH };
//}