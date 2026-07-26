#pragma once
#include "../System/Dungeon/Dungeon.h"

struct Level 
{
	Dungeon* dungeon;
	bool isActive = false;
};