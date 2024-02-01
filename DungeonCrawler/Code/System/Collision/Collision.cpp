#include "Collision.h"
#include "Geometry/Geometry.h"
#include <fstream>

extern int gFrameNumber;

// simple collision test for geometries
bool check_RectVsRect(SDL_Rect& rect1, SDL_Rect& rect2)
{
	bool returnBool = false;

	if ((rect1.x + rect1.w) > rect2.x &&
		rect1.x < (rect2.x + rect2.w) &&
		(rect1.y + rect1.h) > rect2.y &&
		rect1.y < (rect2.y + rect2.h))
	{
		returnBool = true;
	}

	return returnBool;
}

bool check_RectVsPoint(SDL_Rect& rect, Vector2D<float>& vec)
{
	bool returnBool = false;

	if (int(vec[0]) >= rect.x &&
		int(vec[0]) <= (rect.x + rect.w) &&
		int(vec[1]) >= rect.y &&
		int(vec[1]) <= (rect.y + rect.h) )
	{
		returnBool = true;
	}

	return returnBool;
}

bool check_RectVsCirc(SDL_Rect& rect, float& circ_radius, Vector2D<float> pos)
{
	bool collision = false;
	std::vector<Vector2D<float>> rtnCrossPoints{};

	Line topLine = create_Line(eRectangleSide::TOP, rect);
	Line botLine = create_Line(eRectangleSide::BOT, rect);
	Line leftLine = create_Line(eRectangleSide::LEFT, rect);
	Line rightLine = create_Line(eRectangleSide::RIGHT, rect);


	if (check_determinante(circ_radius, pos, topLine))
	{
		std::vector<Vector2D<float>> tmpVec = crossPoint_CircVsLine(circ_radius, pos, topLine);
		for (size_t i = 0; i < tmpVec.size(); i++)
		{
			rtnCrossPoints.push_back(tmpVec[i]);
		}
	}
	if (check_determinante(circ_radius, pos, botLine))
	{
		std::vector<Vector2D<float>> tmpVec = crossPoint_CircVsLine(circ_radius, pos, botLine);
		for (size_t i = 0; i < tmpVec.size(); i++)
		{
			rtnCrossPoints.push_back(tmpVec[i]);
		}
	}
	if (check_determinante(circ_radius, pos, leftLine))
	{
		std::vector<Vector2D<float>> tmpVec = crossPoint_CircVsLine(circ_radius, pos, leftLine);
		for (size_t i = 0; i < tmpVec.size(); i++)
		{
			rtnCrossPoints.push_back(tmpVec[i]);
		}
	}
	if (check_determinante(circ_radius, pos, rightLine))
	{
		std::vector<Vector2D<float>> tmpVec = crossPoint_CircVsLine(circ_radius, pos, rightLine);
		for (size_t i = 0; i < tmpVec.size(); i++)
		{
			rtnCrossPoints.push_back(tmpVec[i]);
		}
	}

	for (size_t i = 0; i < rtnCrossPoints.size(); i++)
	{
	
		if (check_PointVsRectEdge(rect, rtnCrossPoints[i]))
		{
			collision = true;
		}
	}

	return collision;
}

bool check_CircleVsCircle(float& radius1, Vector2D<float>& position1, float& radius2, Vector2D<float>& position2)
{
	bool returnBool = false;
	if ((position2 - position1).calc_amountSquared() < ((radius1 + radius2) * (radius1 + radius2)))
	{
		returnBool = true;
	}

	return returnBool;
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

CollisionDataSub check_Geometry_arbitrary(Geometry& geom1, Vector2D<float>& pos1, Geometry& geom2, Vector2D<float>& pos2)
{
	if (geom1.return_GeomType() == geom2.return_GeomType())
	{
		switch (geom1.return_GeomType())
		{
		case eGeomType::RECTANGLE:
			std::array<float, 2> rect1 = geom1.return_para_Rect();
			std::array<float, 2> rect2 = geom2.return_para_Rect();

			SDL_Rect collRect1_1 = create_Rect(pos1, rect1);
			SDL_Rect collRect2_1 = create_Rect(pos2, rect2);

			return calc_RectVsRect_distVec(collRect1_1, collRect2_1);
			break;
		case eGeomType::CIRCLE:
			std::array<float, 1> circ1 = geom1.return_para_Circ();
			std::array<float, 1> circ2 = geom2.return_para_Circ();

			SDL_Rect collRect1_2 = geom1.return_AABB(pos1);
			SDL_Rect collRect2_2 = geom2.return_AABB(pos2);
			 
			return calc_CircVsCirc_distVec(circ1[0], collRect1_2, circ2[0], collRect2_2);
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

				return calc_RectVsCirc_distVec(collRect, circlePara[0], circAABB, true);
			}
			else
			{
				rectanglePara = geom2.return_para_Rect();
				collRect = create_Rect(pos2, rectanglePara);
				circlePara = geom1.return_para_Circ();
				circAABB = geom1.return_AABB(pos1);

				return calc_RectVsCirc_distVec(collRect, circlePara[0], circAABB, false);
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

// utils
SDL_Rect create_Rect(Vector2D<float>& cornerPoint, std::array<float, 2>& remainderData)
{
	int tmpX = int(cornerPoint[0]);
	int tmpY = int(cornerPoint[1]);
	int tmpW = int(remainderData[0]);
	int tmpH = int(remainderData[0]);
	return SDL_Rect{ tmpX, tmpY, tmpW, tmpH };
}

void print_Rect_data(SDL_Rect& rect)
{
	std::cout << "rectangle data: x:" << rect.x << " y:" << rect.y << " h:" << rect.h << " w:" << rect.w << std::endl;
	std::cout << "middle point: " << calc_Rect_center(rect) << std::endl;
}

// legacy code how to write a file
/*
	if (false)
	{
		float tmpAmount = calc_RectVsRect_distVec(collRect1_1, collRect2_1).calc_amount();
		std::cout << tmpAmount << '\n';

		std::ofstream outputFile;

		outputFile.open("tmpAmountDataRect.txt", std::ios_base::app); // append instead of overwrite
		outputFile << tmpAmount << "\n";
		outputFile.close();
	}

	if (true)
	{
		std::ofstream outputCircOne;
		outputCircOne.open("tmpCircle_circ1data", std::ios_base::app);
		outputCircOne << circ1[0] << ' ' << collRect1_2.x << ' ' << collRect1_2.y << ' ' << gFrameNumber << '\n';
		outputCircOne.close();

		std::ofstream outputCircTwo;
		outputCircTwo.open("tmpCircle_circ2data", std::ios_base::app);
		outputCircTwo << circ2[0] << ' ' << collRect2_2.x << ' ' << collRect2_2.y << ' ' << gFrameNumber << '\n';
		outputCircTwo.close();

		std::ofstream outputFile;
		outputFile.open("tmpCircle_norm.txt", std::ios_base::app); // append instead of overwrite
		outputFile << calc_CircVsCirc_distVec(circ1[0], collRect1_2, circ2[0], collRect2_2)[0] << ' '
			<< calc_CircVsCirc_distVec(circ1[0], collRect1_2, circ2[0], collRect2_2)[1] << ' ' << gFrameNumber << "\n";
		outputFile.close();
	}
*/