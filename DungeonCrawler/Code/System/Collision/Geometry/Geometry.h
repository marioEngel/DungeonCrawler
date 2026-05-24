#pragma once
#include "../../../Math/Vector2D.h"
#include <SDL3/SDL.h>

// powers of 2
enum eGeomType
{
	DEFAULT = 1,				// 2^0
	RECTANGLE = 2,				// 2^1
	RECTANGLE_ROTATET = 4,		// 2^2 
	CIRCLE = 8,					// 2^3
	CIRCLE_SECTOR = 16,			// 2^4
	POINT = 32,					// 2^5
	LINE = 64					// 2^6
};

class Geometry
{
public:
	Geometry(float inWidth, float inHeight, eGeomType inGeomType);
	~Geometry();
	virtual void print_Stuff() const = 0;
	eGeomType return_GeomType() const;
	SDL_FRect return_AABB(Vector2D<float> positionPoint) const;

	// virtual function for specific geometries maybe there are better solutions for this
	// default trash definition bottom
	virtual std::array<float, 2> return_para_Rect() const;
	virtual std::array<float, 3> return_para_RecR() const;
	virtual std::array<float, 1> return_para_Circ() const;
	virtual std::array<float, 4> return_para_CirS() const;
	virtual std::array<float, 2> return_para_Poin() const;
	virtual std::array<float, 2> return_para_Line() const;

private:
	//Vector2D<float> mOriginPoint;
	// checking if to geometries are close and worth checking collision
	float mHalveWidth;
	float mHalveHeight;
	eGeomType mGeomType;
};

