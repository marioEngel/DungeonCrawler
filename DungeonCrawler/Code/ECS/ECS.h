#pragma once
#include <iostream>
#include <bitset>
#include <queue>
#include <array>
#include <unordered_map>
#include <set>
#include <assert.h>

// Type alias
using Entity = std::uint8_t;
using ComponentType = std::uint8_t;

// Max Entities, Components 
const Entity MAX_ENTITIES = 250;
const ComponentType MAX_COMPONENTS = 64;

using Signature = std::bitset<MAX_COMPONENTS>;