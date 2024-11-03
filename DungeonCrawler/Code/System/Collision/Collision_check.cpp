#include "Collision.h"

bool check_RectVsRect(SDL_Rect& rect1, SDL_Rect& rect2)
{
	bool rtnBool = false;

	if ((rect1.x + rect1.w) > rect2.x &&
		rect1.x < (rect2.x + rect2.w) &&
		(rect1.y + rect1.h) > rect2.y &&
		rect1.y < (rect2.y + rect2.h))
	{
		rtnBool = true;
	}

	return rtnBool;
}

bool check_RectVsPoint(SDL_Rect& rect, Vector2D<float>& vec)
{
	bool rtnBool = false;

	if (int(vec[0]) >= rect.x &&
		int(vec[0]) <= (rect.x + rect.w) &&
		int(vec[1]) >= rect.y &&
		int(vec[1]) <= (rect.y + rect.h))
	{
		rtnBool = true;
	}

	return rtnBool;
}

bool check_RectVsCirc(SDL_Rect& rect, float& circ_radius, Vector2D<float> pos)
{
	bool rtnBool = false;
	std::vector<Vector2D<float>> rtnCrossPoints{};

	Line topLine = create_Line(eRectangleSide::TOP, rect);
	Line botLine = create_Line(eRectangleSide::BOT, rect);
	Line leftLine = create_Line(eRectangleSide::LEFT, rect);
	Line rightLine = create_Line(eRectangleSide::RIGHT, rect);

	if (check_determinante(circ_radius, pos, topLine))
	{
		std::vector<Vector2D<float>> tmpVec = crossPoints_CircVsLine(circ_radius, pos, topLine);
		for (size_t i = 0; i < tmpVec.size(); i++)
		{
			rtnCrossPoints.push_back(tmpVec[i]);
		}
	}
	if (check_determinante(circ_radius, pos, botLine))
	{
		std::vector<Vector2D<float>> tmpVec = crossPoints_CircVsLine(circ_radius, pos, botLine);
		for (size_t i = 0; i < tmpVec.size(); i++)
		{
			rtnCrossPoints.push_back(tmpVec[i]);
		}
	}
	if (check_determinante(circ_radius, pos, leftLine))
	{
		std::vector<Vector2D<float>> tmpVec = crossPoints_CircVsLine(circ_radius, pos, leftLine);
		for (size_t i = 0; i < tmpVec.size(); i++)
		{
			rtnCrossPoints.push_back(tmpVec[i]);
		}
	}
	if (check_determinante(circ_radius, pos, rightLine))
	{
		std::vector<Vector2D<float>> tmpVec = crossPoints_CircVsLine(circ_radius, pos, rightLine);
		for (size_t i = 0; i < tmpVec.size(); i++)
		{
			rtnCrossPoints.push_back(tmpVec[i]);
		}
	}

	for (size_t i = 0; i < rtnCrossPoints.size(); i++)
	{
		if (check_PointVsRectEdge(rect, rtnCrossPoints[i]))
		{
			rtnBool = true;
		}
	}

	return rtnBool;
}

bool check_CircleVsCircle(float& radius1, Vector2D<float>& position1, float& radius2, Vector2D<float>& position2)
{
	bool rtnBool = false;

	if ((position2 - position1).calc_amountSquared() < ((radius1 + radius2) * (radius1 + radius2)))
	{
		rtnBool = true;
	}

	return rtnBool;
}

bool check_PointVsRectEdge(SDL_Rect& rect, Vector2D<float>& point)
{
	bool rtnBool = false;

	float leftSide = float(rect.x);
	float rightSide = float(rect.x + rect.w);
	float topSide = float(rect.y + rect.h);
	float botSide = float(rect.y);

	if ((point[0] > leftSide && point[0] < rightSide) || floatsEqual(point[0], leftSide) || floatsEqual(point[0], rightSide))
	{
		if ((point[1] > botSide && point[1] < topSide) || floatsEqual(point[1], botSide) || floatsEqual(point[1], topSide))
		{
			rtnBool = true;
		}
	}
	return rtnBool;
}

// more advanced collision test for geometries
bool check_Geometry_AABB(Geometry& geom1, Vector2D<float>& pos1, Geometry& geom2, Vector2D<float>& pos2)
{
	SDL_Rect geom1_AABB = geom1.return_AABB(pos1);
	SDL_Rect geom2_AABB = geom2.return_AABB(pos2);

	return check_RectVsRect(geom1_AABB, geom2_AABB);
}

bool check_determinante(float& circ_radius, Vector2D<float>& pos, Line& line)
{
	// solving Mitternachtsformel: a*x^2 + b*x + c = 0
	// x_1/2 = ( -b +/- sqrt(b^2 - 4*a*C) )/(2*a) 

	float par_a{};
	float par_b{};
	float par_c{};
	float determinante{};
	bool rtnBool = false;

	switch (line.return_Linestyle())
	{
	case eLineStyle::HORIZONTAL:
		par_a = 1.0f;
		par_b = -2.0f * pos[0];
		par_c = pos[0] * pos[0] + pos[1] * pos[1] - circ_radius * circ_radius +
			line.return_Offset_t() * line.return_Offset_t() - 2.0f * line.return_Offset_t() * pos[1];

		determinante = par_b * par_b - 4.0f * par_a * par_c;

		if (determinante < 0.0f)
		{
			rtnBool = false;
		}
		else
		{
			rtnBool = true;
		}
		break;
	case eLineStyle::VERTICAL:
		par_a = 1.0f;
		par_b = -2.0f * pos[1];
		par_c = pos[0] * pos[0] + pos[1] * pos[1] - circ_radius * circ_radius +
			line.return_StartPoint()[0] * line.return_StartPoint()[0] - 2.0f * line.return_StartPoint()[0] * pos[0];

		determinante = par_b * par_b - 4.0f * par_a * par_c;

		if (determinante < 0.0f)
		{
			rtnBool = false;
		}
		else
		{
			rtnBool = true;
		}
		break;
	case eLineStyle::NORMAL:
		par_a = (1.0f + line.return_Slope_m() * line.return_Slope_m());
		par_b = 2.0f * (line.return_Slope_m() * line.return_Offset_t() - pos[0] - line.return_Slope_m() * pos[1]);
		par_c = (pos[0] * pos[0] + pos[1] * pos[1] - circ_radius * circ_radius
			+ line.return_Offset_t() * line.return_Offset_t() - 2.0f * line.return_Offset_t() * pos[1]);

		determinante = par_b * par_b - 4.0f * par_a * par_c;

		if (determinante < 0.0f)
		{
			rtnBool = false;
		}
		else
		{
			rtnBool = true;
		}
		break;
	default:
		break;
	}

	return rtnBool;
}

