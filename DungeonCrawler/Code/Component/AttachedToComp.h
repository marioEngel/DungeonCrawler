#pragma once
#include "../ECS/ECS.h"

struct AttachedTo
{
	bool isAttached = false;
	Entity attachedEntity = 0;
};