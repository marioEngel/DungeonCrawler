#pragma once
#include "ECS.h"
#include "System.h" 

class SystemManager
{
public:
	// register the system
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) == mSystems.end()
			&& "Registering system more than once.");

		auto system = std::make_shared<T>();
		mSystems.insert({ typeName, system });
		return system;
	}

	// set the systems signature
	template<typename T>
	void SetSigniture(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) != mSystems.end()
			&& "System used before registered.");

		mSignatures.insert({ typeName, signature });
	}

	// destroy entities not needed any more
	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;

			system->mEntities.erase(entity);
		}
	}

	// update every system if the signature of an entity changed
	void EntitySignatureChanged(Entity entity, Signature entitySignature)
	{
		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			else
			{
				system->mEntities.erase(entity);
			}
		}
	}

private:
	// map from name to signature
	std::unordered_map<const char*, Signature> mSignatures{};
	// map from name to system pointer
	std::unordered_map<const char*, std::shared_ptr<Systems>> mSystems{};
};
