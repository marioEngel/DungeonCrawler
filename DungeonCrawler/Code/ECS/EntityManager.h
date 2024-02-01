#pragma once
#include "ECS.h"

class EntityManager
{
public:
	// initialias queue with every possible entity
	// queue: get number from the front and put deleted numbers in the back
	EntityManager()
	{
		for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			mAvailableEntities.push(entity);
		}
	}

	// get the first number (front()) and delete the first number (pop())
	// add 1 to the count of living entities
	Entity CreateEntity()
	{
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

		Entity id = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntityCount;

		return id;
	}

	// resets the signature of the entity
	// puts the entity back in the available entities queue
	// living count minus 1
	void DestroyEntity(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		mSignatures[entity].reset();
		mAvailableEntities.push(entity);
		--mLivingEntityCount;
	}

	// set the signature of an entity
	void SetSignature(Entity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		mSignatures[entity] = signature;
	}

	// get the signature of an entity
	Signature GetSignature(Entity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		return mSignatures[entity];
	}

private:
	// Queue of unused entities IDs
	std::queue<Entity> mAvailableEntities{};

	// Array of signatures where the index corresponds to the entity ID 
	// signature is a bitset, which "tells" the entity what components it has
	std::array<Signature, MAX_ENTITIES> mSignatures{};

	// max entities
	Entity mLivingEntityCount{};
};
