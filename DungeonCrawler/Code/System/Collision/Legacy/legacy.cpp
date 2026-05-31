#include "legacy.h"

Line create_Line(eRectangleSide side, SDL_FRect& rect)
{
	// y-axis inverted
	Vector2D<float> corner_topLeft{ rect.x, rect.y };
	Vector2D<float> corner_topRight{ rect.x + rect.w, rect.y };
	Vector2D<float> corner_botLeft{ rect.x, rect.y + rect.h };
	Vector2D<float> corner_botRight{ rect.x + rect.w, rect.y + rect.h };


	switch (side)
	{
	case TOP:
		return Line{ corner_topLeft, corner_topRight };
		break;
	case BOT:
		return Line{ corner_botLeft, corner_botRight };
		break;
	case LEFT:
		return Line{ corner_botLeft, corner_topLeft };
		break;
	case RIGHT:
		return Line{ corner_botRight, corner_topRight };
		break;
	default:
		std::cout << "something wrong Mario\n";
		return Line{ Vector2D<float>{}, Vector2D<float>{} };
		break;
	}
}

eRectangleSide side_RectVsLine(SDL_FRect& rect, Line& line, Vector2D<float>& circleCentre)
{
	std::vector<Vector2D<float>> crossPoints{};
	std::vector<Vector2D<float>> lessCrossPoints{};
	int tmpFlag1{ 0 };
	int tmpFlag2{ 0 };
	bool switcher{ true };

	Line topLine = create_Line(eRectangleSide::TOP, rect);
	Line botLine = create_Line(eRectangleSide::BOT, rect);
	Line leftLine = create_Line(eRectangleSide::LEFT, rect);
	Line rightLine = create_Line(eRectangleSide::RIGHT, rect);

	crossPoints.push_back(crossPoint_LineVsLine(topLine, line));
	crossPoints.push_back(crossPoint_LineVsLine(botLine, line));
	crossPoints.push_back(crossPoint_LineVsLine(leftLine, line));
	crossPoints.push_back(crossPoint_LineVsLine(rightLine, line));

	for (size_t i = 0; i < 4; i++)
	{
		if (check_RectVsPoint(rect, crossPoints[i]))
		{
			lessCrossPoints.push_back(crossPoints[i]);
			if (switcher)
			{
				tmpFlag1 = i;
				switcher = false;
			}
			else
			{
				tmpFlag2 = i;
			}
		}
	}

	if (lessCrossPoints.size() == 2)
	{
		if (check_firstPointCloser(lessCrossPoints[0], lessCrossPoints[1], circleCentre))
		{
			return convert_int_eRectangleSide(tmpFlag1);
		}
		else
		{
			return convert_int_eRectangleSide(tmpFlag2);
		}
	}
	else
	{
		int tmpTmpFlag{};
		if (check_firstPointCloser(lessCrossPoints[0], lessCrossPoints[1], circleCentre))
		{
			tmpTmpFlag = 0;
			return convert_int_eRectangleSide(tmpTmpFlag);
		}
		else
		{
			tmpTmpFlag = 1;
			return convert_int_eRectangleSide(tmpTmpFlag);
		}
	}
}

// input 0-3
eRectangleSide convert_int_eRectangleSide(int& input)
{
	switch (input)
	{
	case 0:
		return eRectangleSide::TOP;
		break;
	case 1:
		return eRectangleSide::BOT;
		break;
	case 2:
		return eRectangleSide::LEFT;
		break;
	case 3:
		return eRectangleSide::RIGHT;
		break;
	default:
		std::cout << "somethings wrong (convert_int_eRectangleSide)\n";
		return eRectangleSide::TOP;
		break;
	}
}

void printRectangleSide(eRectangleSide& tmpSide)
{
	switch (tmpSide)
	{
	case TOP:
		std::cout << "TOP\n";
		break;
	case BOT:
		std::cout << "BOT\n";
		break;
	case LEFT:
		std::cout << "LEFT\n";
		break;
	case RIGHT:
		std::cout << "RIGHT\n";
		break;
	default:
		break;
	}
}

Vector2D<float> rtnCorner(SDL_FRect& rect, eRectCorner rtnCorner)
{
	switch (rtnCorner)
	{
	case TopRight:
		return Vector2D<float> { (rect.x + rect.w), rect.y};
		break;
	case TopLeft:
		return Vector2D<float> { rect.x, rect.y};
		break;
	case BotRight:
		return Vector2D<float> { (rect.x + rect.w), (rect.y + rect.h)};
		break;
	case BotLeft:
		return Vector2D<float> { rect.x, (rect.y + rect.h)};
		break;
	default:
		std::cout << "Somethings wrong Mario in Vector2D<float> rtnCorner(SDL_Rect& rect, eRectCorner rtnCorner) \n";
		return Vector2D<float> { 0, 0};
		break;
	}
}

