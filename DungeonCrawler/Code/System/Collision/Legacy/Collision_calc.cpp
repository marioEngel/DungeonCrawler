#include "Collision.h"

CollisionDataSub calc_RectVsRect_distVec(SDL_FRect & rect1, SDL_FRect & rect2, Vector2D<float> rect1_direction, Vector2D<float> rect2_direction)
{
	/*
	1. Calculate the cross points between the line of movement of one rectangle and the other rectangle
	2. Do it again for the other rectangle
	3. Chose the cross points which are clossest to the rectangle
	*/
	// First Rectangle
	Line directionLine{ calc_Rect_center(rect1), calc_Rect_center(rect1) + rect1_direction };

	// first rect
	std::vector<float> distanceCrossPointsRect1{};
	std::vector<Vector2D<float>> crossPointsRect1 = crossPoints_RectVsLine(rect1, directionLine);
	for (size_t i = 0; i < crossPointsRect1.size(); i++)
	{
		distanceCrossPointsRect1.push_back((calc_Rect_center(rect2) - crossPointsRect1[i]).calc_amountSquared());
	}
	auto it1 = std::min_element(std::begin(distanceCrossPointsRect1), std::end(distanceCrossPointsRect1));
	int index1 = std::distance(std::begin(distanceCrossPointsRect1), it1);

	// second rect
	std::vector<float> distanceCrossPointsRect2{};
	std::vector<Vector2D<float>> crossPointsRect2 = crossPoints_RectOpenVsLine(rect2, directionLine);
	for (size_t i = 0; i < crossPointsRect2.size(); i++)
	{
		distanceCrossPointsRect2.push_back((crossPointsRect1[index1] - crossPointsRect2[i]).calc_amountSquared());
	}
	auto it2 = std::min_element(std::begin(distanceCrossPointsRect2), std::end(distanceCrossPointsRect2));
	int index2 = std::distance(std::begin(distanceCrossPointsRect2), it2);

	Vector2D<float> finalRect = crossPointsRect1[index1] - crossPointsRect2[index2];
	return CollisionDataSub{ finalRect, true };
}

CollisionDataSub calc_CircVsCirc_distVec(float& circ1_radius, SDL_FRect& circ1_AABB, float& circ2_radius, SDL_FRect& circ2_AABB)
{
	Vector2D<float> circ1_center = calc_Rect_center(circ1_AABB);
	Vector2D<float> circ2_center = calc_Rect_center(circ2_AABB);

	if (check_CircleVsCircle(circ1_radius, circ1_center, circ2_radius, circ2_center))
	{
		Line directionLine{ calc_Rect_center(circ1_AABB) , calc_Rect_center(circ2_AABB) };

		// first circle 
		std::vector<Vector2D<float>> crossPointsCirc1 = crossPoints_CircVsLine(circ1_radius, circ1_center, directionLine);
		std::vector<float> distanceCrossPointsCirc1{};
		for (size_t i = 0; i < crossPointsCirc1.size(); i++)
		{
			distanceCrossPointsCirc1.push_back((calc_Rect_center(circ2_AABB) - crossPointsCirc1[i]).calc_amountSquared());
		}
		auto it1 = std::min_element(std::begin(distanceCrossPointsCirc1), std::end(distanceCrossPointsCirc1));
		int index1 = std::distance(std::begin(distanceCrossPointsCirc1), it1);

		// second circle 
		std::vector<Vector2D<float>> crossPointsCirc2 = crossPoints_CircVsLine(circ2_radius, circ2_center, directionLine);
		std::vector<float> distanceCrossPointsCirc2{};
		for (size_t i = 0; i < crossPointsCirc2.size(); i++)
		{
			distanceCrossPointsCirc2.push_back((calc_Rect_center(circ1_AABB) - crossPointsCirc2[i]).calc_amountSquared());
		}
		auto it2 = std::min_element(std::begin(distanceCrossPointsCirc2), std::end(distanceCrossPointsCirc2));
		int index2 = std::distance(std::begin(distanceCrossPointsCirc2), it2);

		Vector2D<float> finalRect = crossPointsCirc2[index2] - crossPointsCirc1[index1];

		return CollisionDataSub{ finalRect, true };
	}
	else
	{
		return CollisionDataSub{ Vector2D<float> {0.0f, 0.0f}, false };
	}
}

