#include "Game.h"
#include <SDL3_ttf/SDL_ttf.h>

SDL_Renderer *Game::renderer = NULL;
SDL_Event Game::event;
int Game::screenWidth;
int Game::screenHeight;

extern Coordinator gCoordinator;

Game::Game() {
}

Game::~Game() {
}

namespace sys {
    // systems forward declaration
    std::shared_ptr<SysRendererSprite> RendererSprite;
    std::shared_ptr<SysRendererTileMap> RendererTileMap;
    std::shared_ptr<SysRendererLight> RendererLight;
    std::shared_ptr<SysRendererUI> RendererUI;
    std::shared_ptr<SysMovementPlayer> MovementPlayer;
    std::shared_ptr<SysMovementPlayerDecision> MovementPlayerDecision;
    std::shared_ptr<SysDirectionPlayer> DirectionPlayer;
    std::shared_ptr<SysMovementObject> MovementObject;
    std::shared_ptr<SysMovementObjectAttached> MovementObjectAttached;
    std::shared_ptr<SysDisplayFPS> DisplayFPS;
    std::shared_ptr<SysMovementTile> MovementTile;
    std::shared_ptr<SysMovementTileDecision> MovementTileDecision;
    std::shared_ptr<SysTower> Tower;
}


void Game::initECS(const char *text, int width, int height, int flag) {
    // init SDL system
    if (
        SDL_Init(SDL_INIT_VIDEO)
        && SDL_Init(SDL_INIT_EVENTS)
        && SDL_Init(SDL_INIT_AUDIO)
    ) {
        window = SDL_CreateWindow(text, width, height, flag);
        renderer = SDL_CreateRenderer(window, NULL);
        if (renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
    } else {
        std::cout << "Error : " << SDL_GetError() << std::endl;
    }
    SDL_StartTextInput(window);
    if (!TTF_Init()) {
        SDL_Log("SDL_ttf could not initialize! SDL_ttf error: %s\n", SDL_GetError());
    }

    screenWidth = width;
    screenHeight = height;

    gCoordinator.Init();

    // register all components, alphabetical order
    gCoordinator.RegisterComponent<AttachedTo>();
    gCoordinator.RegisterComponent<DirectionDecision>();
    gCoordinator.RegisterComponent<DisplayFPS>();
    gCoordinator.RegisterComponent<FaceDirection>();
    gCoordinator.RegisterComponent<InputKeys>();
    gCoordinator.RegisterComponent<IsCollision>();
    gCoordinator.RegisterComponent<IsLight>();
    gCoordinator.RegisterComponent<IsObject>();
    gCoordinator.RegisterComponent<IsPlayer>();
    gCoordinator.RegisterComponent<IsUI>();
    gCoordinator.RegisterComponent<Mass>();
    gCoordinator.RegisterComponent<Movement>();
    gCoordinator.RegisterComponent<Position>();
    gCoordinator.RegisterComponent<PositionTile>();
    gCoordinator.RegisterComponent<Text>();
    gCoordinator.RegisterComponent<Texture>();
    gCoordinator.RegisterComponent<TextureLight>();
    gCoordinator.RegisterComponent<TileMap>();
    gCoordinator.RegisterComponent<MovementTile>();


    // register all systems, forward declaration, signature
    sys::RendererSprite = gCoordinator.RegisterSystem<SysRendererSprite>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Texture>());
        signature.set(gCoordinator.GetComponentType<Position>());
        gCoordinator.SetSystemSignature<SysRendererSprite>(signature);
    }
    sys::RendererTileMap = gCoordinator.RegisterSystem<SysRendererTileMap>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<TileMap>());
        gCoordinator.SetSystemSignature<SysRendererTileMap>(signature);
    }
    sys::RendererLight = gCoordinator.RegisterSystem<SysRendererLight>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<IsLight>());
        signature.set(gCoordinator.GetComponentType<Position>());
        signature.set(gCoordinator.GetComponentType<TextureLight>());
        gCoordinator.SetSystemSignature<SysRendererLight>(signature);
    }
    sys::RendererUI = gCoordinator.RegisterSystem<SysRendererUI>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<IsUI>());
        signature.set(gCoordinator.GetComponentType<Position>());
        signature.set(gCoordinator.GetComponentType<Text>());
        gCoordinator.SetSystemSignature<SysRendererUI>(signature);
    }
    sys::MovementPlayer = gCoordinator.RegisterSystem<SysMovementPlayer>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Position>());
        signature.set(gCoordinator.GetComponentType<DirectionDecision>());
        signature.set(gCoordinator.GetComponentType<Movement>());
        signature.set(gCoordinator.GetComponentType<IsPlayer>());
        gCoordinator.SetSystemSignature<SysMovementPlayer>(signature);
    }
    sys::MovementPlayerDecision = gCoordinator.RegisterSystem<SysMovementPlayerDecision>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<InputKeys>());
        signature.set(gCoordinator.GetComponentType<DirectionDecision>());
        signature.set(gCoordinator.GetComponentType<IsPlayer>());
        signature.set(gCoordinator.GetComponentType<Movement>());
        gCoordinator.SetSystemSignature<SysMovementPlayerDecision>(signature);
    }
    sys::DirectionPlayer = gCoordinator.RegisterSystem<SysDirectionPlayer>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<FaceDirection>());
        signature.set(gCoordinator.GetComponentType<Position>());
        signature.set(gCoordinator.GetComponentType<IsPlayer>());
        gCoordinator.SetSystemSignature<SysDirectionPlayer>(signature);
    }
    sys::MovementObjectAttached = gCoordinator.RegisterSystem<SysMovementObjectAttached>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<AttachedTo>());
        signature.set(gCoordinator.GetComponentType<Position>());
        gCoordinator.SetSystemSignature<SysMovementObjectAttached>(signature);
    }
    sys::MovementObject = gCoordinator.RegisterSystem<SysMovementObject>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Position>());
        signature.set(gCoordinator.GetComponentType<FaceDirection>());
        signature.set(gCoordinator.GetComponentType<Movement>());
        signature.set(gCoordinator.GetComponentType<IsObject>());
        gCoordinator.SetSystemSignature<SysMovementObject>(signature);
    }
    sys::DisplayFPS = gCoordinator.RegisterSystem<SysDisplayFPS>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<DisplayFPS>());
        signature.set(gCoordinator.GetComponentType<Text>());
        gCoordinator.SetSystemSignature<SysDisplayFPS>(signature);
    }
    sys::MovementTile = gCoordinator.RegisterSystem<SysMovementTile>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<Position>());
        signature.set(gCoordinator.GetComponentType<MovementTile>());
        gCoordinator.SetSystemSignature<SysMovementTile>(signature);
    }
    sys::MovementTileDecision = gCoordinator.RegisterSystem<SysMovementTileDecision>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<InputKeys>());
        signature.set(gCoordinator.GetComponentType<DirectionDecision>());
        signature.set(gCoordinator.GetComponentType<IsPlayer>());
        signature.set(gCoordinator.GetComponentType<MovementTile>());
        signature.set(gCoordinator.GetComponentType<Position>());
        gCoordinator.SetSystemSignature<SysMovementTileDecision>(signature);
    }
    sys::Tower = gCoordinator.RegisterSystem<SysTower>();
    {
        Signature signature;
        signature.set(gCoordinator.GetComponentType<IsPlayer>());
        gCoordinator.SetSystemSignature<SysTower>(signature);
    }
}
