#include "Collision.h"

std::vector<Vector2D<float>> crossPoints_RectVsLine(SDL_FRect& rect, Line& line)
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

std::vector<Vector2D<float>> crossPoints_RectOpenVsLine(SDL_FRect& rect, Line& line)
{
	std::vector<Vector2D<float>> rtnCrossPoints;

	Line topLine = create_Line(eRectangleSide::TOP, rect);
	Line botLine = create_Line(eRectangleSide::BOT, rect);
	Line leftLine = create_Line(eRectangleSide::LEFT, rect);
	Line rightLine = create_Line(eRectangleSide::RIGHT, rect);

	Vector2D<float> tmpCrossPoint1 = crossPoint_LineVsLine(line, topLine);
	rtnCrossPoints.push_back(tmpCrossPoint1);

	Vector2D<float> tmpCrossPoint2 = crossPoint_LineVsLine(line, botLine);
	rtnCrossPoints.push_back(tmpCrossPoint2);

	Vector2D<float> tmpCrossPoint3 = crossPoint_LineVsLine(line, leftLine);
	rtnCrossPoints.push_back(tmpCrossPoint3);

	Vector2D<float> tmpCrossPoint4 = crossPoint_LineVsLine(line, rightLine);
	rtnCrossPoints.push_back(tmpCrossPoint4);
	
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

std::vector<Vector2D<float>> crossPoints_CircVsLine(float& circ_radius, Vector2D<float>& pos, Line& line)
{
	// solving Mitternachtsformel: a*x^2 + b*x + c = 0
	// x_1/2 = ( -b +/- sqrt(b^2 - 4*a*C) )/(2*a) 

	std::vector<Vector2D<float>> rtnVec{};

	float par_a{};
	float par_b{};
	float par_c{};
	float determinante{};

	switch (line.return_Linestyle())
	{
	case eLineStyle::HORIZONTAL:
	{
		par_a = 1.0f;
		par_b = -2.0f * pos[0];
		par_c = pos[0] * pos[0] + pos[1] * pos[1] - circ_radius * circ_radius +
			line.return_Offset_t() * line.return_Offset_t() - 2.0f * line.return_Offset_t() * pos[1];

		determinante = par_b * par_b - 4.0f * par_a * par_c;

		if (determinante < 0.0f)
		{
			rtnVec.push_back(Vector2D<float>{ 0.0f, 0.0f });
			std::cout << "something wrong Mario\n determinante negatice\n";
		}
		else if (floatsEqual(determinante, 0.0f))
		{
			float tmpX = -par_b / (2.0f * par_a);
			float tmpY = line.return_StartPoint()[1];

			rtnVec.push_back(Vector2D<float> { tmpX, tmpY });
			//std::cout << "something wrong Mario\n determinante zero\n";
		}
		else
		{
			float tmpX1 = (-par_b + std::sqrt(determinante)) / (2.0f * par_a);
			float tmpX2 = (-par_b - std::sqrt(determinante)) / (2.0f * par_a);

			float tmpY = line.return_StartPoint()[1];


			rtnVec.push_back(Vector2D<float> { tmpX1, tmpY });
			rtnVec.push_back(Vector2D<float> { tmpX2, tmpY });
		}

		break;
	}
	case eLineStyle::VERTICAL:
	{
		par_a = 1.0f;
		par_b = -2.0f * pos[1];
		par_c = pos[0] * pos[0] + pos[1] * pos[1] - circ_radius * circ_radius +
			line.return_StartPoint()[0] * line.return_StartPoint()[0] - 2.0f * line.return_StartPoint()[0] * pos[0];

		determinante = par_b * par_b - 4.0f * par_a * par_c;
		//std::cout << determinante << '\n';

		if (determinante < 0.0f)
		{
			rtnVec.push_back(Vector2D<float>{ 0.0f, 0.0f });
			std::cout << "something wrong Mario\n determinante negatice\n";
		}
		else if (floatsEqual(determinante, 0.0f))
		{
			float tmpY = -par_b / (2.0f * par_a);
			float tmpX = line.return_StartPoint()[0];

			rtnVec.push_back(Vector2D<float> { tmpX, tmpY });
			//std::cout << "something wrong Mario\n determinante zero\n";
		}
		else
		{
			float tmpX = line.return_StartPoint()[0];

			float tmpY1 = (-par_b + std::sqrt(determinante)) / (2.0f * par_a);
			float tmpY2 = (-par_b - std::sqrt(determinante)) / (2.0f * par_a);


			rtnVec.push_back(Vector2D<float> { tmpX, tmpY1 });
			rtnVec.push_back(Vector2D<float> { tmpX, tmpY2 });
		}
		break;
	}
	case eLineStyle::NORMAL:
	{
		par_a = 1;
		par_b = 2.0f * (line.return_Slope_m() * line.return_Offset_t() - pos[0] - line.return_Slope_m() * pos[1])
			/ (1.0f + line.return_Slope_m() * line.return_Slope_m()) ;
		par_c = (pos[0] * pos[0] + pos[1] * pos[1] - circ_radius * circ_radius
			+ line.return_Offset_t() * line.return_Offset_t() - 2.0f * line.return_Offset_t() * pos[1]) 
			/ (1.0f + line.return_Slope_m() * line.return_Slope_m());

		determinante = par_b * par_b - 4.0f * par_a * par_c;
		//std::cout << determinante << '\n';

		if (determinante < 0.0f)
		{
			rtnVec.push_back(Vector2D<float>{ 0.0f, 0.0f });
			std::cout << "something wrong Mario\n determinante negatice\n";

			// probably very close to zero and because of rounding errors negative
			float tmpX = -par_b / (2.0f * par_a);
			float tmpY = line.return_Slope_m() * tmpX + line.return_Offset_t();

			rtnVec.push_back(Vector2D<float> { tmpX, tmpY });
		}
		else if (floatsEqual(determinante, 0.0f))
		{
			float tmpX = -par_b / (2.0f * par_a);
			float tmpY = line.return_Slope_m() * tmpX + line.return_Offset_t();

			rtnVec.push_back(Vector2D<float> { tmpX, tmpY });
			//std::cout << "something wrong Mario\n determinante zero\n";
		}
		else
		{
			float tmpX1 = (-par_b + std::sqrt(determinante)) / (2.0f * par_a);
			float tmpX2 = (-par_b - std::sqrt(determinante)) / (2.0f * par_a);

			float tmpY1 = line.return_Slope_m() * tmpX1 + line.return_Offset_t();
			float tmpY2 = line.return_Slope_m() * tmpX2 + line.return_Offset_t();


			rtnVec.push_back(Vector2D<float> { tmpX1, tmpY1 });
			rtnVec.push_back(Vector2D<float> { tmpX2, tmpY2 });
		}
		break;
	}
	default:
		break;
	}

	return rtnVec;
}

std::vector<Vector2D<float>> crossPoints_RectVsCirc(SDL_FRect& rect, float& circ_radius, Vector2D<float>& pos)
{
	std::vector<Vector2D<float>> rtnCrossPoints;

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

	return rtnCrossPoints;
}