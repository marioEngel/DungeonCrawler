#include "MovementObjectAttached.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/Comp_AttachedTo.h"
#include "../../Component/Comp_Position.h"
#include "../../Component/Comp_Texture.h"
#include "../../Component/Comp_TextureLight.h"
#include "../../Misc/MiscFunctions.h"

extern Coordinator gCoordinator;

void SysMovementObjectAttached::update()
{
	for (const auto& entity : mEntities)
	{
		auto& attachment = gCoordinator.GetComponent<AttachedTo>(entity);
		auto& positionAttach = gCoordinator.GetComponent<Position>(entity);
		auto& textureAttach = gCoordinator.GetComponent<TextureLight>(entity);
		auto& positionSource = gCoordinator.GetComponent<Position>(attachment.attachedEntity);
		auto& textureSource = gCoordinator.GetComponent<Texture>(attachment.attachedEntity);

		positionAttach.pos =  positionAttach.pos
							+ rtnCenter(positionAttach, textureAttach).flip() // flip = *(-1)
							+ rtnCenter(positionSource, textureSource);
	}
}