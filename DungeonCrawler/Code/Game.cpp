#include "Game.h"
#include <map>
#include "System/Render/TextureFunc.h"

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;
extern Entity gPlayerEntity;
Camera gCamera;
Mouse gMouse;

// systems forward declaration
extern std::shared_ptr<RendererSystem_Sprite> rendererSystem_Sprite;
extern std::shared_ptr<RendererSystem_TileMap> rendererSystem_TileMap;
extern std::shared_ptr<RendererSystem_Light> rendererSystem_Light;
extern std::shared_ptr<RendererSystem_UI> rendererSystem_UI;
extern std::shared_ptr<MovementPlayerSystem> movementPlayerSystem;
extern std::shared_ptr<MovementPlayer_decisionSystem> movementPlayer_DecisionSystem;
extern std::shared_ptr<DirectionPlayerSystem> directionPlayerSystem;
extern std::shared_ptr<MovementObjectSystem> movementObjectSystem;
extern std::shared_ptr<CollisionSystem> collisionSystem;
extern std::shared_ptr<MovementObject_attachedSystem> movementObject_attachedSystem;
extern std::shared_ptr<System_Display_FPS> system_Display_FPS;

void Game::update(float delta)
{
	gKeyboardInput.registerInput(); 
	
	if (gKeyboardInput.getButtonState(SDL_SCANCODE_X) ||
		gKeyboardInput.getButtonState(SDL_SCANCODE_X) == eButtonState::RELEASED)
	{
		gMouse.printMousePosition_color();
	}

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
		collisionSystem ->check_AABB();
		collisionSystem->check_General();
		collisionSystem->react();
		
		//for (int i = 0; i < 5; i++) {       // iteration count tunable
		//	collisionSystem->react();
		//	collisionSystem->recheck_General(); // narrowphase only, skip broadphase
		//}

		gCamera.CheckCollision(gPlayerEntity);


	}

	// Mouse test delete late
	{
		gMouse.update();
		//gMouse.printMousePosition();
		//gMouse.printMousePosition_color();
		gMouse.click();
	
	}

	{
		system_Display_FPS->update(delta);
	}

	// render pipeline
	{
		rendererSystem_TileMap->loadTexture();
		rendererSystem_TileMap->createTileMapRenderertex();
		rendererSystem_TileMap->initRenderertex();
		rendererSystem_TileMap->render();

		rendererSystem_Sprite->loadTexture();
		rendererSystem_Sprite->initRenderertex();
		//tmp for checking Hitboxes not working
		bool renderHitbox = true;
		if (renderHitbox) {
			collisionSystem->create_HitboxRender();
			collisionSystem->render_Hitbox();
		}
 		rendererSystem_Sprite->render();
			

		rendererSystem_Light->loadTexture();
		rendererSystem_Light->initRenderertex();
		rendererSystem_Light->render();

		rendererSystem_UI->loadTexture();
		rendererSystem_UI->initRenderertex();
		rendererSystem_UI->render();
	}
	
	gKeyboardInput.getPrevKeys();
	drawAndClear(
		rendererSystem_TileMap->rtnRenderertex(),
		rendererSystem_Sprite->rtnRenderertex(),
		rendererSystem_Light->rtnRenderertex(),
		rendererSystem_UI->rtnRenderertex()
	);
}

// is game closed? returning bool
void Game::handleEvents()
{
	gKeyboardInput.registerInput();

	switch (event.type)
	{
	case SDL_EVENT_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::clean() {
	SDL_StopTextInput(window);
	SDL_DestroyWindow(window);

	SDL_DestroyRenderer(renderer);
	TTF_Quit();
	SDL_Quit();
}

void Game::makeEnd()
{
	isRunning = false;
}

// is game running? bool
bool Game::gameRunning()
{
	return isRunning;
}
