#pragma once
#include "../../ECS/System.h"
#include "Collision.h"

class CollisionSystem : public Systems
{
public:
	void check_AABB();
	void check_General();
	void react();

	
	void create_HitboxRender();
	void render_Hitbox();

private:
	std::vector<CollisionData> mCollisionData;
};