CollisionData check_Geometry_arbitrary(
	Geometry& geom1, Vector2D<float>& pos1, Movement& mov1,
	Geometry& geom2, Vector2D<float>& pos2, Movement& mov2
)
{
	// Collision between same Geometries
	if (geom1.return_GeomType() == geom2.return_GeomType())
	{
		switch (geom1.return_GeomType())
		{
		case eGeomType::RECTANGLE:
		{
			std::array<float, 2> rect1 = geom1.return_para_Rect();
			std::array<float, 2> rect2 = geom2.return_para_Rect();

			SDL_FRect collRect1 = create_Rect(pos1, rect1);
			SDL_FRect collRect2 = create_Rect(pos2, rect2);

			return calc_RectVsRect_distVec(collRect2, collRect1, mov1.direction, mov2.direction);
			break;
		}
		case eGeomType::RECTANGLE_ROTATET:
		{
			// not imoplementet yet
			std::cout << "somethings wrong (check_Geometry_arbitrary->rectangle rotatet)\n";
			return CollisionData{ ()};
			break;
		}
		case eGeomType::CIRCLE:
		{
			std::array<float, 1> circ1 = geom1.return_para_Circ();
			std::array<float, 1> circ2 = geom2.return_para_Circ();

			SDL_FRect collRect1_2 = geom1.return_AABB(pos1);
			SDL_FRect collRect2_2 = geom2.return_AABB(pos2);

			return calc_CircVsCirc_distVec(circ1[0], collRect1_2, circ2[0], collRect2_2);
			break;
		}
		case eGeomType::CIRCLE_SECTOR:
		{
			// not imoplementet yet
			std::cout << "somethings wrong (check_Geometry_arbitrary->circle sector)\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
		// both cases below shouldn't happen
		case eGeomType::POINT:
		{
			std::cout << "somethings wrong (check_Geometry_arbitrary->point)\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
		case eGeomType::LINE:
		{
			std::cout << "somethings wrong (check_Geometry_arbitrary->line)\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
		default:
		{
			std::cout << "somethings wrong (check_Geometry_arbitrary->default)\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
		}
	}
	else
	{
		switch (geom1.return_GeomType() + geom2.return_GeomType())
		{
		// Rectangle vs Rectangle_Rotatet
		case 6:
		{
			std::cout << "somethings wrong (check_Geometry_arbitrary) Rectangle vs Rectangle_Rotatet not defined\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
		// Rectangle vs Circle 
		case 10:
		{
			std::array<float, 1> circlePara;
			std::array<float, 2> rectanglePara;
			SDL_FRect circAABB{};
			SDL_FRect collRect{};
			if (geom1.return_GeomType() == eGeomType::RECTANGLE)
			{
				rectanglePara = geom1.return_para_Rect();
				collRect = create_Rect(pos1, rectanglePara);
				circlePara = geom2.return_para_Circ();
				circAABB = geom2.return_AABB(pos2);

				return calc_RectVsCirc_distVec(collRect, circlePara[0], circAABB, mov1.direction, mov2.direction);
			} 
			else
			{
				rectanglePara = geom2.return_para_Rect();
				collRect = create_Rect(pos2, rectanglePara);
				circlePara = geom1.return_para_Circ();
				circAABB = geom1.return_AABB(pos1);

				return calc_RectVsCirc_distVec(collRect, circlePara[0], circAABB, mov2.direction, mov1.direction);
			}
			break;
		}
		// Cirlce vs Rectangle_Rotatet
		case 12:
		{
			std::cout << "somethings wrong (check_Geometry_arbitrary) Cirlce vs Rectangle_Rotatet not defined\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
		// Rectangle vs Circle_Sector
		case 18:
		{
			std::cout << "somethings wrong (check_Geometry_arbitrary) Rectangle vs Circle_Sector not defined\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
		// Rectangle_Rotatet vs Circle_Sector
		case 20:
		{
			std::cout << "somethings wrong (check_Geometry_arbitrary) Rectangle_Rotatet vs Circle_Sector not defined\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
		// Circle vs Circle_Sector
		case 24:
		{
			std::cout << "somethings wrong (check_Geometry_arbitrary) Circle vs Circle_Sector not defined\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
		default:
		{
			std::cout << "somethings wrong (check_Geometry_arbitrary)\n";
			return CollisionDataSub{ Vector2D<float> {}, false };
			break;
		}
		}
	}

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

bool check_RectVsCirc(SDL_FRect& rect, float& circ_radius, Vector2D<float> pos)
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

bool check_PointVsRectEdge(SDL_FRect& rect, Vector2D<float>& point)
{
	bool rtnBool = false;

	float leftSide = rect.x;
	float rightSide = rect.x + rect.w;
	float topSide = rect.y + rect.h;
	float botSide = rect.y;

	if ((point[0] > leftSide && point[0] < rightSide) || floatsEqual(point[0], leftSide) || floatsEqual(point[0], rightSide))
	{
		if ((point[1] > botSide && point[1] < topSide) || floatsEqual(point[1], botSide) || floatsEqual(point[1], topSide))
		{
			rtnBool = true;
		}
	}
	return rtnBool;
}
