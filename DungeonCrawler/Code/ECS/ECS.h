#pragma once
#include <bitset>
#include <cstdint>

// Type alias
using Entity = std::uint8_t;
using ComponentType = std::uint8_t;

// Max Entities, Components 
const Entity MAX_ENTITIES = 250;
const ComponentType MAX_COMPONENTS = 64;

using Signature = std::bitset<MAX_COMPONENTS>;