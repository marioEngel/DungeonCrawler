#include "Collision.h"


CollisionDataSub check_Geometry_arbitrary(
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
			return CollisionDataSub{ Vector2D<float> {}, false };
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