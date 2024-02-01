#include "Geometry.h"

Geometry::Geometry(float inWidth, float inHeight, eGeomType inGeomType) :
	mHalveWidth{ inWidth },
	mHalveHeight{ inHeight },
	mGeomType{ inGeomType }
{
}

Geometry::~Geometry()
{
}

SDL_Rect Geometry::return_AABB(Vector2D<float> positionPoint) const
{
	//int tmpX = int(positionPoint[0] - mHalveWidth);
	//int tmpY = int(positionPoint[1] - mHalveHeight);
	int tmpX = int(positionPoint[0]);
	int tmpY = int(positionPoint[1]);
	int tmpW = 2 * int(mHalveWidth);
	int tmpH = 2 * int(mHalveHeight);
	return SDL_Rect{ tmpX, tmpY, tmpW, tmpH };
}

eGeomType Geometry::return_GeomType() const
{
	return mGeomType;
}

std::array<float, 2> Geometry::return_para_Rect() const
{
	std::cout << "You did something wrong Mario (return_para_Rect)\n"; 

	return std::array<float, 2> {0.0f, 0.0f};
}

std::array<float, 1> Geometry::return_para_Circ() const
{
	std::cout << "You did something wrong Mario (return_para_Circ)\n"; 

	return std::array<float, 1> {0.0f};
}

std::array<float, 2> Geometry::return_para_Sect() const
{
	std::cout << "You did something wrong Mario (return_para_Sect)\n";

	return std::array<float, 2> {0.0f, 0.0f};
}