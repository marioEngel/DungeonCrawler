#include "Coordinator.h"
#include "ECS/ECS.h"
#include "System/Render/Camera/Camera.h"
#include "System/KeyInput/KeyboardInput.h"

Entity gPlayerEntity{};
Coordinator gCoordinator{};
Camera gCamera{};
KeyboardInput gKeyboardInput{};
float gDeltaT{};
