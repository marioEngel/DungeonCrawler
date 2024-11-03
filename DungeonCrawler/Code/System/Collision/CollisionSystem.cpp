#include "CollisionSystem.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/IsCollision.h"
#include "../../Component/PositionComp.h"
#include "../../Component/Hitbox.h"
#include "../../Component/MovementComp.h"
#include "Collision.h"
#include "../Render/TextureFunc.h"
#include "../Render/Camera.h"
#include "../../Game.h"

extern Coordinator gCoordinator;
extern Camera gCamera;

// detecting collisions and responding to them

void CollisionSystem::check_AABB()
{
	for (auto& const entity : mEntities)
	{
		auto& checkCollision = gCoordinator.GetComponent<IsCollision>(entity);

		if (checkCollision.checkCollision)
		{
			auto& originHitbox = gCoordinator.GetComponent<Hitbox>(entity);
			auto& originPosition = gCoordinator.GetComponent<Position>(entity);
			
			SDL_Rect originAABB = originHitbox.geomHitbox->return_AABB(originPosition.pos);

			for (auto& const otherEntity : mEntities)
			{
				if (!(entity == otherEntity))
				{
					auto& targetHitbox = gCoordinator.GetComponent<Hitbox>(otherEntity);
					auto& targetPosition = gCoordinator.GetComponent<Position>(otherEntity);
					SDL_Rect targetAABB = targetHitbox.geomHitbox->return_AABB(targetPosition.pos);

					if (check_RectVsRect(originAABB, targetAABB))
					{
						mCollisionData.push_back(CollisionData{ std::array<Entity, 2> {entity, otherEntity}, Vector2D<float> {}, true });
					}
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
		auto& originHitbox = gCoordinator.GetComponent<Hitbox>(mCollisionData[i].collisionPair[0]);
		auto& originPosition = gCoordinator.GetComponent<Position>(mCollisionData[i].collisionPair[0]);
		auto& origionMovement = gCoordinator.GetComponent<Movement>(mCollisionData[i].collisionPair[0]);

		auto& targetHitbox = gCoordinator.GetComponent<Hitbox>(mCollisionData[i].collisionPair[1]);
		auto& targetPosition = gCoordinator.GetComponent<Position>(mCollisionData[i].collisionPair[1]);
		auto& targetMovement = gCoordinator.GetComponent<Movement>(mCollisionData[i].collisionPair[1]);

		CollisionDataSub tmpCollisionData = check_Geometry_arbitrary(
			*originHitbox.geomHitbox, originPosition.pos, origionMovement,
			*targetHitbox.geomHitbox, targetPosition.pos, targetMovement
		);

		if (tmpCollisionData.collided)
		{
			tmpData.push_back(CollisionData{ mCollisionData[i].collisionPair, tmpCollisionData.normDirection, true });
		}
	}

	mCollisionData.clear();
	mCollisionData = tmpData;
}

void CollisionSystem::react()
{
	//std::cout << mCollisionData.size() << '\n';
	for (size_t i = 0; i < mCollisionData.size(); i++)
	{
		auto& originPosition = gCoordinator.GetComponent<Position>(mCollisionData[i].collisionPair[0]);

		originPosition.pos = originPosition.pos + mCollisionData[i].normDirection; //+ directionNorm(mCollisionData[i].normDirection);
	}

	mCollisionData.clear();
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
			SDL_Rect srcRec;
			SDL_Rect destRec;

			srcRec.x = 0;
			srcRec.y = 0;
			srcRec.w = texture.textureWidth;
			srcRec.h = texture.textureHeight;

			destRec.x = int(position.pos[0]);
			destRec.y = int(position.pos[1]);
			destRec.w = texture.textureWidth * texture.scale;
			destRec.h = texture.textureHeight * texture.scale;

			if (check_RectVsRect(gCamera.mCamera, destRec))
			{
				destRec.x -= gCamera.mCamera.x;
				destRec.y -= gCamera.mCamera.y;
				SDL_RenderCopyEx(Game::renderer, texture.textureSDL, &srcRec, &destRec, texture.angle, NULL, SDL_FLIP_NONE);
			}
		}
	}
}
