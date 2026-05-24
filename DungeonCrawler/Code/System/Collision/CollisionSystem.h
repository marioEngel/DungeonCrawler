#pragma once
#include "../../ECS/System.h"
#include "Collision.h"
#include "../../Component/Comp_Hitbox.h"

class CollisionSystem : public Systems
{
public:
	void check_AABB();
	void check_General();
	void react();

	bool check_specificComp_Hitbox(Entity& entity);

	void create_Comp_HitboxRender();
	void render_Comp_Hitbox();

private:
	std::vector<CollisionData> mCollisionData;
};

