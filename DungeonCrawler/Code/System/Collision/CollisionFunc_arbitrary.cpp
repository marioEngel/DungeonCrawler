#include "CollisionFunc.h"

Contact check_Geometry_arbitrary(
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

				return check_RectVsRect_contact(collRect2, collRect1);
				break;
			}
			case eGeomType::CIRCLE:
			{
				std::array<float, 1> circ1 = geom1.return_para_Circ();
				std::array<float, 1> circ2 = geom2.return_para_Circ();

				return check_CircVsCirc_contact(pos1, circ1[0], pos2, circ2[0]);
				break;
			}

			default:
			{
				std::cout << "somethings wrong (check_Geometry_arbitrary->default)\n";
				return Contact{ Vector2D<float> { }, 0.0f, false};
				break;
			}
		}
	}
	else
	{
		switch (geom1.return_GeomType() + geom2.return_GeomType())
		{
		// Rectangle vs Circle 
		case 10:
		{
			float circleRadius;
			std::array<float, 2> rectanglePara;
			SDL_FRect collRect{};

			if (geom1.return_GeomType() == eGeomType::RECTANGLE)
			{
				rectanglePara = geom1.return_para_Rect();
				collRect = create_Rect(pos1, rectanglePara);
				circleRadius = geom2.return_para_Circ()[0];
				Vector2D<float> circleCenter = geom2.return_AABB_Center(pos2);

				return check_CircleVsRect_contact(circleCenter, circleRadius, collRect);
			}
			else
			{
				rectanglePara = geom2.return_para_Rect();
				collRect = create_Rect(pos2, rectanglePara);
				circleRadius = geom1.return_para_Circ()[0];
				Vector2D<float> circleCenter = geom1.return_AABB_Center(pos1);

				return check_CircleVsRect_contact(circleCenter, circleRadius, collRect, true);
			}
			break;
		}
		default:
		{
			std::cout << "somethings wrong (check_Geometry_arbitrary)\n";
			return Contact{ Vector2D<float> { }, 0.0f, false };
			break;
		}
		}
	}
}