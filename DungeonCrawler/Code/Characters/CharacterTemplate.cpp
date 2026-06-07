#include "CharacterTemplate.h"
#include "../ECS/Coordinator.h"
#include "../Component/Comp_Position.h"
#include "../Component/Comp_Texture.h"
#include "../Component/Comp_Hitbox.h"
#include "../Component/Is_Collision.h"
#include "../Component/Comp_Movement.h"
#include "../Component/Comp_Mass.h"
#include "../System/Collision/Geometry/Circle.h"

extern Coordinator gCoordinator;
const char* pic_ratte = "Picture/Ratte.png";
GeomCircle hitbox_rat{ 16.0f, 16.0f, 16.0f };
const char* texture_hitbox_rat = "Picture/FullPinkCircle.png";


void create_Mouse(Vector2D<float> position)
{
	Entity tmpMouse = gCoordinator.CreateEntity();

	gCoordinator.AddComponent<Position>(tmpMouse, Position{ position });
	gCoordinator.AddComponent<Texture>(tmpMouse, Texture{ pic_ratte});
	gCoordinator.AddComponent<Hitbox>(tmpMouse, Hitbox{ std::make_shared<GeomCircle>(hitbox_rat), eCollisionType::PHYSICAL, texture_hitbox_rat});
	gCoordinator.AddComponent<IsCollision>(tmpMouse, IsCollision{});
	gCoordinator.AddComponent<Movement>(tmpMouse, Movement{ 0, Vector2D<float>{}, true });
	gCoordinator.AddComponent<Mass>(tmpMouse, Mass{ 10, true });
}