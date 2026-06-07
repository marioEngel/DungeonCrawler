// ----------Rest----------------------
#include "Game.h"
#include "System/Render/TextureFunc.h"
#include "Math/Matrix.h"
#include <map>
#include "Misc/MiscFunctions.h"
#include "System/KeyInput/Mouse.h"
#include "System/Render/Renderer_Misc.h"
#include "Characters/CharacterTemplate.h"
// ---------Components------------------ max 32
#include "Component/Comp_Position.h"
#include "Component/Comp_Texture.h"
#include "Component/Comp_TileMap.h"
#include "Component/Is_Player.h"
#include "Component/Comp_InputKeys.h"			// 5
#include "Component/Comp_Movement.h"
#include "Component/Comp_Affiliation.h"
#include "Component/Comp_FaceDirection.h"
#include "Component/Comp_DirectionDecision.h"
#include "Component/Comp_Hitbox.h"				// 10
#include "Component/Is_Collision.h"
#include "Component/Is_Light.h"
#include "Component/Comp_TextureLight.h"
#include "Component/Comp_AttachedTo.h"
#include "Component/Comp_Text.h"
#include "Component/Is_UI.h"
#include "Component/Comp_Mass.h"
// --------Systems---------------------- 
#include "System/Render/Renderer_Sprite.h"
#include "System/Render/Renderer_TileMap.h"
#include "System/Render/Renderer_Light.h"
#include "System/Render/Camera.h"
#include "System/Movement/DirectionPlayer.h"			// 5
#include "System/Movement/MovementPlayer.h"
#include "System/Movement/MovementPlayer_decision.h"
#include "System/Dungeon/Dungeon.h"
#include "System/Movement/MovementObject.h"				// 10
#include "System/KeyInput/KeyboardInput.h"
#include "System/Collision/CollisionSystem.h"				
#include "System/Collision/Geometry/Geometry.h"
#include "System/Collision/Geometry/Rectangle.h"
#include "System/Collision/Geometry/Circle.h"			// 15
#include "System/Movement/MovementObject_attached.h"
#include "System/Render/Renderer_UI.h"

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;
Entity gPlayerEntity;
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

// is game running? bool
bool Game::gameRunning()
{
	return isRunning;
}

