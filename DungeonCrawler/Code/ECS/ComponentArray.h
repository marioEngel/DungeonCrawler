#pragma once
#include "ECS.h"

// needed for Componentmanager
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};


template<typename T>
class ComponentArray : public IComponentArray
{
public:
	// updating the two maps and adding 
	void InsertData(Entity entity, T component)
	{
		assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end()
			&& "Component added to same entity more than once.");

		size_t newIndex = mSize;
		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;
		mComponentArray[newIndex] = component;
		++mSize;
	}

	// removes entity and updates the maps
	void RemoveData(Entity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()
			&& "Removing non-existent component.");

		size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
		size_t indexOfLastElement = mSize - 1;
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

		Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(indexOfLastElement);

		--mSize;
	}

	// get components of entity
	T& GetData(Entity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end()
			&& "Retrieving non-existent component.");

		return mComponentArray[mEntityToIndexMap[entity]];
	}

	bool CheckComponent(Entity entity)
	{
		return (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end());
	}

	// destroys entity
	void EntityDestroyed(Entity entity) override
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
		{
			RemoveData(entity);
		}
	}

private:
	// array with one component of each entity 
	// max size = max entitiy count
	std::array<T, MAX_ENTITIES> mComponentArray{};
	// map from entity to index
	// unordnet_map: container with (key, element)
	std::unordered_map<Entity, size_t> mEntityToIndexMap{};
	// map from index to entity
	std::unordered_map<size_t, Entity> mIndexToEntityMap{};
	// size of map
	size_t mSize;
};
