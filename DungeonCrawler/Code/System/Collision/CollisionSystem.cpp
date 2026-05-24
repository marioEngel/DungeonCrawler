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
		auto& entity = *it;
		auto& checkCollision = gCoordinator.GetComponent<IsCollision>(entity);

		if (checkCollision.checkCollision) {
			auto& originComp_Hitbox = gCoordinator.GetComponent<Comp_Hitbox>(entity);
			auto& originPosition = gCoordinator.GetComponent<Position>(entity);
			SDL_FRect originAABB = originComp_Hitbox.geomComp_Hitbox->return_AABB(originPosition.pos);

			for (size_t j = i + 1; j < mEntities.size(); ++j) 
			{
				auto it2 = std::next(mEntities.begin(), j);
				auto& otherEntity = *it2;
				auto& targetComp_Hitbox = gCoordinator.GetComponent<Comp_Hitbox>(otherEntity);
				auto& targetPosition = gCoordinator.GetComponent<Position>(otherEntity);
				SDL_FRect targetAABB = targetComp_Hitbox.geomComp_Hitbox->return_AABB(targetPosition.pos);

				if (check_RectVsRect(originAABB, targetAABB))
				{
					mCollisionData.push_back(CollisionData{ std::array<Entity, 2> {entity, otherEntity}, Vector2D<float> {}, true });
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
		auto& originComp_Hitbox = gCoordinator.GetComponent<Comp_Hitbox>(mCollisionData[i].collisionPair[0]);
		auto& originPosition = gCoordinator.GetComponent<Position>(mCollisionData[i].collisionPair[0]);
		auto& origionMovement = gCoordinator.GetComponent<Movement>(mCollisionData[i].collisionPair[0]);

		auto& targetComp_Hitbox = gCoordinator.GetComponent<Comp_Hitbox>(mCollisionData[i].collisionPair[1]);
		auto& targetPosition = gCoordinator.GetComponent<Position>(mCollisionData[i].collisionPair[1]);
		auto& targetMovement = gCoordinator.GetComponent<Movement>(mCollisionData[i].collisionPair[1]);

		CollisionDataSub tmpCollisionData = check_Geometry_arbitrary(
			*originComp_Hitbox.geomComp_Hitbox, originPosition.pos, origionMovement,
			*targetComp_Hitbox.geomComp_Hitbox, targetPosition.pos, targetMovement
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
	for (size_t i = 0; i < mCollisionData.size(); i++)
	{
		auto& originPosition = gCoordinator.GetComponent<Position>(mCollisionData[i].collisionPair[0]);

		originPosition.pos = originPosition.pos + mCollisionData[i].normDirection; //+ directionNorm(mCollisionData[i].normDirection);
	}

	mCollisionData.clear();
}

bool CollisionSystem::check_specificComp_Hitbox(Entity& entity)
{
	auto& originComp_Hitbox = gCoordinator.GetComponent<Comp_Hitbox>(entity);
	auto& originPosition = gCoordinator.GetComponent<Position>(entity);
	SDL_FRect originAABB = originComp_Hitbox.geomComp_Hitbox->return_AABB(originPosition.pos);

	for (auto& const objectEntity : mEntities)
	{
		SDL_FRect originAABB = originComp_Hitbox.geomComp_Hitbox->return_AABB(originPosition.pos);

			for (auto& const otherEntity : mEntities)
			{
				if (!(entity == otherEntity))
				{
					auto& targetComp_Hitbox = gCoordinator.GetComponent<Comp_Hitbox>(otherEntity);
					auto& targetPosition = gCoordinator.GetComponent<Position>(otherEntity);
					SDL_FRect targetAABB = targetComp_Hitbox.geomComp_Hitbox->return_AABB(targetPosition.pos);

					if (check_RectVsRect(originAABB, targetAABB))
					{
						mCollisionData.push_back(CollisionData{ std::array<Entity, 2> {entity, otherEntity}, Vector2D<float> {}, true });
					}
				}
			}
	}
	return false;
}

void CollisionSystem::create_Comp_HitboxRender()
{
	for (auto& const entity : mEntities)
	{
		auto& originComp_Hitbox = gCoordinator.GetComponent<Comp_Hitbox>(entity);

		if (originComp_Hitbox.textureSDL == nullptr)
		{
			originComp_Hitbox.textureSDL = LoadTexture(originComp_Hitbox.path);
		}
	}
}

void CollisionSystem::render_Comp_Hitbox()
{
	for (auto& const entity : mEntities)
	{
		auto& texture = gCoordinator.GetComponent<Comp_Hitbox>(entity);
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
