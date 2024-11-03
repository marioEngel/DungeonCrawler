#include "Collision.h"
#include <iostream>
#include <fstream>
#include "../../Misc/MistFunctions.h"

CollisionDataSub calc_RectVsCirc_distVec(SDL_Rect& rect, float& circ_radius, SDL_Rect& circ_AABB, bool playerRect)
{
	Vector2D<float> circCentre = calc_Rect_center(circ_AABB);
	Vector2D<float> directionVec = circCentre - calc_Rect_center(rect);

	if (check_RectVsCirc(rect, circ_radius, circCentre))
	{
		// testing
		//std::ofstream outFile("C:/Users/Mario/Desktop/GameDev/directionVals.txt", std::ios::app);
		
		Line directionLine{ calc_Rect_center(rect) , circCentre };

		eRectangleSide tmpSide = side_RectVsLine(rect, directionLine, circCentre);
		printRectangleSide(tmpSide);
		int tmpAmount{ 0 };
		std::cout << "directionVec1: " << directionVec << '\n';


		switch (tmpSide)
		{
		case TOP:
			tmpAmount = (circCentre[1] + circ_radius) - rect.y;
			directionVec.scaleToY(float(tmpAmount));
			break;
		case BOT:
			if (circCentre[0] < rect.x)
			{
				Line diag_botLeft_topRight = Line{rtnCorner(rect, eRectCorner::BotLeft), rtnCorner(rect, eRectCorner::TopRight) };
				std::vector<Vector2D<float>> crossPoints = crossPoints_CircVsLine(circ_radius, circCentre, diag_botLeft_topRight);
				int index_crossPoints = 0;
				if (crossPoints[1].returnXval() > crossPoints[0].returnXval())
				{
					index_crossPoints = 1;
				}
				directionVec = crossPoints[index_crossPoints] - rtnCorner(rect, eRectCorner::BotLeft);
			}
			else if (circCentre[0] > (rect.x + rect.w))
			{

			}
			else
			{
				tmpAmount = (rect.y + rect.h) - (circCentre[1] - circ_radius);
				directionVec.scaleToY(float(-tmpAmount));
			}
			break;
		case LEFT:
			tmpAmount = (circCentre[0] + circ_radius) - rect.x;
			directionVec.scaleToX(float(tmpAmount));
			break;
		case RIGHT:
			tmpAmount = (rect.x + rect.w) - (circCentre[0] - circ_radius);
			directionVec.scaleToX(float(-tmpAmount));
			break;
		default:
			break;
		}

		std::cout << "amount: " << tmpAmount << '\n';
		std::cout << "directionVec1: " << directionVec << '\n' << '\n';

		//if (outFile.is_open())
		//{
		//	outFile << directionVec.returnXval();
		//	outFile << " ";
		//	outFile << directionVec.returnYval();
		//	outFile << " ";
		//	outFile << tmpAmount;
		//	outFile << " ";
		//	outFile << rect.x;
		//	outFile << " ";
		//	outFile << rect.y;
		//	outFile << " ";
		//	outFile << rect.h;
		//	outFile << " ";
		//	outFile << rect.w;
		//	outFile << " ";
		//	outFile << circCentre.returnXval();
		//	outFile << " ";
		//	outFile << circCentre.returnYval();
		//	outFile << " ";
		//	outFile << circ_radius;
		//	outFile << " ";
		//	outFile << "\n";

		//	outFile.close();
		//}

		if (playerRect)
		{
			return CollisionDataSub{ directionVec, true };
		}
		else
		{
			return CollisionDataSub{ directionVec.flip(), true};
		}
	}
	else
	{
		return CollisionDataSub{ Vector2D<float> {0.0f, 0.0f}, false};
	}
}

std::vector<Vector2D<float>> crossPoint_RectVsCirc(SDL_Rect& rect, float& circ_radius, Vector2D<float>& pos)
{
	std::vector<Vector2D<float>> rtnCrossPoints;

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

	return rtnCrossPoints;
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

eRectangleSide side_RectVsLine(SDL_Rect& rect, Line& line, Vector2D<float>& circleCentre)
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

	//std::cout << lessCrossPoints.size() << '\n';

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
