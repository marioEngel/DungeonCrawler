#pragma once
#include "ECS.h"
#include "ComponentArray.h"

class ComponentManager
{
public:
	// register component
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) == mComponentTypes.end()
			&& "Registering component type more than once.");

		mComponentTypes.insert({ typeName, mNextComponentType });
		mComponentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });
		++mNextComponentType;
	}

	// returns index, used for signature
	template<typename T>
	ComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end()
			&& "Component not registered before use.");

		return mComponentTypes[typeName];
	}

	// add component to entity
	template<typename T>
	void AddComponent(Entity entitiy, T component)
	{
		GetComponentArray<T>()->InsertData(entitiy, component);
	}

	// removes component of entity
	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}

	// gets component of array
	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	template<typename T>
	bool CheckComponent(Entity entity)
	{
		return GetComponentArray<T>()->CheckComponent(entity);
	}

	// notifize every componentarray
	// delete entity if it has it
	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}

private:
	// map with name and index
	std::unordered_map<const char*, ComponentType> mComponentTypes{};
	// map with name and pointer to IComponentArray
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays{};

	ComponentType mNextComponentType{};

	template<typename T>
	std::shared_ptr<ComponentArray<T>>  GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		return std::static_pointer_cast<ComponentArray<T>>(mComponentArrays[typeName]);
	}
};
