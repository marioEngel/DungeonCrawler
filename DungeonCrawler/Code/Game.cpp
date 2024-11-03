// ----------Rest----------------------
#include "Game.h"
#include "System/Render/TextureFunc.h"
#include "Math/Matrix.h"
#include <map>
// ---------Components------------------ max 32
#include "Component/PositionComp.h"
#include "Component/TextureComp.h"
#include "Component/TileMapComp.h"
#include "Component/IsPlayerComp.h"
#include "Component/InputComp.h"			// 5
#include "Component/MovementComp.h"
#include "Component/AffiliationComp.h"
#include "Component/FaceDirection.h"
#include "Component/MoveDecisionComp.h"
#include "Component/Hitbox.h"				// 10
#include "Component/IsCollision.h"
#include "Component/IsLight.h"
#include "Component/TextureLightComp.h"
#include "Component/AttachedToComp.h"
// --------Systems---------------------- 
#include "System/Render/Renderer_Sprite.h"
#include "System/Render/Renderer_TileMap.h"
#include "System/Render/Renderer_Light.h"
#include "System/Render/Camera.h"
#include "System/Movement/DirectionPlayer.h"			// 5
#include "System/Movement/MovementPlayer.h"
#include "System/Movement/MovementPlayer_decision.h"
#include "System/Dungeon/Dungeon.h"
#include "System/Movement/MovementObject.h"				// 10
#include "System/KeyInput/KeyboardInput.h"
#include "System/Collision/CollisionSystem.h"				
#include "System/Collision/Geometry/Geometry.h"
#include "System/Collision/Geometry/Rectangle.h"
#include "System/Collision/Geometry/Circle.h"			// 15
#include "System/Movement/MovementObject_attached.h"

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;
Entity gPlayerEntity;
Camera gCamera;

// systems forward declaration
extern std::shared_ptr<RendererSpriteSystem> rendererSpriteSystem;
extern std::shared_ptr<RendererTileMapSystem> rendererTileMapSystem;
extern std::shared_ptr<RendererLightSystem> rendererLightSystem;
extern std::shared_ptr<MovementPlayerSystem> movementPlayerSystem;
extern std::shared_ptr<MovementPlayer_decisionSystem> movementPlayer_DecisionSystem;
extern std::shared_ptr<DirectionPlayerSystem> directionPlayerSystem;
extern std::shared_ptr<MovementObjectSystem> movementObjectSystem;
extern std::shared_ptr<CollisionSystem> collisionSystem;
extern std::shared_ptr<MovementObject_attachedSystem> movementObject_attachedSystem;

// is game running? bool
bool Game::gameRunning()
{
	return isRunning;
}