void Game::initEntities()
{
	GeomRectangle playerHitbox_Rect{ 16.0f, 16.0f, 32.0f, 32.0f };
	GeomCircle playerHitbox_Circ{ 16.0f, 16.0f, 16.0f };
	GeomRectangle torch_Hitbox{ 8.0f, 8.0f, 16.0f, 16.0f };

	gPlayerEntity = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Position>(gPlayerEntity, Position{ Vector2D<float>(400.0f, 300.0f) });
		gCoordinator.AddComponent<Texture>(gPlayerEntity, Texture{ "Picture/Player.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsPlayer>(gPlayerEntity, IsPlayer{});
		gCoordinator.AddComponent<InputKeys>(gPlayerEntity, InputKeys{});
		gCoordinator.AddComponent<Movement>(gPlayerEntity, Movement{156.0f});
		gCoordinator.AddComponent<Affiliation>(gPlayerEntity, Affiliation{ eAffKind::Player });
		gCoordinator.AddComponent<DirectionDecision>(gPlayerEntity, DirectionDecision{});
		gCoordinator.AddComponent<FaceDirection>(gPlayerEntity, FaceDirection{});
		//gCoordinator.AddComponent<Hitbox>(gPlayerEntity, Hitbox{
			//std::make_shared<GeomRectangle>(playerHitbox_Rect), eCollisionType::PHYSICAL, "Picture/FullPinkEdge.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<Hitbox>(gPlayerEntity, Hitbox{ 
			std::make_shared<GeomCircle>(playerHitbox_Circ), eCollisionType::PHYSICAL, "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsCollision>(gPlayerEntity, IsCollision{ true });
		gCoordinator.AddComponent<Mass>(gPlayerEntity, Mass{ 50 });
	}

	//for (size_t i = 0; i < 5; i++)
	//{
	//	create_Mouse(Vector2D<float>(100.0f + 50.0f*i, 150.0f));
	//}
	
	create_Mouse(Vector2D<float>(400.0f, 150.0f));



	Entity lightSource = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Position>(lightSource, Position{ Vector2D<float>(40,-60) });
		gCoordinator.AddComponent<TextureLight>(lightSource, 
			TextureLight{ std::make_tuple<int, int, float, float, float>(720, 720, 100.0f, 0, 0), generateGaussianLight});
		//gCoordinator.AddComponent<TextureLight>(lightSource,
		//	TextureLight{ std::make_tuple<int, int, float, float, float>(2000, 2000, 30.0f, 0.0f, 0.0f), generateFlashLightCone, 2000, 2000 });
		//gCoordinator.AddComponent<TextureLight>(lightSource,
		//	TextureLight{ 720, 720, {255, 255, 255}, std::make_tuple<int, int, float, float, float>(720, 720, 50.0f, 200.0f, 0), generateGaussianLightTwoSigma });

		gCoordinator.AddComponent<IsLight>(lightSource, IsLight{ true });
		gCoordinator.AddComponent<AttachedTo>(lightSource, AttachedTo{ true, gPlayerEntity });
	}

	//Entity torch = gCoordinator.CreateEntity();
	//{
	//	gCoordinator.AddComponent<Position>(torch, Position{ Vector2D<float>(100, 100) });
	//	gCoordinator.AddComponent<Texture>(torch, Texture{ "Picture/torch.png", 16, 16, 1, 0.0 });
	//	gCoordinator.AddComponent<Hitbox>(torch, 
	//		Hitbox{ std::make_shared<GeomRectangle>(torch_Hitbox), eCollisionType::PHYSICAL, "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
	//}

	//candle{ 255,  80,  10 }  — dimmer, more red than torch
	//torch{ 255, 100,  20 }  — warm orange
	//campfire{ 255, 120,  30 }  — slightly brighter, more yellow
	//lantern{ 255, 160,  60 }  — warmer, more yellow, steadier
	//moonlight{ 80, 100, 180 }  — cold blue - white
	//magic blue{ 20, 100, 255 }  — full blue, good for magic effects
	//magic green{ 20, 200,  50 }  — poison / nature magic
	//ColorValues lightYellow{ 255, 255, 100 };
	//ColorValues torchRed{ 255, 80, 10 };
	//ColorValues torchYellow{ 255, 140, 40 };
	//Entity torch_light = gCoordinator.CreateEntity();
	//{
	//	gCoordinator.AddComponent<Position>(torch_light, Position{ Vector2D<float>(100, 100) });
	//	gCoordinator.AddComponent<AttachedTo>(torch_light, AttachedTo{ true, torch });
	//	gCoordinator.AddComponent<TextureLight>(torch_light,
	//		TextureLight{ std::make_tuple<int, int, float, float, float>(200, 200, 25.0f, 0, 0),
	//		generateGaussianLight, 500, 500, 0.0f, torchYellow, false, 0.5, 1.0 });
	//	gCoordinator.AddComponent<IsLight>(torch_light, IsLight{ true });
	//} 

	//Entity testHitbox3 = gCoordinator.CreateEntity();
	//gCoordinator.AddComponent<Position>(testHitbox3, Position{ Vector2D<float>(465.0f, 150.0f) });
	//gCoordinator.AddComponent<Texture>(testHitbox3, TextureLight{ "Picture/Ratte.png", 32, 32, 1, 0.0 });
	//gCoordinator.AddComponent<Hitbox>(testHitbox3, Hitbox{ std::make_shared<GeomCircle>(tmpCircle),  "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
	//gCoordinator.AddComponent<Is_Collision>(testHitbox3, Is_Collision{ false });

	//Entity testText = gCoordinator.CreateEntity();
	//{
	//	gCoordinator.AddComponent<Text>(testText, Text{ " Hier ist ein Text" });
	//	gCoordinator.AddComponent<IsUI>(testText, IsUI{ true });
	//	gCoordinator.AddComponent<Position>(testText, Position{ Vector2D<float>(100.0f, 100.0f) });
	//}


	DungeonSystem test;
	test.init();
	directionPlayerSystem->makeMarker();
}


void Game::update()
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

	// render pipeline
	{
		rendererSystem_TileMap->loadTexture();
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


