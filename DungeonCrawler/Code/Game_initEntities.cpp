#include "Game.h"

extern Coordinator gCoordinator;
Entity gPlayerEntity;

// systems forward declaration
extern std::shared_ptr<SysDirectionPlayer> sysDirectionPlayer;
extern std::shared_ptr<SysTower> sysTower;

void Game::initEntities()
{
	GeomRectangle playerHitbox_Rect{ 16.0f, 16.0f, 32.0f, 32.0f };
	GeomCircle playerHitbox_Circ{ 16.0f, 16.0f, 16.0f };
	GeomRectangle torch_Hitbox{ 8.0f, 8.0f, 16.0f, 16.0f };

	gPlayerEntity = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Position>(gPlayerEntity, Position{ Vector2D<float>(400.0f, 300.0f) });
		gCoordinator.AddComponent<Texture>(gPlayerEntity, Texture{ "Picture/Player.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsPlayer>(gPlayerEntity, IsPlayer{});
		gCoordinator.AddComponent<InputKeys>(gPlayerEntity, InputKeys{});
		//gCoordinator.AddComponent<Movement>(gPlayerEntity, Movement{ 156.0f });
		gCoordinator.AddComponent<MovementTile>(gPlayerEntity, MovementTile{ 250.0f });
		gCoordinator.AddComponent<Affiliation>(gPlayerEntity, Affiliation{ eAffKind::Player });
		gCoordinator.AddComponent<DirectionDecision>(gPlayerEntity, DirectionDecision{});
		gCoordinator.AddComponent<FaceDirection>(gPlayerEntity, FaceDirection{});
		//gCoordinator.AddComponent<Hitbox>(gPlayerEntity, Hitbox{
			//std::make_shared<GeomRectangle>(playerHitbox_Rect), eCollisionType::PHYSICAL, "Picture/FullPinkEdge.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<Hitbox>(gPlayerEntity, Hitbox{
			std::make_shared<GeomCircle>(playerHitbox_Circ), eCollisionType::PHYSICAL, "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsCollision>(gPlayerEntity, IsCollision{ true });
		gCoordinator.AddComponent<Mass>(gPlayerEntity, Mass{ 50 });
	}


	//create_Mouse(Vector2D<float>(400.0f, 150.0f));

	Entity frameRate_UI = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<IsUI>(frameRate_UI, IsUI{});
		gCoordinator.AddComponent<Position>(frameRate_UI, Position{ Vector2D<float>(710.0f, 5.0f) });
		gCoordinator.AddComponent<Text>(frameRate_UI, Text{ "60", 25, 25, ColorValues{0, 255, 0} });
		gCoordinator.AddComponent<DisplayFPS>(frameRate_UI, DisplayFPS{});
	}




	sysTower->init();
	sysTower->syncPlayer();
	sysDirectionPlayer->makeMarker();
}