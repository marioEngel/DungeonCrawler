// ----------Rest----------------------
#include "Game.h"
#include "System/Render/TextureFunc.h"
#include "Math/Matrix.h"
#include <map>
#include "Misc/MiscFunctions.h"
#include "System/KeyInput/Mouse.h"
#include "System/Render/Renderer_Misc.h"
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
extern std::shared_ptr<RendererSpriteSystem> rendererSpriteSystem;
extern std::shared_ptr<RendererTileMapSystem> rendererTileMapSystem;
extern std::shared_ptr<RendererLightSystem> rendererLightSystem;
extern std::shared_ptr<RendererUISystem> rendererUISystem;

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
	GeomRectangle playerComp_Hitbox_Rect{ 16.0f, 16.0f, 32.0f, 32.0f };
	GeomCircle playerComp_Hitbox_Circ{ 16.0f, 16.0f, 16.0f };
	GeomRectangle torch_Comp_Hitbox{ 8.0f, 8.0f, 16.0f, 16.0f };

	gPlayerEntity = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Position>(gPlayerEntity, Position{ Vector2D<float>(400.0f, 300.0f) });
		gCoordinator.AddComponent<Texture>(gPlayerEntity, Texture{ "Picture/Player.png", 32, 32, 1, 0.0 });
		//gCoordinator.AddComponent<Texture>(gPlayerEntity, Texture{ "Picture/FullPinkEdge.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsPlayer>(gPlayerEntity, IsPlayer{});
		gCoordinator.AddComponent<InputKeys>(gPlayerEntity, InputKeys{});
		gCoordinator.AddComponent<Movement>(gPlayerEntity, Movement{});
		gCoordinator.AddComponent<Affiliation>(gPlayerEntity, Affiliation{ eAffKind::Player });
		gCoordinator.AddComponent<DirectionDecision>(gPlayerEntity, DirectionDecision{});
		gCoordinator.AddComponent<FaceDirection>(gPlayerEntity, FaceDirection{});
		//gCoordinator.AddComponent<Comp_Hitbox>(gPlayerEntity, Comp_Hitbox{
		//	std::make_shared<GeomRectangle>(playerComp_Hitbox_Rect), eCollisionType::PHYSICAL, "Picture/FullPinkEdge.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<Comp_Hitbox>(gPlayerEntity, Comp_Hitbox{ 
			std::make_shared<GeomCircle>(playerComp_Hitbox_Circ), eCollisionType::PHYSICAL, "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsCollision>(gPlayerEntity, IsCollision{ true });
	}

	for (size_t i = 0; i < 16; i++)
	{
		Entity tmpEntity = gCoordinator.CreateEntity();		
		gCoordinator.AddComponent<Position>(tmpEntity, Position{ Vector2D<float>(32.0f * (i % 4), 32.0f * (i / 4)) });
		gCoordinator.AddComponent<Texture>(tmpEntity, Texture{ "Picture/FullPinkEdge.png", 32, 32, 1, 0.0 });
	}
	
	Entity testComp_Hitbox = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Position>(testComp_Hitbox, Position{ Vector2D<float>(400.0f, 150.0f) });
		gCoordinator.AddComponent<Texture>(testComp_Hitbox, Texture{ "Picture/Ratte.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<Comp_Hitbox>(testComp_Hitbox, Comp_Hitbox{ 
			std::make_shared<GeomRectangle>(playerComp_Hitbox_Rect), eCollisionType::PHYSICAL, "Picture/FullPinkEdge.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsCollision>(testComp_Hitbox, IsCollision{ false });
		gCoordinator.AddComponent<Movement>(testComp_Hitbox, Movement{ 0, Vector2D<float> {}, true });
	}

	Entity testComp_Hitbox2 = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Position>(testComp_Hitbox2, Position{ Vector2D<float>(500.0f, 150.0f) });
		gCoordinator.AddComponent<Texture>(testComp_Hitbox2, Texture{ "Picture/Ratte.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<Comp_Hitbox>(testComp_Hitbox2, Comp_Hitbox{ 
			std::make_shared<GeomCircle>(playerComp_Hitbox_Circ), eCollisionType::PHYSICAL, "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
		gCoordinator.AddComponent<IsCollision>(testComp_Hitbox2, IsCollision{ false });
		gCoordinator.AddComponent<Movement>(testComp_Hitbox2, Movement{ 0, Vector2D<float> {}, true });
	}

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
	//	gCoordinator.AddComponent<Comp_Hitbox>(torch, 
	//		Comp_Hitbox{ std::make_shared<GeomRectangle>(torch_Comp_Hitbox), eCollisionType::PHYSICAL, "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
	//}
	//Entity torch_light = gCoordinator.CreateEntity();
	//{
	//	gCoordinator.AddComponent<Position>(torch_light, Position{ Vector2D<float>(100, 100) });
	//	gCoordinator.AddComponent<AttachedTo>(torch_light, AttachedTo{ true, torch });
	//	gCoordinator.AddComponent<TextureLight>(torch_light, TextureLight{ 720, 720, 50.0f});
	//	gCoordinator.AddComponent<IsLight>(torch_light, IsLight{ true });
	//} 
	//Entity lightSource2 = gCoordinator.CreateEntity();
	//{
	//	gCoordinator.AddComponent<Position>(lightSource2, Position{ Vector2D<float>(100,50) });
	//	gCoordinator.AddComponent<TextureLight>(lightSource2, TextureLight{ "Picture/light.png", 720, 720, 1, 0, ColorValues{255, 204, 153} });
	//	gCoordinator.AddComponent<Is_Light>(lightSource2, Is_Light{ true });
	//}
	//Entity testComp_Hitbox3 = gCoordinator.CreateEntity();
	//gCoordinator.AddComponent<Position>(testComp_Hitbox3, Position{ Vector2D<float>(465.0f, 150.0f) });
	//gCoordinator.AddComponent<Texture>(testComp_Hitbox3, TextureLight{ "Picture/Ratte.png", 32, 32, 1, 0.0 });
	//gCoordinator.AddComponent<Comp_Hitbox>(testComp_Hitbox3, Comp_Hitbox{ std::make_shared<GeomCircle>(tmpCircle),  "Picture/FullPinkCircle.png", 32, 32, 1, 0.0 });
	//gCoordinator.AddComponent<Is_Collision>(testComp_Hitbox3, Is_Collision{ false });

	Entity testText = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<Text>(testText, Text{ " Hier ist ein Text" });
		gCoordinator.AddComponent<IsUI>(testText, IsUI{ true });
		gCoordinator.AddComponent<Position>(testText, Position{ Vector2D<float>(100.0f, 100.0f) });
	}


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
		collisionSystem->check_AABB();
		collisionSystem->check_General();
		collisionSystem->react();
		
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
		rendererTileMapSystem->loadTexture();
		rendererTileMapSystem->initRenderertex();
		rendererTileMapSystem->render();

		rendererSpriteSystem->loadTexture();
		rendererSpriteSystem->initRenderertex();

		//tmp for checking Comp_Hitboxes not working
		bool renderComp_Hitbox = true;
		collisionSystem->create_Comp_HitboxRender();
		if (renderComp_Hitbox) {
			collisionSystem->render_Comp_Hitbox();
		}
		
		rendererSpriteSystem->render();

		rendererLightSystem->loadTexture();
		rendererLightSystem->initRenderertex();
		rendererLightSystem->render();

		rendererUISystem->loadTexture();
		rendererUISystem->initRenderertex();
		rendererUISystem->render();
	}
	
	gKeyboardInput.getPrevKeys();
	drawAndClear(
		rendererTileMapSystem->rtnRenderertex(),
		rendererSpriteSystem->rtnRenderertex(),
		rendererLightSystem->rtnRenderertex(),
		rendererUISystem->rtnRenderertex()
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


