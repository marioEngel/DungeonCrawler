#pragma once

enum eAffKind
{
	Player,
	Enemy,
	Map
};

struct Affiliation
{
	eAffKind affiliation;
};