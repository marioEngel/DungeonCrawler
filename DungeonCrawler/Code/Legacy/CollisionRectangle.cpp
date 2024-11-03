#include "Collision.h"

CollisionDataSub calc_RectVsRect_distVec(SDL_Rect& rect1, SDL_Rect& rect2)
{
	if (check_RectVsRect(rect1, rect2))
	{
		//Vector2D<float> directionVec = calc_Rect_center(rect2) - calc_Rect_center(rect1);
		Line directionLine{ calc_Rect_center(rect1), calc_Rect_center(rect2) };

		// first rect
		std::vector<Vector2D<float>> crossPointsRect1 = crossPoint_RectVsLine(rect1, directionLine);
		std::vector<float> distanceCrossPointsRect1{};
		for (size_t i = 0; i < crossPointsRect1.size(); i++)
		{
			distanceCrossPointsRect1.push_back((calc_Rect_center(rect2) - crossPointsRect1[i]).calc_amountSquared());
		}
		auto it1 = std::min_element(std::begin(distanceCrossPointsRect1), std::end(distanceCrossPointsRect1));
		int index1 = std::distance(std::begin(distanceCrossPointsRect1), it1);

		// second rect
		std::vector<Vector2D<float>> crossPointsRect2 = crossPoint_RectVsLine(rect2, directionLine);
		std::vector<float> distanceCrossPointsRect2{};
		for (size_t i = 0; i < crossPointsRect2.size(); i++)
		{
			distanceCrossPointsRect2.push_back((calc_Rect_center(rect1) - crossPointsRect2[i]).calc_amountSquared());
		}
		auto it2 = std::min_element(std::begin(distanceCrossPointsRect2), std::end(distanceCrossPointsRect2));
		int index2 = std::distance(std::begin(distanceCrossPointsRect2), it2);

		Vector2D<float> finalRect = crossPointsRect2[index2] - crossPointsRect1[index1];

		return CollisionDataSub{ finalRect, true };
	}
	else
	{
		return CollisionDataSub{ Vector2D<float>{0.0f, 0.0f}, false };
	}

}

std::vector<Vector2D<float>> crossPoint_RectVsLine(SDL_Rect& rect, Line& line)
{
	std::vector<Vector2D<float>> rtnCrossPoints;

	Line topLine = create_Line(eRectangleSide::TOP, rect);
	Line botLine = create_Line(eRectangleSide::BOT, rect);
	Line leftLine = create_Line(eRectangleSide::LEFT, rect);
	Line rightLine = create_Line(eRectangleSide::RIGHT, rect);

	Vector2D<float> tmpCrossPoint1 = crossPoint_LineVsLine(line, topLine);
	if (check_PointVsRectEdge(rect, tmpCrossPoint1))
	{
		rtnCrossPoints.push_back(tmpCrossPoint1);
	}

	Vector2D<float> tmpCrossPoint2 = crossPoint_LineVsLine(line, botLine);
	if (check_PointVsRectEdge(rect, tmpCrossPoint2))
	{
		rtnCrossPoints.push_back(tmpCrossPoint2);
	}

	Vector2D<float> tmpCrossPoint3 = crossPoint_LineVsLine(line, leftLine);
	if (check_PointVsRectEdge(rect, tmpCrossPoint3))
	{
		rtnCrossPoints.push_back(tmpCrossPoint3);
	}

	Vector2D<float> tmpCrossPoint4 = crossPoint_LineVsLine(line, rightLine);
	if (check_PointVsRectEdge(rect, tmpCrossPoint4))
	{
		rtnCrossPoints.push_back(tmpCrossPoint4);
	}

	return rtnCrossPoints;
}

Vector2D<float> crossPoint_LineVsLine(Line& line1, Line& line2)
{
	if (line1.return_Linestyle() == line2.return_Linestyle())
	{
		float tmpX = 0.0f;
		float tmpY = 0.0f;

		switch (line1.return_Linestyle())
		{
		case eLineStyle::NORMAL:
			tmpX = (line2.return_Offset_t() - line1.return_Offset_t()) / (line1.return_Slope_m() - line2.return_Slope_m());
			tmpY = (line1.return_Slope_m() + line2.return_Slope_m()) / 2.0f * tmpX + (line1.return_Offset_t() + line2.return_Offset_t()) / 2.0f;

			return Vector2D<float> {tmpX, tmpY};
			break;
		case eLineStyle::HORIZONTAL:
			//std::cout << "probably parallel\n";
			return Vector2D<float> {tmpX, tmpY};
			break;
		case eLineStyle::VERTICAL:
			//std::cout << "probably parallel\n";
			return Vector2D<float> {tmpX, tmpY};
			break;
		default:
			std::cout << "something wrong Mario\n";
			return Vector2D<float> {tmpX, tmpY};
			break;
		}
	}
	else
	{
		float tmpX = 0.0f;
		float tmpY = 0.0f;

		switch (line1.return_Linestyle() + line2.return_Linestyle())
		{
		// Normal-Horizontal
		case 2:
			if (line1.return_Linestyle() == eLineStyle::NORMAL)
			{
				tmpY = line2.return_StartPoint()[1];
				tmpX = (tmpY - line1.return_Offset_t()) / line1.return_Slope_m();

				return Vector2D<float> {tmpX, tmpY};
			}
			else
			{
				tmpY = line1.return_StartPoint()[1];
				tmpX = (tmpY - line2.return_Offset_t()) / line2.return_Slope_m();

				return Vector2D<float> {tmpX, tmpY};
			}
			break;
		// Normal-Vertical
		case 4:
			if (line1.return_Linestyle() == eLineStyle::NORMAL)
			{
				tmpX = line2.return_StartPoint()[0];
				tmpY = line1.return_Slope_m() * tmpX + line1.return_Offset_t();

				return Vector2D<float> {tmpX, tmpY};
			}
			else
			{
				tmpX = line1.return_StartPoint()[0];
				tmpY = line2.return_Slope_m() * tmpX + line2.return_Offset_t();

				return Vector2D<float> {tmpX, tmpY};
			}
			break;
		// Horizontal-Vertical
		case 6:
			if (line1.return_Linestyle() == eLineStyle::HORIZONTAL)
			{
				float tmpY = line1.return_StartPoint()[1];
				float tmpX = line2.return_StartPoint()[0];

				return Vector2D<float> {tmpX, tmpY};
			}
			else
			{
				tmpY = line2.return_StartPoint()[1];
				tmpX = line1.return_StartPoint()[0];

				return Vector2D<float> {tmpX, tmpY};
			}
			break;
		default:
			std::cout << "something wrong Mario\n";
			return Vector2D<float> {tmpX, tmpY};
			break;
		}
	}
}

Vector2D<float> calc_Rect_center(SDL_Rect& rect)
{
	return Vector2D<float> {rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f };
}

Line create_Line(eRectangleSide side, SDL_Rect& rect)
{
	// y-axis inverted
	Vector2D<float> corner_topLeft{ float(rect.x), float(rect.y) };
	Vector2D<float> corner_topRight{ float(rect.x + rect.w), float(rect.y) };
	Vector2D<float> corner_botLeft{ float(rect.x), float(rect.y + rect.h) };
	Vector2D<float> corner_botRight{ float(rect.x + rect.w), float(rect.y + rect.h) };


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

