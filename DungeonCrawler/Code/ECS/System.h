#pragma once
#include "ECS.h"

// set is a list with only unique members
// searchin is much faster, but inserting is slower
class Systems
{
public:
	std::set<Entity> mEntities;
};

/* example for system:
for (auto const& entity : mEntites)
{
	auto& rigidBody = GetComponent<RigidBody>(entity);
	auto& transform = GetComponent<Transform>(entity);
	auto const& gravity = GetComponent<Gravity>(entity);

	transform.position += rigidBody * dt;
	rigidBody.velocity += gravity.force * dt;
}
*/
