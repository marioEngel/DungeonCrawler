// ----------Rest----------------------
#include <map>
#include "Game.h"
#include "System/KeyInput/KeyboardInput.h"
#include "System/Dungeon/Dungeon.h"
// ---------Components------------------ 
#include "Component/AffiliationComp.h"
#include "Component/FaceDirection.h"
#include "Component/Hitbox.h"
#include "Component/InputComp.h"
#include "Component/InputComp.h"
#include "Component/IsPlayerComp.h"
#include "Component/IsObject.h"
#include "Component/IsCollision.h"
#include "Component/IsLight.h"
#include "Component/MovementComp.h"
#include "Component/MoveDecisionComp.h"
#include "Component/PositionComp.h"
#include "Component/TextureComp.h"
#include "Component/TextureLightComp.h"
#include "Component/TileMapComp.h"
#include "Component/AttachedToComp.h"
// --------Systems---------------------- 
#include "System/Collision/CollisionSystem.h"
#include "System/Movement/DirectionPlayer.h"
#include "System/Movement/MovementPlayer.h"
#include "System/Movement/MovementPlayer_decision.h"
#include "System/Movement/MovementObject.h"
#include "System/Movement/MovementObject_attached.h"
#include "System/Render/Renderer_Sprite.h"
#include "System/Render/Renderer_TileMap.h"
#include "System/Render/Renderer_Light.h"

SDL_Renderer* Game::renderer = NULL;
SDL_Event Game::event;
int Game::screenWidth;
int Game::screenHeight;

Coordinator gCoordinator;
KeyboardInput gKeyboardInput;
DungeonSystem gDungeonSystem;

// systems forward declaration
std::shared_ptr<RendererSpriteSystem> rendererSpriteSystem;
std::shared_ptr<RendererTileMapSystem> rendererTileMapSystem;
std::shared_ptr<RendererLightSystem> rendererLightSystem;
std::shared_ptr<MovementPlayerSystem> movementPlayerSystem;
std::shared_ptr<MovementPlayer_decisionSystem> movementPlayer_DecisionSystem;
std::shared_ptr<DirectionPlayerSystem> directionPlayerSystem;
std::shared_ptr<MovementObjectSystem> movementObjectSystem;
std::shared_ptr<CollisionSystem> collisionSystem;
std::shared_ptr<MovementObject_attachedSystem> movementObject_attachedSystem;

Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char* text, int width, int height, int flag)
{
	// init SDL system
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		window = SDL_CreateWindow(text, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flag);
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
	}
	screenWidth = width;
	screenHeight = height;

	gCoordinator.Init();

	// register all components, alphabetical order
	gCoordinator.RegisterComponent<Affiliation>();
	gCoordinator.RegisterComponent<FaceDirection>();
	gCoordinator.RegisterComponent<Hitbox>();
	gCoordinator.RegisterComponent<IsCollision>();
	gCoordinator.RegisterComponent<IsPlayer>();
	gCoordinator.RegisterComponent<IsObject>();
	gCoordinator.RegisterComponent<IsLight>();
	gCoordinator.RegisterComponent<InputKeys>();
	gCoordinator.RegisterComponent<Movement>();
	gCoordinator.RegisterComponent<MoveDecision>();
	gCoordinator.RegisterComponent<Position>();
	gCoordinator.RegisterComponent<Texture>();
	gCoordinator.RegisterComponent<TileMap>();
	gCoordinator.RegisterComponent<TextureLight>();
	gCoordinator.RegisterComponent<AttachedTo>();


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
	rendererLightSystem = gCoordinator.RegisterSystem<RendererLightSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<IsLight>());
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<TextureLight>());
		gCoordinator.SetSystemSignature<RendererLightSystem>(signature);
	}
	movementPlayerSystem = gCoordinator.RegisterSystem<MovementPlayerSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<MoveDecision>());
		signature.set(gCoordinator.GetComponentType<Movement>());
		signature.set(gCoordinator.GetComponentType<IsPlayer>());
		gCoordinator.SetSystemSignature<MovementPlayerSystem>(signature);
	}
	movementPlayer_DecisionSystem = gCoordinator.RegisterSystem<MovementPlayer_decisionSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<InputKeys>());
		signature.set(gCoordinator.GetComponentType<MoveDecision>());
		signature.set(gCoordinator.GetComponentType<IsPlayer>());
		gCoordinator.SetSystemSignature<MovementPlayer_decisionSystem>(signature);
	}
	directionPlayerSystem = gCoordinator.RegisterSystem<DirectionPlayerSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<FaceDirection>());
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<IsPlayer>());
		gCoordinator.SetSystemSignature<DirectionPlayerSystem>(signature);
	}
	movementObject_attachedSystem = gCoordinator.RegisterSystem< MovementObject_attachedSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<AttachedTo>());
		signature.set(gCoordinator.GetComponentType<Position>());
		gCoordinator.SetSystemSignature<MovementObject_attachedSystem>(signature);
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
		signature.set(gCoordinator.GetComponentType<Movement>());
		signature.set(gCoordinator.GetComponentType<IsCollision>());
		gCoordinator.SetSystemSignature<CollisionSystem>(signature);
	}
}
