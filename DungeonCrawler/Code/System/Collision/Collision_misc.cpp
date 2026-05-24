#include "Collision.h"

SDL_FRect create_Rect(Vector2D<float>& cornerPoint, std::array<float, 2>& remainderData)
{
	float tmpX = cornerPoint[0];
	float tmpY = cornerPoint[1];
	float tmpW = remainderData[0];
	float tmpH = remainderData[0];
	return SDL_FRect{ tmpX, tmpY, tmpW, tmpH };
}

void print_Rect_data(SDL_FRect& rect)
{
	std::cout << "rectangle data: x:" << rect.x << " y:" << rect.y << " h:" << rect.h << " w:" << rect.w << std::endl;
	std::cout << "middle point: " << calc_Rect_center(rect) << std::endl;
}

Vector2D<float> calc_Rect_center(SDL_FRect& rect)
{
	return Vector2D<float> {rect.x + rect.w / 2.0f, rect.y + rect.h / 2.0f };
}

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