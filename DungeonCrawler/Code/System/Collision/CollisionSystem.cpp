#include "CollisionSystem.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/Is_Collision.h"
#include "../../Component/Comp_Position.h"
#include "../../Component/Comp_Hitbox.h"
#include "../../Component/Comp_Movement.h"
#include "Collision.h"
#include "../Render/TextureFunc.h"
#include "../Render/Camera.h"
#include "../../Game.h"
#include <iterator>
#include "../../Component/Comp_Mass.h"

extern Coordinator gCoordinator;
extern Camera gCamera;

// detecting collisions and responding to them
// more effectig grid based approach n*log(n)
//  currently n(n-1)/2
void CollisionSystem::check_AABB()
{
	for (size_t i = 0; i < mEntities.size(); i++)
	{
		auto it = std::next(mEntities.begin(), i);
		auto& originEntity = *it;
		auto& checkCollision = gCoordinator.GetComponent<IsCollision>(originEntity);

		if (checkCollision.checkCollision) {
			auto& originHitbox = gCoordinator.GetComponent<Hitbox>(originEntity);
			auto& originPosition = gCoordinator.GetComponent<Position>(originEntity);
			SDL_FRect originAABB = originHitbox.geomHitbox->return_AABB(originPosition.pos);

			for (size_t j = i + 1; j < mEntities.size(); ++j) 
			{
				auto it2 = std::next(mEntities.begin(), j);
				auto& targetEntity = *it2;
				auto& targetHitbox = gCoordinator.GetComponent<Hitbox>(targetEntity);
				auto& targetPosition = gCoordinator.GetComponent<Position>(targetEntity);
				SDL_FRect targetAABB = targetHitbox.geomHitbox->return_AABB(targetPosition.pos);

				if (check_RectVsRect(originAABB, targetAABB))
				{
					mCollisionData.push_back(CollisionData{ std::array<Entity, 2> {originEntity, targetEntity}, Contact{ Vector2D<float> { }, 0.0f, true} });
				}
			}
		}
	}
}

void CollisionSystem::check_General()
{
	std::vector<CollisionData> tmpData{};

	for (size_t i = 0; i < mCollisionData.size(); i++)
	{
		Entity originEnt = mCollisionData[i].collisionPair[0];
		Entity targEnt = mCollisionData[i].collisionPair[1];

		auto& originHitbox = gCoordinator.GetComponent<Hitbox>(originEnt);
		auto& originPosition = gCoordinator.GetComponent<Position>(originEnt);
		auto& origionMovement = gCoordinator.GetComponent<Movement>(originEnt);

		auto& targetHitbox = gCoordinator.GetComponent<Hitbox>(targEnt);
		auto& targetPosition = gCoordinator.GetComponent<Position>(targEnt);
		auto& targetMovement = gCoordinator.GetComponent<Movement>(targEnt);

		Contact tmpCollisionData = check_Geometry_arbitrary(
			*originHitbox.geomHitbox, originPosition.pos, origionMovement,
			*targetHitbox.geomHitbox, targetPosition.pos, targetMovement
		);

		if (tmpCollisionData.valid)
		{
			tmpData.push_back(CollisionData{ std::array<Entity, 2>{originEnt, targEnt},	tmpCollisionData });
		}
	}

	mCollisionData.clear();
	mCollisionData = tmpData;
}

void CollisionSystem::react()
{
	for (size_t i = 0; i < mCollisionData.size(); i++)
	{
		auto& originPosition = gCoordinator.GetComponent<Position>(mCollisionData[i].collisionPair[0]);
		auto& originMass = gCoordinator.GetComponent<Mass>(mCollisionData[i].collisionPair[0]);
		auto& targetPosition = gCoordinator.GetComponent<Position>(mCollisionData[i].collisionPair[1]);
		auto& targetMass = gCoordinator.GetComponent<Mass>(mCollisionData[i].collisionPair[1]);

		float originWeight{};
		float targetWeight{};

		if (originMass.unmoveable)
		{
			targetPosition.pos = targetPosition.pos + mCollisionData[i].contact.normal * mCollisionData[i].contact.penetration;
		}
		else if (targetMass.unmoveable)
		{
			originPosition.pos = originPosition.pos - mCollisionData[i].contact.normal * mCollisionData[i].contact.penetration;
		}
		else
		{
			originWeight = originMass.mass / (originMass.mass + targetMass.mass);
			targetWeight = targetMass.mass / (originMass.mass + targetMass.mass);

			originPosition.pos = originPosition.pos - originWeight * mCollisionData[i].contact.normal * mCollisionData[i].contact.penetration;
			targetPosition.pos = targetPosition.pos + targetWeight * mCollisionData[i].contact.normal * mCollisionData[i].contact.penetration;
		}
	}
 
	mCollisionData.clear();
}

bool CollisionSystem::check_specificHitbox(Entity& entity)
{
	auto& originHitbox = gCoordinator.GetComponent<Hitbox>(entity);
	auto& originPosition = gCoordinator.GetComponent<Position>(entity);
	SDL_FRect originAABB = originHitbox.geomHitbox->return_AABB(originPosition.pos);

	for (auto& const objectEntity : mEntities)
	{
		SDL_FRect originAABB = originHitbox.geomHitbox->return_AABB(originPosition.pos);

			for (auto& const otherEntity : mEntities)
			{
				if (!(entity == otherEntity))
				{
					auto& targetHitbox = gCoordinator.GetComponent<Hitbox>(otherEntity);
					auto& targetPosition = gCoordinator.GetComponent<Position>(otherEntity);
					SDL_FRect targetAABB = targetHitbox.geomHitbox->return_AABB(targetPosition.pos);

					if (check_RectVsRect(originAABB, targetAABB))
					{
						mCollisionData.push_back(CollisionData{ std::array<Entity, 2> {entity, otherEntity}, Contact{ Vector2D<float> { }, 0.0f, true} });
					}
				}
			}
	}
	return false;
}

void CollisionSystem::create_HitboxRender()
{
	for (auto& const entity : mEntities)
	{
		auto& originHitbox = gCoordinator.GetComponent<Hitbox>(entity);

		if (originHitbox.textureSDL == nullptr)
		{
			originHitbox.textureSDL = LoadTexture(originHitbox.path);
		}
	}
}

void CollisionSystem::render_Hitbox()
{
	for (auto& const entity : mEntities)
	{
		auto& texture = gCoordinator.GetComponent<Hitbox>(entity);
		auto& position = gCoordinator.GetComponent<Position>(entity);

		if (texture.textureSDL != nullptr)
		{
			SDL_FRect srcRec;
			SDL_FRect destRec;

			srcRec.x = 0;
			srcRec.y = 0;
			srcRec.w = texture.textureWidth;
			srcRec.h = texture.textureHeight;

			destRec.x = position.pos[0];
			destRec.y = position.pos[1];
			destRec.w = texture.textureWidth * texture.scale;
			destRec.h = texture.textureHeight * texture.scale;

			if (check_RectVsRect(gCamera.mCamera, destRec))
			{
				destRec.x -= gCamera.mCamera.x;
				destRec.y -= gCamera.mCamera.y;
				 SDL_RenderTextureRotated(Game::renderer, texture.textureSDL, &srcRec, &destRec, texture.angle, NULL, SDL_FLIP_NONE);
			}
		}
	}
}
