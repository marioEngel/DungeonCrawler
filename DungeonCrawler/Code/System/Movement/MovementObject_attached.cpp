#include "MovementObject_attached.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/AttachedToComp.h"
#include "../../Component/PositionComp.h"
#include "../../Component/TextureComp.h"
#include "../../Component/TextureLightComp.h"
#include "../../Misc/MistFunctions.h"

extern Coordinator gCoordinator;

void MovementObject_attachedSystem::update()
{
	for (auto& const entity : mEntities)
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