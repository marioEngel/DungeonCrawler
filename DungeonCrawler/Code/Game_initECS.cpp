#include <map>
#include "Game.h"
#include <SDL3_ttf/SDL_ttf.h>

SDL_Renderer* Game::renderer = NULL;
SDL_Event Game::event;
int Game::screenWidth;
int Game::screenHeight;

Coordinator gCoordinator;
KeyboardInput gKeyboardInput;

// systems forward declaration
std::shared_ptr<SysRendererSprite> sysRendererSprite;
std::shared_ptr<SysRendererTileMap> sysRendererTileMap;
std::shared_ptr<SysRendererLight> sysRendererLight;
std::shared_ptr<SysRendererUI> sysRendererUI;
std::shared_ptr<SysMovementPlayer> sysMovementPlayer;
std::shared_ptr<SysMovementPlayerDecision> sysMovementPlayerDecision;
std::shared_ptr<SysDirectionPlayer> sysDirectionPlayer;
std::shared_ptr<SysMovementObject> sysMovementObject;
std::shared_ptr<SysCollision> sysCollision;
std::shared_ptr<SysMovementObjectAttached> sysMovementObjectAttached;
std::shared_ptr<SysDisplayFPS> sysDisplayFPS;
std::shared_ptr<SysLevel> sysLevel;
std::shared_ptr<SysMovementTile> sysMovementTile;
std::shared_ptr<SysMovementTileDecision> sysMovementTileDecision;

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
		&& SDL_Init(SDL_INIT_AUDIO))
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
	gCoordinator.RegisterComponent<Level>();
	gCoordinator.RegisterComponent<MovementTile>();

	// register all systems, forward declaration, signature
	sysRendererSprite = gCoordinator.RegisterSystem<SysRendererSprite>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Texture>());
		signature.set(gCoordinator.GetComponentType<Position>());
		gCoordinator.SetSystemSignature<SysRendererSprite>(signature);
	}
	sysRendererTileMap = gCoordinator.RegisterSystem<SysRendererTileMap>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<TileMap>());
		gCoordinator.SetSystemSignature<SysRendererTileMap>(signature);
	}
	sysRendererLight = gCoordinator.RegisterSystem<SysRendererLight>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<IsLight>());
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<TextureLight>());
		gCoordinator.SetSystemSignature<SysRendererLight>(signature);
	}
	sysRendererUI = gCoordinator.RegisterSystem<SysRendererUI>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<IsUI>());
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<Text>());
		gCoordinator.SetSystemSignature<SysRendererUI>(signature);
	}
	sysMovementPlayer = gCoordinator.RegisterSystem<SysMovementPlayer>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<DirectionDecision>());
		signature.set(gCoordinator.GetComponentType<Movement>());
		signature.set(gCoordinator.GetComponentType<IsPlayer>());
		gCoordinator.SetSystemSignature<SysMovementPlayer>(signature);
	}
	sysMovementPlayerDecision = gCoordinator.RegisterSystem<SysMovementPlayerDecision>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<InputKeys>());
		signature.set(gCoordinator.GetComponentType<DirectionDecision>());
		signature.set(gCoordinator.GetComponentType<IsPlayer>());
		signature.set(gCoordinator.GetComponentType<Movement>());
		gCoordinator.SetSystemSignature<SysMovementPlayerDecision>(signature);
	}
	sysDirectionPlayer = gCoordinator.RegisterSystem<SysDirectionPlayer>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<FaceDirection>());
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<IsPlayer>());
		gCoordinator.SetSystemSignature<SysDirectionPlayer>(signature);
	}
	sysMovementObjectAttached = gCoordinator.RegisterSystem<SysMovementObjectAttached>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<AttachedTo>());
		signature.set(gCoordinator.GetComponentType<Position>());
		gCoordinator.SetSystemSignature<SysMovementObjectAttached>(signature);
	}
	sysMovementObject = gCoordinator.RegisterSystem<SysMovementObject>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<FaceDirection>());
		signature.set(gCoordinator.GetComponentType<Movement>());
		signature.set(gCoordinator.GetComponentType<IsObject>());
		gCoordinator.SetSystemSignature<SysMovementObject>(signature);
	}
	sysCollision = gCoordinator.RegisterSystem<SysCollision>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<Hitbox>());
		signature.set(gCoordinator.GetComponentType<Movement>());
		signature.set(gCoordinator.GetComponentType<IsCollision>());
		signature.set(gCoordinator.GetComponentType<Mass>());
		signature.set(gCoordinator.GetComponentType<Mass>());
		gCoordinator.SetSystemSignature<SysCollision>(signature);
	}
	sysDisplayFPS = gCoordinator.RegisterSystem<SysDisplayFPS>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<DisplayFPS>());
		signature.set(gCoordinator.GetComponentType<Text>());
		gCoordinator.SetSystemSignature<SysDisplayFPS>(signature);
	}
	sysLevel = gCoordinator.RegisterSystem<SysLevel>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Level>());
		gCoordinator.SetSystemSignature<SysLevel>(signature);
	}
	sysMovementTile = gCoordinator.RegisterSystem<SysMovementTile>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<Position>());
		signature.set(gCoordinator.GetComponentType<MovementTile>());
		gCoordinator.SetSystemSignature<SysMovementTile>(signature);
	}
	sysMovementTileDecision = gCoordinator.RegisterSystem<SysMovementTileDecision>();
	{
		Signature signature;
		signature.set(gCoordinator.GetComponentType<InputKeys>());
		signature.set(gCoordinator.GetComponentType<DirectionDecision>());
		signature.set(gCoordinator.GetComponentType<IsPlayer>());
		signature.set(gCoordinator.GetComponentType<MovementTile>());
		signature.set(gCoordinator.GetComponentType<Position>());
		gCoordinator.SetSystemSignature<SysMovementTileDecision>(signature);
	}
}
