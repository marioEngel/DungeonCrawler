#pragma once
#include "../ECS/System.h"

class ConversePositionSystem : public Systems
{
public:
	// Position values to matrix values
	void conversePM(); 
	// the other way around
	void converseMP();
};

