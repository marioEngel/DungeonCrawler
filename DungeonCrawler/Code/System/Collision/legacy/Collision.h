#pragma once
#include "../../Component/Comp_Hitbox.h"
#include "../../ECS/System.h"
#include "CollisionFunc.h"

class SysCollision : public Systems
{
public:
	void check_AABB();
	void check_General();
	void react();

	bool check_specificHitbox(Entity& entity);

	void create_HitboxRender();
	void render_Hitbox();

private:
	std::vector<CollisionData> mCollisionData;
};

