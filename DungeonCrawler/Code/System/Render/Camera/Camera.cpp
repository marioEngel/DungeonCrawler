#include "Camera.h"
#include "../../../ECS/Coordinator.h"
#include "../../../Component/Comp_Position.h"
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

void Camera::CheckCollision(Entity player)
{
	auto& position = gCoordinator.GetComponent<Position>(player);

	//std::cout << position.pos << '\n';

	if (check_RectVsPoint(mCamera, position.pos))
	{
		if (int(position.pos[0]) < (mCamera.x + mEdgeWidth))
		{
			mCamera.x -= ((mCamera.x + mEdgeWidth) - int(position.pos[0]));
		}
		else if (int(position.pos[0]) > (mCamera.x + mCamera.w - mEdgeWidth))
		{
			mCamera.x += (int(position.pos[0]) - (mCamera.x + mCamera.w - mEdgeWidth));
		}

		if (int(position.pos[1]) < (mCamera.y + mEdgeHight))
		{
			mCamera.y -= ((mCamera.y + mEdgeHight) - int(position.pos[1]));
		}
		else if (int(position.pos[1]) > (mCamera.y + mCamera.h - mEdgeHight))
		{
			mCamera.y += (int(position.pos[1]) - (mCamera.y + mCamera.h - mEdgeHight));
		}
	}
}

void Camera::transformToBaseCoord(Vector2D<float>& cameraCoord)
{
	Vector2D<float> camerePos{ mCamera.x, mCamera.y };
	cameraCoord = cameraCoord + camerePos;
}