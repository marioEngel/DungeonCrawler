#include <map>
#include "Game.h"
#include <SDL3_ttf/SDL_ttf.h>

SDL_Renderer* Game::renderer = NULL;
SDL_Event Game::event;
int Game::screenWidth;
int Game::screenHeight;

Coordinator gCoordinator;
KeyboardInput gKeyboardInput;
DungeonSystem gDungeonSystem;

// systems forward declaration
std::shared_ptr<RendererSystem_Sprite> rendererSystem_Sprite;
std::shared_ptr<RendererSystem_TileMap> rendererSystem_TileMap;
std::shared_ptr<RendererSystem_Light> rendererSystem_Light;
std::shared_ptr<RendererSystem_UI> rendererSystem_UI;
std::shared_ptr<MovementPlayerSystem> movementPlayerSystem;
std::shared_ptr<MovementPlayer_decisionSystem> movementPlayer_DecisionSystem;
std::shared_ptr<DirectionPlayerSystem> directionPlayerSystem;
std::shared_ptr<MovementObjectSystem> movementObjectSystem;
std::shared_ptr<CollisionSystem> collisionSystem;
std::shared_ptr<MovementObject_attachedSystem> movementObject_attachedSystem;
std::shared_ptr<System_Display_FPS> system_Display_FPS;

Game::Game()
{
}

Game::~Game()
{
}

void Game::initECS(const char* text, int width, int height, int flag)
{
	// init SDL system
	if (
		   SDL_Init(SDL_INIT_VIDEO)
		&& SDL_Init(SDL_INIT_EVENTS)
		&& SDL_Init(SDL_INIT_AUDIO)
	)
	{
		window = SDL_CreateWindow(text, width, height, flag);
		renderer = SDL_CreateRenderer(window, NULL);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		}
	}
	else {
		std::cout << "Error : " << SDL_GetError() << std::endl;
	}
	SDL_StartTextInput(window);
	if (!TTF_Init())
	{
		SDL_Log("SDL_ttf could not initialize! SDL_ttf error: %s\n", SDL_GetError());
	}

	screenWidth = width;
	screenHeight = height;

	gCoordinator.Init();

	// register all components, alphabetical order
	gCoordinator.RegisterComponent<Affiliation>();
	gCoordinator.RegisterComponent<AttachedTo>();
	gCoordinator.RegisterComponent<DirectionDecision>();
	gCoordinator.RegisterComponent<DisplayFPS>();
	gCoordinator.RegisterComponent<FaceDirection>();
	gCoordinator.RegisterComponent<Hitbox>();
	gCoordinator.RegisterComponent<InputKeys>();
	gCoordinator.RegisterComponent<IsCollision>();
	gCoordinator.RegisterComponent<IsLight>();
	gCoordinator.RegisterComponent<IsObject>();
	gCoordinator.RegisterComponent<IsPlayer>();
	gCoordinator.RegisterComponent<IsUI>();
	gCoordinator.RegisterComponent<Mass>();
	gCoordinator.RegisterComponent<Movement>();
	gCoordinator.RegisterComponent<Position>();
	gCoordinator.RegisterComponent<Text>();
	gCoordinator.RegisterComponent<Texture>();
	gCoordinator.RegisterComponent<TextureLight>();
	gCoordinator.RegisterComponent<TileMap>();

	// register all systems, forward declaration, signature
	rendererSystem_Sprite = gCoordinator.RegisterSystem<RendererSystem_Sprite>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Texture>());
		signature.set(gCoordinator.GetComponentType<Position>());
		gCoordinator.SetSystemSignature<RendererSystem_Sprite>(signature);
	}
	rendererSystem_TileMap = gCoordinator.RegisterSystem<RendererSystem_TileMap>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<TileMap>());
		gCoordinator.SetSystemSignature<RendererSystem_TileMap>(signature);
	}
	rendererSystem_Light = gCoordinator.RegisterSystem<RendererSystem_Light>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<IsLight>());
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<TextureLight>());
		gCoordinator.SetSystemSignature<RendererSystem_Light>(signature);
	}
	rendererSystem_UI = gCoordinator.RegisterSystem<RendererSystem_UI>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<IsUI>());
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<Text>());
		gCoordinator.SetSystemSignature<RendererSystem_UI>(signature);
	}
	movementPlayerSystem = gCoordinator.RegisterSystem<MovementPlayerSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<DirectionDecision>());
		signature.set(gCoordinator.GetComponentType<Movement>());
		signature.set(gCoordinator.GetComponentType<IsPlayer>());
		gCoordinator.SetSystemSignature<MovementPlayerSystem>(signature);
	}
	movementPlayer_DecisionSystem = gCoordinator.RegisterSystem<MovementPlayer_decisionSystem>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<InputKeys>());
		signature.set(gCoordinator.GetComponentType<DirectionDecision>());
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
		signature.set(gCoordinator.GetComponentType<Mass>());
		signature.set(gCoordinator.GetComponentType<Mass>());
		gCoordinator.SetSystemSignature<CollisionSystem>(signature);
	}
	system_Display_FPS = gCoordinator.RegisterSystem<System_Display_FPS>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<DisplayFPS>());
		signature.set(gCoordinator.GetComponentType<Text>());
		gCoordinator.SetSystemSignature<System_Display_FPS>(signature);
	}
}
