#pragma once
#include <SDL3/SDL.h>
#include <string>
#include "ECS/Coordinator.h"
// ----------Misc----------------------
#include "System/KeyInput/KeyboardInput.h"
#include "System/KeyInput/Mouse.h"
#include "System/Dungeon/Mine/Dungeon.h"
#include "System/Render/Misc/Misc.h"
#include "Misc/MiscFunctions.h"
#include "Characters/CharacterTemplate.h"
#include "Math/Matrix.h"
#include "System/Render/Camera/Camera.h"
// ---------Components------------------ max 64
#include "Component/Comp_Affiliation.h"
#include "Component/Comp_FaceDirection.h"
#include "Component/Comp_Hitbox.h"
#include "Component/Comp_InputKeys.h"
#include "Component/Is_Player.h"					// 5
#include "Component/Is_Object.h"
#include "Component/Is_Collision.h"
#include "Component/Is_Light.h"
#include "Component/Comp_Mass.h"
#include "Component/Comp_Movement.h"				// 10
#include "Component/Comp_DirectionDecision.h"
#include "Component/Comp_Position.h"
#include "Component/Comp_Texture.h"
#include "Component/Comp_TextureLight.h"
#include "Component/Comp_TileMap.h"					// 15
#include "Component/Comp_AttachedTo.h"
#include "Component/Is_UI.h"
#include "Component/Comp_Text.h"
#include "Component/Comp_DisplayFPS.h"
#include "Component/Comp_Level.h"					// 20
#include "Component/Comp_MovementTile.h"
// --------Systems---------------------- 
#include "System/Collision/Collision.h"
#include "System/Movement/DirectionPlayer.h"
#include "System/Movement/MovementPlayer.h"
#include "System/Movement/MovementPlayerDecision.h"
#include "System/Movement/MovementObject.h"
#include "System/Movement/MovementObjectAttached.h"
#include "System/Render/RendererSprite.h"
#include "System/Render/RendererTileMap.h"
#include "System/Render/RendererLight.h"
#include "System/Render/RendererUI.h"
#include "System/Render/UI/DisplayFPS.h"
#include "System/Dungeon/Mine/Level.h"
#include "System/Movement/TileMap/MovementTile.h"
#include "System/Movement/TileMap/MovementTileDecision.h"


class Game
{
public:
	Game();
	~Game();

	bool gameRunning();
	void clean();
	void initECS(const char* text, int width, int height, int flag = 0);
	//void makeEnd();

	void initEntities();
	void update(float delta);
	void handleEvents();

	static SDL_Renderer* renderer;
	static SDL_Event event;
	static int screenWidth;
	static int screenHeight;

private:
	bool isRunning = true;
	SDL_Window* window;
};