// return true if point1 is closer to the referencePoint than point2 else false
bool check_firstPointCloser(Vector2D<float>& point1, Vector2D<float>& point2, Vector2D<float>& referencePoint)
{
	float distance1 = (point1 - referencePoint).calc_amountSquared();
	float distance2 = (point2 - referencePoint).calc_amountSquared();

	if (distance1 < distance2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

CollisionDataSub check_Geometry_arbitrary(
	Geometry& geom1, Vector2D<float>& pos1, Movement& mov1,
	Geometry& geom2, Vector2D<float>& pos2, Movement& mov2
)
{
	if (geom1.return_GeomType() == geom2.return_GeomType())
	{
		switch (geom1.return_GeomType())
		{
		case eGeomType::RECTANGLE:
			// one object is stationary 
			if (mov1.isStationary || mov2.isStationary)
			{
				// move object which is not stationary
				if (mov1.isStationary)
				{
					std::array<float, 2> rect1 = geom1.return_para_Rect();
					std::array<float, 2> rect2 = geom2.return_para_Rect();

					SDL_Rect collRect1 = create_Rect(pos1, rect1);
					SDL_Rect collRect2 = create_Rect(pos2, rect2);

					return calc_RectVsRect_distVec(collRect2, collRect1, mov2.direction);
				}
				else
				{
					std::array<float, 2> rect1 = geom1.return_para_Rect();
					std::array<float, 2> rect2 = geom2.return_para_Rect();

					SDL_Rect collRect1 = create_Rect(pos1, rect1);
					SDL_Rect collRect2 = create_Rect(pos2, rect2);

					return calc_RectVsRect_distVec(collRect1, collRect2, mov1.direction);
				}
			}
			// both objects are moving
			// more difficult
			else
			{
				std::cout << "One object not stationary (Collision_check.cpp)\n";
				return CollisionDataSub{ Vector2D<float> {}, false };
			}
			break;
		case eGeomType::CIRCLE:
			// one object is stationary 
			if (mov1.isStationary || mov2.isStationary)
			{
				if (mov2.isStationary)
				{
					std::array<float, 1> circ1 = geom1.return_para_Circ();
					std::array<float, 1> circ2 = geom2.return_para_Circ();

					SDL_Rect collRect1_2 = geom1.return_AABB(pos1);
					SDL_Rect collRect2_2 = geom2.return_AABB(pos2);

					return calc_CircVsCirc_distVec(circ1[0], collRect1_2, circ2[0], collRect2_2);
				}
				else
				{
					std::array<float, 1> circ1 = geom1.return_para_Circ();
					std::array<float, 1> circ2 = geom2.return_para_Circ();

					SDL_Rect collRect1_2 = geom1.return_AABB(pos1);
					SDL_Rect collRect2_2 = geom2.return_AABB(pos2);

					return calc_CircVsCirc_distVec(circ2[0], collRect2_2, circ1[0], collRect1_2);
				}
			}
			// both objects are moving
			// more difficult	
			else
			{
				std::cout << "One object not stationary (Collision_check.cpp)\n";
				return CollisionDataSub{ Vector2D<float> {}, false };
			}
			break;
		default:
			std::cout << "somethings wrong\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
	}
	else
	{
		switch (geom1.return_GeomType() + geom2.return_GeomType())
		{
			// Rectangle vs Circle 
		case 6:
		{
			std::array<float, 1> circlePara;
			std::array<float, 2> rectanglePara;
			SDL_Rect circAABB{};
			SDL_Rect collRect{};
			if (geom1.return_GeomType() == eGeomType::RECTANGLE)
			{
				rectanglePara = geom1.return_para_Rect();
				collRect = create_Rect(pos1, rectanglePara);
				circlePara = geom2.return_para_Circ();
				circAABB = geom2.return_AABB(pos2);

				return calc_RectVsCirc_distVec(collRect, circlePara[0], circAABB, true, mov2.direction);
			}
			else
			{
				rectanglePara = geom2.return_para_Rect();
				collRect = create_Rect(pos2, rectanglePara);
				circlePara = geom1.return_para_Circ();
				circAABB = geom1.return_AABB(pos1);

				return calc_RectVsCirc_distVec(collRect, circlePara[0], circAABB, false, mov1.direction);
			}
			break;
		}
		// Rectangle vs Circular Section 
		case 10:
			break;
			// Circle vs Circular Section
		case 12:
			break;
		default:
			std::cout << "somethings wrong (check_Geometry_arbitrary)\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
	}
}