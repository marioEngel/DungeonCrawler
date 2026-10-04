#include "Game.h"
#include "System/Render/Misc/TextureFunc.h"

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;
extern Entity gPlayerEntity;
extern Camera gCamera;
static Mouse gMouse;

extern std::shared_ptr<SysMovementPlayerDecision> MovementPlayerDecision;

void Game::update(float delta) {
    // input pipeline stuff soll in handle input sein
    {
        if (gKeyboardInput.getButtonState(SDL_SCANCODE_X)) {
            gMouse.printMousePosition_color();
        }

        sys::MovementPlayerDecision->update();
        sys::MovementTileDecision->update();
    }

    // movement pipeline
    {
        // sysMovementPlayer->update();
        sys::MovementTile->update(delta);
        //sys::DirectionPlayer->update();

        sys::MovementObject->update();
        sys::MovementObjectAttached->update();
    }

    // collision pipeline
    {
        //sysCollision->check_AABB();
        //sysCollision->check_General();
        //sysCollision->react();

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
        sys::DisplayFPS->update(delta);
    }

    // render pipeline
    {
        sys::RendererTileMap->loadTexture();
        sys::RendererTileMap->createTileMapRenderertex();
        sys::RendererTileMap->initRenderertex();
        sys::RendererTileMap->render();

        sys::RendererSprite->loadTexture();
        sys::RendererSprite->initRenderertex();
        //tmp for checking Hitboxes not working
        //bool renderHitbox = true;
        //if (renderHitbox) {
        //	sysCollision->create_HitboxRender();
        //	sysCollision->render_Hitbox();
        //}
        sys::RendererSprite->render();


        sys::RendererLight->loadTexture();
        sys::RendererLight->initRenderertex();
        sys::RendererLight->render();

        sys::RendererUI->loadTexture();
        sys::RendererUI->initRenderertex();
        sys::RendererUI->render();
    }

    gKeyboardInput.getPrevKeys();
    drawAndClear(
        sys::RendererTileMap->rtnRenderertex(),
        sys::RendererSprite->rtnRenderertex(),
        sys::RendererLight->rtnRenderertex(),
        sys::RendererUI->rtnRenderertex()
    );
}

// is game closed? returning bool
void Game::handleEvents() {
    gKeyboardInput.registerInput();

    switch (event.type) {
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
bool Game::gameRunning() {
    return isRunning;
}
