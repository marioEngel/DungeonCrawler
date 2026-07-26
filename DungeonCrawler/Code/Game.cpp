#include "Game.h"
#include <map>
#include "System/Render/Misc/TextureFunc.h"

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;
extern Entity gPlayerEntity;
Camera gCamera;
Mouse gMouse;

// systems forward declaration
extern std::shared_ptr<SysRendererSprite> sysRendererSprite;
extern std::shared_ptr<SysRendererTileMap> sysRendererTileMap;
extern std::shared_ptr<SysRendererLight> sysRendererLight;
extern std::shared_ptr<SysRendererUI> sysRendererUI;
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
		sysRendererTileMap->loadTexture();
		sysRendererTileMap->createTileMapRenderertex();
		sysRendererTileMap->initRenderertex();
		sysRendererTileMap->render();

		sysRendererSprite->loadTexture();
		sysRendererSprite->initRenderertex();
		//tmp for checking Hitboxes not working
		bool renderHitbox = true;
		if (renderHitbox) {
			collisionSystem->create_HitboxRender();
			collisionSystem->render_Hitbox();
		}
 		sysRendererSprite->render();
			

		sysRendererLight->loadTexture();
		sysRendererLight->initRenderertex();
		sysRendererLight->render();

		sysRendererUI->loadTexture();
		sysRendererUI->initRenderertex();
		sysRendererUI->render();
	}
	
	gKeyboardInput.getPrevKeys();
	drawAndClear(
		sysRendererTileMap->rtnRenderertex(),
		sysRendererSprite->rtnRenderertex(),
		sysRendererLight->rtnRenderertex(),
		sysRendererUI->rtnRenderertex()
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
