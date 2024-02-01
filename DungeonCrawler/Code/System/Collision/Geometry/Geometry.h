#pragma once
#include "../../../Math/Vector2D.h"
#include <SDL.h>

// powers of 2
enum eGeomType
{
	BASE = 1,				// 2^0
	RECTANGLE = 2,			// 2^1
	CIRCLE = 4,				// 2^2
	CIRCULAR_SECTOR = 8		// 2^3
};

class Geometry
{
public:
	Geometry(float inWidth, float inHeight, eGeomType inGeomType);
	~Geometry();
	virtual void print_Stuff() const = 0;
	eGeomType return_GeomType() const;
	SDL_Rect return_AABB(Vector2D<float> positionPoint) const;

	// virtual function for specific geometries maybe there are better solutions for this
	// default trash definition bottom
	virtual std::array<float, 2> return_para_Rect() const;
	virtual std::array<float, 1> return_para_Circ() const;
	virtual std::array<float, 2> return_para_Sect() const;

private:
	//Vector2D<float> mOriginPoint;
	// checking if to geometries are close and worth checking collision
	float mHalveWidth;
	float mHalveHeight;
	eGeomType mGeomType;
};

