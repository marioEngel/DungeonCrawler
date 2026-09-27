#include "Camera.h"
#include "../../../ECS/Coordinator.h"
#include "../../../Component/Comp_Position.h"
#include "../../../Component/Comp_Texture.h"
#include "../../Collision/Collision.h"

extern Coordinator gCoordinator;

Camera::Camera()
{
	mCamera.x = 0.0f;
	mCamera.y = 0.0f;
	mCamera.w = 800.0f;
	mCamera.h = 600.0f;

	mEdgeWidth = 150.0f;
	mEdgeHight = 150.0f;
}

Camera::~Camera()
{
}

void Camera::CheckCollision(Entity entity)
{
	auto& position = gCoordinator.GetComponent<Position>(entity);

	if (check_RectVsPoint(mCamera, position.vec))
	{
		if (int(position.vec.x) < (mCamera.x + mEdgeWidth))
		if (int(position.vec.x) < (mCamera.x + mEdgeWidth))
		{
			mCamera.x -= ((mCamera.x + mEdgeWidth) - int(position.vec.x));
		}
		else if (int(position.vec.x) > (mCamera.x + mCamera.w - mEdgeWidth))
		{
			mCamera.x += (int(position.vec.x) - (mCamera.x + mCamera.w - mEdgeWidth));
		}

		if (int(position.vec.y) < (mCamera.y + mEdgeHight))
		{
			mCamera.y -= ((mCamera.y + mEdgeHight) - int(position.vec.y));
		}
		else if (int(position.vec.y) > (mCamera.y + mCamera.h - mEdgeHight))
		{
			mCamera.y += (int(position.vec.y) - (mCamera.y + mCamera.h - mEdgeHight));
		}
	}
}

void Camera::center(Entity entity)
{
	auto& pos = gCoordinator.GetComponent<Position>(entity);
	auto& texture = gCoordinator.GetComponent<Texture>(entity);

	float centerX = pos.vec.x + texture.width / 2;
	float centerY = pos.vec.y + texture.height / 2;

	mCamera.x = centerX - mCamera.w / 2;
	mCamera.y = centerY - mCamera.h / 2;
}

void Camera::transformToBaseCoord(Vector2D<float>& cameraCoord)
{
	Vector2D<float> camerePos{ mCamera.x, mCamera.y };
	cameraCoord = cameraCoord + camerePos;
}