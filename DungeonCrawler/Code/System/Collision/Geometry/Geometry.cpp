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

SDL_FRect Geometry::return_AABB(Vector2D<float> positionPoint) const
{
	//int tmpX = int(positionPoint[0] - mHalveWidth);
	//int tmpY = int(positionPoint[1] - mHalveHeight);
	float tmpX = positionPoint[0];
	float tmpY = positionPoint[1];
	float tmpW = 2 * mHalveWidth;
	float tmpH = 2 * mHalveHeight;
	return SDL_FRect{ tmpX, tmpY, tmpW, tmpH };
}

Vector2D<float> Geometry::return_AABB_Center(Vector2D<float> positionPoint) const
{
	return Vector2D<float> {positionPoint[0] + mHalveWidth, positionPoint[1] + mHalveHeight};
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

std::array<float, 3> Geometry::return_para_RecR() const
{
	std::cout << "You did something wrong Mario (return_para_RecR)\n";

	return std::array<float, 3> {0.0f, 0.0f, 0.0f};
}

std::array<float, 1> Geometry::return_para_Circ() const
{
	std::cout << "You did something wrong Mario (return_para_Circ)\n"; 

	return std::array<float, 1> {0.0f};
}

std::array<float, 4> Geometry::return_para_CirS() const
{
	std::cout << "You did something wrong Mario (return_para_CirS)\n";

	return std::array<float, 4> {0.0f, 0.0f, 0.0f, 0.0f};
}

std::array<float, 2> Geometry::return_para_Poin() const

{
	std::cout << "You did something wrong Mario (return_para_Poin)\n";

	return std::array<float, 2> {0.0f, 0.0f};
}

std::array<float, 2> Geometry::return_para_Line() const
{
	std::cout << "You did something wrong Mario (return_para_Line)\n";

	return std::array<float, 2> {0.0f, 0.0f};
}

