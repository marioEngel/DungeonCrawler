#pragma once
#include "../System/Dungeon/Mine/Dungeon.h"

struct Level 
{
	Dungeon* dungeon;
	bool isActive = false;
};