#include "Collision.h"
#include "../../Component/PositionComp.h"

CollisionDataSub calc_CircVsCirc_distVec(float& circ1_radius, SDL_Rect& circ1_AABB, float& circ2_radius, SDL_Rect& circ2_AABB)
{
	Vector2D<float> circ1_center = calc_Rect_center(circ1_AABB);
	Vector2D<float> circ2_center = calc_Rect_center(circ2_AABB);

	if (check_CircleVsCircle(circ1_radius, circ1_center, circ2_radius, circ2_center))
	{
		Line directionLine{ calc_Rect_center(circ1_AABB) , calc_Rect_center(circ2_AABB) };

		// first circle 
		std::vector<Vector2D<float>> crossPointsCirc1 = crossPoint_CircVsLine(circ1_radius, circ1_center, directionLine);
		std::vector<float> distanceCrossPointsCirc1{};
		for (size_t i = 0; i < crossPointsCirc1.size(); i++)
		{
			distanceCrossPointsCirc1.push_back((calc_Rect_center(circ2_AABB) - crossPointsCirc1[i]).calc_amountSquared());
		}
		auto it1 = std::min_element(std::begin(distanceCrossPointsCirc1), std::end(distanceCrossPointsCirc1));
		int index1 = std::distance(std::begin(distanceCrossPointsCirc1), it1);

		// second circle 
		std::vector<Vector2D<float>> crossPointsCirc2 = crossPoint_CircVsLine(circ2_radius, circ2_center, directionLine);
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


std::vector<Vector2D<float>> crossPoint_CircVsLine(float& circ_radius, Vector2D<float>& pos, Line& line)
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
		par_a = 1.0f;
		par_b = -2.0f * pos[0];
		par_c = pos[0] * pos[0] + pos[1] * pos[1] - circ_radius * circ_radius + 
			line.return_Offset_t() * line.return_Offset_t() - 2.0f * line.return_Offset_t() * pos[1];

		determinante = par_b * par_b - 4.0f * par_a * par_c;
		//std::cout << determinante << '\n';

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
	case eLineStyle::VERTICAL:
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
	case eLineStyle::NORMAL:
		par_a = (1.0f + line.return_Slope_m() * line.return_Slope_m());
		par_b = 2.0f * (line.return_Slope_m() * line.return_Offset_t() - pos[0] - line.return_Slope_m() * pos[1]);
		par_c = (pos[0] * pos[0] + pos[1] * pos[1] - circ_radius * circ_radius
			+ line.return_Offset_t() * line.return_Offset_t() - 2.0f * line.return_Offset_t() * pos[1]);

		determinante = par_b * par_b - 4.0f * par_a * par_c;
		//std::cout << determinante << '\n';

		if (determinante < 0.0f)
		{
			rtnVec.push_back(Vector2D<float>{ 0.0f, 0.0f });
			std::cout << "something wrong Mario\n determinante negatice\n";

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
	default:
		break;
	}

	return rtnVec;
}