#include <map>
// ----------Rest----------------------
#include "Game.h"
#include "System/Render/TextureFunc.h"
#include "Math/Matrix.h"
#include "System/KeyInput/KeyboardInput.h"
#include "System/Collision/Geometry/Geometry.h"
#include "System/Collision/Geometry/Rectangle.h"
#include "System/Collision/Geometry/Circle.h"
// ---------Components------------------ 15/32
#include "Component/PositionComp.h"
#include "Component/TextureComp.h"
#include "Component/TileMapComp.h"
#include "Component/IsPlayerComp.h"
#include "Component/InputComp.h"
#include "Component/MovementComp.h"
#include "Component/AffiliationComp.h"
#include "Component/FaceDirection.h"
#include "Component/MoveDecisionComp.h"
#include "Component/Hitbox.h"
#include "Component/IsCollision.h"
// --------Systems---------------------- 15
#include "System/Render/RendererSprite.h"
#include "System/Render/RendererTileMap.h"
#include "System/Movement.h"
#include "System/MovementDecision.h"
#include "System/Dungeon/Dungeon.h"
#include "System/Direction.h"
#include "System/MoveExceptPlayer/MovementObject.h"
#include "System/Render/Camera.h"
#include "System/Collision/CollisionSystem.h"

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;
Entity gPlayerEntity;
Camera gCamera;

SDL_Texture* light_texutre;
SDL_Texture* rendertex_light;
SDL_Rect light_rect = SDL_Rect{ 0,0,800,600 };

// systems forward declaration
extern std::shared_ptr<RendererSpriteSystem> rendererSpriteSystem;
extern std::shared_ptr<RendererTileMapSystem> rendererTileMapSystem;
extern std::shared_ptr<MovementSystem> movementSystem;
extern std::shared_ptr<MovementDecisionSystem> movementDecisionSystem;
extern std::shared_ptr<DirectionSystem> directionSystem;
extern std::shared_ptr<MovementObjectSystem> movementObjectSystem;
extern std::shared_ptr<CollisionSystem> collisionSystem;

// is game running? bool
bool Game::gameRunning()
{
	return isRunning;
}

void Game::initEntities()
{
	gPlayerEntity = gCoordinator.CreateEntity();
	gCoordinator.AddComponent<Position>(gPlayerEntity, Position{ Vector2D<float>(400.0f, 300.0f) });
	gCoordinator.AddComponent<Texture>(gPlayerEntity, Texture{ "Picture/Player.png", 32, 32, 1, 0.0 });
	gCoordinator.AddComponent<IsPlayer>(gPlayerEntity, IsPlayer{});
	gCoordinator.AddComponent<InputKeys>(gPlayerEntity, InputKeys{});
	gCoordinator.AddComponent<Movement>(gPlayerEntity, Movement{});
	gCoordinator.AddComponent<Affiliation>(gPlayerEntity, Affiliation{ eAffKind::Player });
	gCoordinator.AddComponent<MoveDecision>(gPlayerEntity, MoveDecision{});
	gCoordinator.AddComponent<FaceDirection>(gPlayerEntity, FaceDirection{});
	GeomRectangle playerHitbox{ 16.0f, 16.0f, 32.0f, 32.0f };
	GeomCircle playerHitbox2{ 16.0f, 16.0f, 16.0f };
	//gCoordinator.AddComponent<Hitbox>(gPlayerEntity, Hitbox{std::make_shared<GeomRectangle>(playerHitbox),  "Picture/FullPinkEdge.png", 32, 32, 1, 0.0 });
	gCoordinator.AddComponent<Hitbox>(gPlayerEntity, 
		Hitbox{ std::make_shared<GeomCircle>(playerHitbox2),  "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
	gCoordinator.AddComponent<isCollision>(gPlayerEntity, isCollision{ true });
	
	Entity testHitbox = gCoordinator.CreateEntity();
	gCoordinator.AddComponent<Position>(testHitbox, Position{ Vector2D<float>(400.0f, 150.0f) });
	gCoordinator.AddComponent<Texture>(testHitbox, Texture{ "Picture/Ratte.png", 32, 32, 1, 0.0 });
	gCoordinator.AddComponent<Hitbox>(testHitbox, Hitbox{ std::make_shared<GeomRectangle>(playerHitbox),  "Picture/FullPinkEdge.png", 32, 32, 1, 0.0 });
	gCoordinator.AddComponent<isCollision>(testHitbox, isCollision{ false });

	GeomCircle tmpCircle{ 16.0f, 16.0f, 16.0f };
	Entity testHitbox2 = gCoordinator.CreateEntity();
	gCoordinator.AddComponent<Position>(testHitbox2, Position{ Vector2D<float>(500.0f, 150.0f) });
	gCoordinator.AddComponent<Texture>(testHitbox2, Texture{ "Picture/Ratte.png", 32, 32, 1, 0.0 });
	gCoordinator.AddComponent<Hitbox>(testHitbox2, Hitbox{ std::make_shared<GeomCircle>(tmpCircle),  "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
	gCoordinator.AddComponent<isCollision>(testHitbox2, isCollision{ false });

	//Entity testHitbox3 = gCoordinator.CreateEntity();
	//gCoordinator.AddComponent<Position>(testHitbox3, Position{ Vector2D<float>(465.0f, 150.0f) });
	//gCoordinator.AddComponent<Texture>(testHitbox3, Texture{ "Picture/Ratte.png", 32, 32, 1, 0.0 });
	//gCoordinator.AddComponent<Hitbox>(testHitbox3, Hitbox{ std::make_shared<GeomCircle>(tmpCircle),  "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
	//gCoordinator.AddComponent<isCollision>(testHitbox3, isCollision{ false });

	DungeonSystem test;
	test.init();
	directionSystem->makeMarker();

	/* // time testing
	std::vector<int> timeVec;
	int length = 10000;
	int startTime;
	int endTime;

	for (int i = 0; i < length; i++)
	{
		startTime = SDL_GetTicks();
		levelSystem->init2();
		endTime = SDL_GetTicks();

		timeVec.push_back(endTime - startTime);
		std::cout << i << std::endl;
	}

	std::cout << "Min element: " << *std::min_element(timeVec.begin(), timeVec.end()) << '\n';
	std::cout << "Max element: " << *std::max_element(timeVec.begin(), timeVec.end()) << '\n';
	std::cout << "Median: " << std::accumulate(timeVec.begin(), timeVec.end(), 0.0) / float(length) << '\n';
	*/

	light_texutre = LoadTexture("Picture/light.png");
	SDL_SetTextureBlendMode(light_texutre, SDL_BLENDMODE_ADD);


	rendertex_light = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 800, 600);
	SDL_SetTextureBlendMode(rendertex_light, SDL_BLENDMODE_MOD);
}

void Game::update()
{
	//std::cout << "update\n";

	gKeyboardInput.registerInput(); 
	movementDecisionSystem->update();

	movementSystem->update();
	directionSystem->update();

	movementObjectSystem->update();
	
	collisionSystem->check_AABB();
	collisionSystem->check_General();
	collisionSystem->react();

	gCamera.CheckCollision(gPlayerEntity);

	SDL_SetRenderTarget(renderer, rendertex_light);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, light_texutre, NULL, &light_rect);

	rendererTileMapSystem->loadTexture();
	rendererTileMapSystem->renderTileMap();

	rendererSpriteSystem->loadTexture();
	rendererSpriteSystem->renderSprite();

	collisionSystem->create_HitboxRender();
	collisionSystem->render_Hitbox();

	gKeyboardInput.getPrevKeys();
	drawAndClear();
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


