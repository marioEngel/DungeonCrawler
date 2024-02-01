#pragma once
#include "ECS.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"


class Coordinator
{
public:
	// create pointers to each manager
	void Init()
	{
		mComponentManager = std::make_unique<ComponentManager>();
		mEntityManager = std::make_unique<EntityManager>();
		mSystemManager = std::make_unique<SystemManager>();
	}

	// the same methods for each manager

	// entity methods
	Entity CreateEntity()
	{
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(Entity entity)
	{
		mEntityManager->DestroyEntity(entity);
		mComponentManager->EntityDestroyed(entity);
		mSystemManager->EntityDestroyed(entity);
	}

	// component methods
	template<typename T>
	void RegisterComponent()
	{
		mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(Entity entity, T component)
	{
		mComponentManager->AddComponent<T>(entity, component);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(Entity entity, T component)
	{
		mComponentManager->RemoveComponent<T>(entity, component);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponent<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(Entity entity)
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}

	// system methods
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return mSystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSigniture<T>(signature);
	}

private:
	std::unique_ptr<ComponentManager> mComponentManager{};
	std::unique_ptr<EntityManager> mEntityManager{};
	std::unique_ptr<SystemManager> mSystemManager{};
};
