#include "Game.h"

extern Coordinator gCoordinator;
Entity gPlayerEntity;

// systems forward declaration
extern std::shared_ptr<DirectionPlayerSystem> directionPlayerSystem;

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
		gCoordinator.AddComponent<Movement>(gPlayerEntity, Movement{ 156.0f });
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
			TextureLight{ std::make_tuple<int, int, float, float, float>(720, 720, 100.0f, 0, 0), generateGaussianLight });
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

	Entity frameRate_UI = gCoordinator.CreateEntity();
	{
		gCoordinator.AddComponent<IsUI>(frameRate_UI, IsUI{});
		gCoordinator.AddComponent<Position>(frameRate_UI, Position{ Vector2D<float>(710.0f, 5.0f) });
		gCoordinator.AddComponent<Text>(frameRate_UI, Text{ "60", 25, 25, ColorValues{0, 255, 0} });
		gCoordinator.AddComponent<DisplayFPS>(frameRate_UI, DisplayFPS{});
	}




	DungeonSystem test;
	test.init();
	directionPlayerSystem->makeMarker();
}