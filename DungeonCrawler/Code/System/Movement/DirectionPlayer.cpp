#include "DirectionPlayer.h"
#include "../../ECS/Coordinator.h"
#include "../KeyInput/KeyboardInput.h"
#include "../../Component/Comp_FaceDirection.h"
#include "../../Component/Comp_Position.h"
#include "../../Math/Vector2D.h"
#include "../../Component/Comp_Texture.h"
#include "../../Component/Comp_Movement.h"
#include "../../Component/Is_Object.h"
#include "../../Misc/MiscFunctions.h"
#include "../../Component/Comp_TextureLight.h"
#include "../../Component/Is_Light.h"
#include "../../Component/Comp_AttachedTo.h"
#include "../Render/Misc/Misc.h"
#include "../Render/Camera/Camera.h"

extern Coordinator gCoordinator;
extern KeyboardInput gKeyboardInput;
extern Camera gCamera;

void SysDirectionPlayer::update()
{
	for (const auto& entity : mEntities)
	{
		auto& position = gCoordinator.GetComponent<Position>(entity);
		auto& direction = gCoordinator.GetComponent<FaceDirection>(entity);
		auto& texture = gCoordinator.GetComponent<Texture>(entity);

		// get mouse coordinates
		Vector2D<float> tmpMouseFloat;
		SDL_GetMouseState(&tmpMouseFloat[0], &tmpMouseFloat[1]);
		gCamera.transformToBaseCoord(tmpMouseFloat);

		// calc angle between player center and mouse position
		Vector2D<float> playerCenter = Vector2D<float>{ position.pos[0] + float(texture.width) / 2.0f, position.pos[1] + float(texture.height) / 2.0f };
		float angle = calc_angle(playerCenter, tmpMouseFloat, true);
		direction.angle = angle;

		auto& pinkPos = gCoordinator.GetComponent<Position>(direction.Entity);
		pinkPos.pos = position.pos + Vector2D<float> {32.0f * std::cos(direction.angle), -32.0f * std::sin(direction.angle) } + Vector2D<float>{16.0f, 16.0f};

		static int test = 0;
		if (gKeyboardInput.getButtonState(SDL_SCANCODE_L) == eButtonState::RELEASED)
		{
			test++;
			if (test < 2)
			{

				Entity tmpBlast = gCoordinator.CreateEntity();
				gCoordinator.AddComponent<Position>(tmpBlast, Position{ position.pos });
				gCoordinator.AddComponent<Texture>(tmpBlast, Texture{ "Picture/Blast.png", 20, 10, 1 , - direction.angle * 180/M_PI });
				gCoordinator.AddComponent<FaceDirection>(tmpBlast, FaceDirection{ direction.angle });
				gCoordinator.AddComponent<Movement>(tmpBlast, Movement{ 200.0f });
				gCoordinator.AddComponent<IsObject>(tmpBlast, IsObject{});

				Entity tmpBlastLight = gCoordinator.CreateEntity();
				gCoordinator.AddComponent<Position>(tmpBlastLight, Position{ position.pos });
				//gCoordinator.AddComponent<TextureLight>(tmpBlastLight, TextureLight{ "Picture/blastLight.png", 200, 100, 1, -direction.angle * 180 / M_PI, ColorValues{255, 1, 237} });
				gCoordinator.AddComponent<TextureLight>(tmpBlastLight,
					TextureLight{ std::make_tuple<int, int, float, float, float>(360, 360, 100.0f, 25.0f, 0), generateGaussianLightTwoSigma,
					360, 360,  -direction.angle * 180 / M_PI , {255, 1, 237} });
				gCoordinator.AddComponent<IsLight>(tmpBlastLight, IsLight{ true });
				gCoordinator.AddComponent<AttachedTo>(tmpBlastLight, AttachedTo{ true, tmpBlast });
			}
		}
	}
}

void SysDirectionPlayer::makeMarker()
{
	for (const auto& entity : mEntities)
	{
		auto& direction = gCoordinator.GetComponent<FaceDirection>(entity);
		auto& position = gCoordinator.GetComponent<Position>(entity);

		direction.Entity = gCoordinator.CreateEntity();
		gCoordinator.AddComponent<Position>(direction.Entity, Position{ position.pos + Vector2D<float>(32.0f, 0.0f) });
		gCoordinator.AddComponent<Texture>(direction.Entity, Texture{ "Picture/FullPink.png", 6, 6, 1 });
	}
}