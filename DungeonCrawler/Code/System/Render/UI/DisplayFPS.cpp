#include "DisplayFPS.h"
#include "../../../ECS/Coordinator.h"
#include "../../../Component/Comp_DisplayFPS.h"
#include "../../../Component/Comp_Text.h"

extern Coordinator gCoordinator;

void SyDisplayFPS::update(float deltaT)
{
    for (auto const& entity : mEntities)
    {
        auto& fpsDisplay = gCoordinator.GetComponent<DisplayFPS>(entity);
        fpsDisplay.updateTimer += deltaT;

        if (fpsDisplay.updateTimer >= fpsDisplay.updateInterval)
        {
            fpsDisplay.updateTimer = 0.0f;

            auto& text = gCoordinator.GetComponent<Text>(entity);
            int fps = (int)(1.0f / deltaT);
            text.text = std::to_string(fps); 
            text.reloadTexture = true;
        }
    }
}