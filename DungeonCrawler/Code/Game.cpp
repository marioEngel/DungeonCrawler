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
extern std::shared_ptr<SysMovementPlayer> sysMovementPlayer;
extern std::shared_ptr<SysMovementPlayerDecision> sysMovementPlayerDecision;
extern std::shared_ptr<SysDirectionPlayer> sysDirectionPlayer;
extern std::shared_ptr<SysMovementObject> sysMovementObject;
extern std::shared_ptr<SysCollision> sysCollision;
extern std::shared_ptr<SysMovementObjectAttached> sysMovementObjectAttached;
extern std::shared_ptr<SysDisplayFPS> sysDisplayFPS;

void Game::update(float delta)
{
	// input pipeline stuff soll in handle input sein
	{
		if (gKeyboardInput.getButtonState(SDL_SCANCODE_X) ||
			gKeyboardInput.getButtonState(SDL_SCANCODE_X) == eButtonState::RELEASED)
		{
			gMouse.printMousePosition_color();
		}

		sysMovementPlayerDecision->update();
	}

	// movement pipeline
	{
		sysMovementPlayer->update();
		sysDirectionPlayer->update();

		sysMovementObject->update();
		sysMovementObjectAttached->update();
	}
	
	// collision pipeline
	{
		sysCollision->check_AABB();
		sysCollision->check_General();
		sysCollision->react();
		
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
		sysDisplayFPS->update(delta);
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
			sysCollision->create_HitboxRender();
			sysCollision->render_Hitbox();
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

//void Game::makeEnd()
//{
//	isRunning = false;
//}

// is game running? bool
bool Game::gameRunning()
{
	return isRunning;
}
