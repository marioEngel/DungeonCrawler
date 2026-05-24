#pragma once
#include "../../../Math/Vector2D.h"
#include "Geometry.h"

class GeomRectangle : public Geometry
{
public:
	GeomRectangle(float inWidth, float inHeight, float inRectWidht, float inRectHeight);
	GeomRectangle(float inWidth, float inHeight, float inRectWidht, float inRectHeight, eGeomType inGeomType);
	~GeomRectangle();

	void print_Stuff() const override;
	std::array<float, 2> return_para_Rect() const override;
	//SDL_Rect returnRectangle(Vector2D<float> position);

private:
	float mRectWidth;
	float mRectHeight;
};