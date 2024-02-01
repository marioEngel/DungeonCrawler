#include <map>
// ----------Rest----------------------
#include "Game.h"
#include "System/KeyInput/KeyboardInput.h"
#include "System/Dungeon/Dungeon.h"
// ---------Components------------------ 
#include "Component/TextureComp.h"
#include "Component/PositionComp.h"
#include "Component/TileMapComp.h"
#include "Component/InputComp.h"
#include "Component/InputComp.h"
#include "Component/IsPlayerComp.h"
#include "Component/MovementComp.h"
#include "Component/AffiliationComp.h"
#include "Component/MoveDecisionComp.h"
#include "Component/FaceDirection.h"
#include "Component/IsObject.h"
#include "Component/Hitbox.h"
#include "Component/IsCollision.h"
// --------Systems---------------------- 
#include "System/Render/RendererSprite.h"
#include "System/Render/RendererTileMap.h"
#include "System/Movement.h"
#include "System/MovementDecision.h"
#include "System/Direction.h"
#include "System/MoveExceptPlayer/MovementObject.h"
#include "System/Collision/CollisionSystem.h"

SDL_Renderer* Game::renderer = NULL;
SDL_Event Game::event;
Coordinator gCoordinator;
KeyboardInput gKeyboardInput;
DungeonSystem gDungeonSystem;

// systems forward declaration
std::shared_ptr<RendererSpriteSystem> rendererSpriteSystem;
std::shared_ptr<RendererTileMapSystem> rendererTileMapSystem;
std::shared_ptr<MovementSystem> movementSystem;
std::shared_ptr<MovementDecisionSystem> movementDecisionSystem;
std::shared_ptr<DirectionSystem> directionSystem;
std::shared_ptr<MovementObjectSystem> movementObjectSystem;
std::shared_ptr<CollisionSystem> collisionSystem;

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char* text, int width, int hight, int flag)
{
	// init SDL system
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(text, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, hight, flag);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
	}


	gCoordinator.Init();

	// register all components, alphabetical order
	gCoordinator.RegisterComponent<Texture>();
	gCoordinator.RegisterComponent<Position>();
	gCoordinator.RegisterComponent<TileMap>();
	gCoordinator.RegisterComponent<InputKeys>();
	gCoordinator.RegisterComponent<IsPlayer>();
	gCoordinator.RegisterComponent<Movement>();
	gCoordinator.RegisterComponent<Affiliation>();
	gCoordinator.RegisterComponent<MoveDecision>();
	gCoordinator.RegisterComponent<FaceDirection>();
	gCoordinator.RegisterComponent<IsObject>();
	gCoordinator.RegisterComponent<Hitbox>();
	gCoordinator.RegisterComponent<isCollision>();


	// register all systems, forward declaration, signature
	rendererSpriteSystem = gCoordinator.RegisterSystem<RendererSpriteSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Texture>());
		signature.set(gCoordinator.GetComponentType<Position>());
		gCoordinator.SetSystemSignature<RendererSpriteSystem>(signature);
	}
	rendererTileMapSystem = gCoordinator.RegisterSystem<RendererTileMapSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<TileMap>());
		gCoordinator.SetSystemSignature<RendererTileMapSystem>(signature);
	}
	movementSystem = gCoordinator.RegisterSystem<MovementSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<MoveDecision>());
		signature.set(gCoordinator.GetComponentType<Movement>());
		gCoordinator.SetSystemSignature<MovementSystem>(signature);
	}
	movementDecisionSystem = gCoordinator.RegisterSystem<MovementDecisionSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<InputKeys>());
		signature.set(gCoordinator.GetComponentType<MoveDecision>());
		gCoordinator.SetSystemSignature<MovementDecisionSystem>(signature);
	}
	directionSystem = gCoordinator.RegisterSystem<DirectionSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<FaceDirection>());
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<IsPlayer>());
		gCoordinator.SetSystemSignature<DirectionSystem>(signature);
	}
	movementObjectSystem = gCoordinator.RegisterSystem<MovementObjectSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<FaceDirection>());
		signature.set(gCoordinator.GetComponentType<Movement>());
		signature.set(gCoordinator.GetComponentType<IsObject>());
		gCoordinator.SetSystemSignature<MovementObjectSystem>(signature);
	}
	collisionSystem = gCoordinator.RegisterSystem<CollisionSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<Hitbox>());
		signature.set(gCoordinator.GetComponentType<isCollision>());
		gCoordinator.SetSystemSignature<CollisionSystem>(signature);
	}
}
