#include "DirectionPlayer.h"
#include "../../ECS/Coordinator.h"
#include "../KeyInput/KeyboardInput.h"
#include "../../Component/FaceDirection.h"
#include "../../Component/PositionComp.h"
#include "../../Math/Vector2D.h"
#include "../../Component/TextureComp.h"
#include "../../Component/MovementComp.h"
#include "../../Component/IsObject.h"

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;

void DirectionPlayerSystem::update()
{
	for (auto& const entity : mEntities)
	{
		auto& position = gCoordinator.GetComponent<Position>(entity);
		auto& direction = gCoordinator.GetComponent<FaceDirection>(entity);

		Vector2D<int> tmpMouseInt;
		SDL_GetMouseState(&tmpMouseInt[0], &tmpMouseInt[1]);
		Vector2D<float> tmpMouse{ float(tmpMouseInt[0]), float(tmpMouseInt[1]) };

		float angle =  calc_angle(Vector2D<float> {position.pos[0], position.pos[1]}, tmpMouse, true);
		direction.angle = angle;
		
		auto& pinkPos = gCoordinator.GetComponent<Position>(direction.Entity);
		pinkPos.pos = position.pos + Vector2D<float> {32.0f * std::cos(direction.angle), -32.0f * std::sin(direction.angle) } + Vector2D<float>{16.0f, 16.0f};

		static int test = 0;

		if (gKeyboardInput.getButtonState(SDL_SCANCODE_L) ||
			gKeyboardInput.getButtonState(SDL_SCANCODE_L) == eButtonState::RELEASED)
		{
			test++;
			if (test < 2)
			{
				std::cout << "worked\n";
				Entity tmpBlast = gCoordinator.CreateEntity();
				gCoordinator.AddComponent<Position>(tmpBlast, Position{ position.pos });
				gCoordinator.AddComponent<Texture>(tmpBlast, Texture{ "Picture/Blast.png", 10, 20, 1 , - direction.angle * 180/M_PI });
				gCoordinator.AddComponent<FaceDirection>(tmpBlast, FaceDirection{ direction.angle });
				gCoordinator.AddComponent<Movement>(tmpBlast, Movement{ 200.0f });
				gCoordinator.AddComponent<IsObject>(tmpBlast, IsObject{});
			}
		}
	}
}

void DirectionPlayerSystem::makeMarker()
{
	for (auto& const entity : mEntities)
	{
		auto& direction = gCoordinator.GetComponent<FaceDirection>(entity);
		auto& position = gCoordinator.GetComponent<Position>(entity);

		direction.Entity = gCoordinator.CreateEntity();
		gCoordinator.AddComponent<Position>(direction.Entity, Position{ position.pos + Vector2D<float>(32.0f, 0.0f) });
		gCoordinator.AddComponent<Texture>(direction.Entity, Texture{ "Picture/FullPink.png", 6, 6, 1 });

		std::cout << position.pos << std::endl;
	}
}