void Game::initEntities()
{
	GeomRectangle playerHitbox_Rect{ 16.0f, 16.0f, 32.0f, 32.0f };
	GeomCircle playerHitbox_Circ{ 16.0f, 16.0f, 16.0f };

	gPlayerEntity = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Position>(gPlayerEntity, Position{ Vector2D<float>(400.0f, 300.0f) });
		gCoordinator.AddComponent<Texture>(gPlayerEntity, Texture{ "Picture/Player.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsPlayer>(gPlayerEntity, IsPlayer{});
		gCoordinator.AddComponent<InputKeys>(gPlayerEntity, InputKeys{});
		gCoordinator.AddComponent<Movement>(gPlayerEntity, Movement{});
		gCoordinator.AddComponent<Affiliation>(gPlayerEntity, Affiliation{ eAffKind::Player });
		gCoordinator.AddComponent<MoveDecision>(gPlayerEntity, MoveDecision{});
		gCoordinator.AddComponent<FaceDirection>(gPlayerEntity, FaceDirection{});
		//gCoordinator.AddComponent<Hitbox>(gPlayerEntity, Hitbox{std::make_shared<GeomRectangle>(playerHitbox_Rect),  "Picture/FullPinkEdge.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<Hitbox>(gPlayerEntity, Hitbox{ std::make_shared<GeomCircle>(playerHitbox_Circ),  "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsCollision>(gPlayerEntity, IsCollision{ true });
	}
	
	Entity testHitbox = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Position>(testHitbox, Position{ Vector2D<float>(400.0f, 150.0f) });
		gCoordinator.AddComponent<Texture>(testHitbox, Texture{ "Picture/Ratte.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<Hitbox>(testHitbox, Hitbox{ std::make_shared<GeomRectangle>(playerHitbox_Rect),  "Picture/FullPinkEdge.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsCollision>(testHitbox, IsCollision{ false });
		gCoordinator.AddComponent<Movement>(testHitbox, Movement{ 0, Vector2D<float> {}, true });
	}
 
	Entity testHitbox2 = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Position>(testHitbox2, Position{ Vector2D<float>(500.0f, 150.0f) });
		gCoordinator.AddComponent<Texture>(testHitbox2, Texture{ "Picture/Ratte.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<Hitbox>(testHitbox2, Hitbox{ std::make_shared<GeomCircle>(playerHitbox_Circ),  "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsCollision>(testHitbox2, IsCollision{ false });
		gCoordinator.AddComponent<Movement>(testHitbox2, Movement{ 0, Vector2D<float> {}, true });
	}

	Entity lightSource = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Position>(lightSource, Position{ Vector2D<float>(40,-60) });
		gCoordinator.AddComponent<TextureLight>(lightSource, TextureLight{ "Picture/light.png", 720, 720, 1, 0, 0 });
		gCoordinator.AddComponent<IsLight>(lightSource, IsLight{ true });
		gCoordinator.AddComponent<AttachedTo>(lightSource, AttachedTo{ true, gPlayerEntity });
	}
	//Entity lightSource2 = gCoordinator.CreateEntity();
	//{
	//	gCoordinator.AddComponent<Position>(lightSource2, Position{ Vector2D<float>(100,50) });
	//	gCoordinator.AddComponent<TextureLight>(lightSource2, TextureLight{ "Picture/light.png", 720, 720, 1, 0, 0 });
	//	gCoordinator.AddComponent<IsLight>(lightSource2, IsLight{ true });
	//}

	//Entity testHitbox3 = gCoordinator.CreateEntity();
	//gCoordinator.AddComponent<Position>(testHitbox3, Position{ Vector2D<float>(465.0f, 150.0f) });
	//gCoordinator.AddComponent<Texture>(testHitbox3, TextureLight{ "Picture/Ratte.png", 32, 32, 1, 0.0 });
	//gCoordinator.AddComponent<Hitbox>(testHitbox3, Hitbox{ std::make_shared<GeomCircle>(tmpCircle),  "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
	//gCoordinator.AddComponent<isCollision>(testHitbox3, isCollision{ false });

	DungeonSystem test;
	test.init();
	directionPlayerSystem->makeMarker();

}

void Game::update()
{
	//std::cout << "update\n";

	gKeyboardInput.registerInput(); 

	movementPlayer_DecisionSystem->update();

	// movement pipeline
	{
		movementPlayerSystem->update();
		directionPlayerSystem->update();

		movementObjectSystem->update();

		movementObject_attachedSystem->update();
	}
	
	// collision pipeline
	{
		collisionSystem->check_AABB();
		collisionSystem->check_General();
		collisionSystem->react();
		
		gCamera.CheckCollision(gPlayerEntity);
	}


	// render pipeline
	{
		rendererTileMapSystem->loadTexture();
		rendererTileMapSystem->initRenderertex();
		rendererTileMapSystem->render();

		rendererSpriteSystem->loadTexture();
		rendererSpriteSystem->initRenderertex();
		rendererSpriteSystem->render();

		rendererLightSystem->loadTexture();
		rendererLightSystem->initRenderertex();
		rendererLightSystem->render();
	}

	// tmp for checking hitboxes not working
	//{
	//	collisionSystem->create_HitboxRender();
	//	collisionSystem->render_Hitbox();
	//}

	gKeyboardInput.getPrevKeys();
	drawAndClear(
		rendererTileMapSystem->rtnRenderertex(),
		rendererSpriteSystem->rtnRenderertex(),
		rendererLightSystem->rtnRenderertex()
	);
}

// is game closed? returning bool
void Game::handleEvents()
{
	gKeyboardInput.registerInput();

	switch (event.type)
	{
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void Game::makeEnd()
{
	isRunning = false;
}