CollisionDataSub calc_RectVsCirc_distVec(SDL_FRect& rect, float& circ_radius, SDL_FRect& circ_AABB, Vector2D<float> rect_dir, Vector2D<float> circ_dir)
{
	Vector2D<float> circCentre = calc_Rect_center(circ_AABB);
	Vector2D<float> directionVec{};
	bool isPlayerRect;

	// check which direction is bigger (which is not zero and therefore the player)
	if (rect_dir.calc_amountSquared() < circ_dir.calc_amountSquared())
	{
		directionVec = circ_dir;
		isPlayerRect = false;
	}
	else
	{
		directionVec = rect_dir;
		isPlayerRect = true;
	}


	if (check_RectVsCirc(rect, circ_radius, circCentre))
	{
		Line directionLine{ calc_Rect_center(rect) , circCentre };
		eRectangleSide tmpSide = side_RectVsLine(rect, directionLine, circCentre);
		float tmpAmount{ 0.0f };

		switch (tmpSide)
		{
		case TOP:
		{
			if (circCentre[0] < rect.x)
			{
				Line diag_topLeft_botRight = Line{ rtnCorner(rect, eRectCorner::TopLeft), rtnCorner(rect, eRectCorner::BotRight) };
				std::vector<Vector2D<float>> crossPoints = crossPoints_CircVsLine(circ_radius, circCentre, diag_topLeft_botRight);
				int index_crossPoints = 0;
				if (crossPoints[1].returnXval() > crossPoints[0].returnXval())
				{
					index_crossPoints = 1;
				}
				directionVec = crossPoints[index_crossPoints] - rtnCorner(rect, eRectCorner::TopLeft);
			}
			else if (circCentre[0] > (rect.x + rect.w))
			{
				Line diag_botLeft_topRight = Line{ rtnCorner(rect, eRectCorner::BotLeft), rtnCorner(rect, eRectCorner::TopRight) };
				std::vector<Vector2D<float>> crossPoints = crossPoints_CircVsLine(circ_radius, circCentre, diag_botLeft_topRight);
				int index_crossPoints = 0;
				if (crossPoints[1].returnXval() < crossPoints[0].returnXval())
				{
					index_crossPoints = 1;
				}
				directionVec = crossPoints[index_crossPoints] - rtnCorner(rect, eRectCorner::TopRight);
			}
			else
			{
				tmpAmount = (circCentre[1] + circ_radius) - rect.y;
				directionVec = Vector2D<float>{ 0.0f, 1.0f };
				directionVec.scaleToY(float(tmpAmount));
			}
			break;
		}
		case BOT:
		{
			if (circCentre[0] < rect.x)
			{
				Line diag_botLeft_topRight = Line{ rtnCorner(rect, eRectCorner::BotLeft), rtnCorner(rect, eRectCorner::TopRight) };
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
				Line diag_topLeft_botRight = Line{ rtnCorner(rect, eRectCorner::TopLeft), rtnCorner(rect, eRectCorner::BotRight) };
				std::vector<Vector2D<float>> crossPoints = crossPoints_CircVsLine(circ_radius, circCentre, diag_topLeft_botRight);
				int index_crossPoints = 0;
				if (crossPoints[1].returnXval() < crossPoints[0].returnXval())
				{
					index_crossPoints = 1;
				}
				directionVec = crossPoints[index_crossPoints] - rtnCorner(rect, eRectCorner::BotRight);
			}
			else
			{
				tmpAmount = (rect.y + rect.h) - (circCentre[1] - circ_radius);
				directionVec = Vector2D<float>{ 0.0f, -1.0f };
				directionVec.scaleToY(float(-tmpAmount));
			}
			break;
		}
		case LEFT:
		{
			Vector2D<float> prevDirectionVec = directionVec;

			if (circCentre[1] < rect.y)
			{
				Line diag_topLeft_botRight = Line{ rtnCorner(rect, eRectCorner::TopLeft), rtnCorner(rect, eRectCorner::BotRight) };
				std::vector<Vector2D<float>> crossPoints = crossPoints_CircVsLine(circ_radius, circCentre, diag_topLeft_botRight);
				int index_crossPoints = 0;
				if (crossPoints[1].returnYval() > crossPoints[0].returnYval())
				{
					index_crossPoints = 1;
				}
				directionVec = crossPoints[index_crossPoints] - rtnCorner(rect, eRectCorner::TopLeft);
			}
			else if (circCentre[1] > (rect.y + rect.h))
			{
				Line diag_botLeft_topRight = Line{ rtnCorner(rect, eRectCorner::BotLeft), rtnCorner(rect, eRectCorner::TopRight) };
				std::vector<Vector2D<float>> crossPoints = crossPoints_CircVsLine(circ_radius, circCentre, diag_botLeft_topRight);
				int index_crossPoints = 0;
				if (crossPoints[1].returnYval() < crossPoints[0].returnYval())
				{
					index_crossPoints = 1;
				}
				directionVec = crossPoints[index_crossPoints] - rtnCorner(rect, eRectCorner::BotLeft);
			}
			else
			{
				tmpAmount = (circCentre[0] + circ_radius) - rect.x;
				directionVec = Vector2D<float>{ 1.0f, 0.0f };
				directionVec.scaleToX(tmpAmount);
			}
			break;
		}
		case RIGHT:
		{
			if (circCentre[1] < rect.y)
			{
				Line diag_botLeft_topRight = Line{ rtnCorner(rect, eRectCorner::BotLeft), rtnCorner(rect, eRectCorner::TopRight) };
				std::vector<Vector2D<float>> crossPoints = crossPoints_CircVsLine(circ_radius, circCentre, diag_botLeft_topRight);
				int index_crossPoints = 0;
				if (crossPoints[1].returnYval() > crossPoints[0].returnYval())
				{
					index_crossPoints = 1;
				}
				directionVec = crossPoints[index_crossPoints] - rtnCorner(rect, eRectCorner::TopRight);
			}
			else if (circCentre[1] > (rect.y + rect.h))
			{
				Line diag_topLeft_botRight = Line{ rtnCorner(rect, eRectCorner::TopLeft), rtnCorner(rect, eRectCorner::BotRight) };
				std::vector<Vector2D<float>> crossPoints = crossPoints_CircVsLine(circ_radius, circCentre, diag_topLeft_botRight);
				int index_crossPoints = 0;
				if (crossPoints[1].returnYval() < crossPoints[0].returnYval())
				{
					index_crossPoints = 1;
				}
				directionVec = crossPoints[index_crossPoints] - rtnCorner(rect, eRectCorner::BotRight);
			}
			else
			{
				tmpAmount = (rect.x + rect.w) - (circCentre[0] - circ_radius);
				directionVec = Vector2D<float>{ -1.0f, 0.0f };
				directionVec.scaleToX(float(-tmpAmount));
			}
			break;
		}
		default:
			break;
		}

		if (isPlayerRect)
		{
			return CollisionDataSub{ directionVec, true };
		}
		else
		{
			return CollisionDataSub{ directionVec.flip(), true };
		}
	}
	else
	{
		return CollisionDataSub{ Vector2D<float> {0.0f, 0.0f}, false };
	}
}