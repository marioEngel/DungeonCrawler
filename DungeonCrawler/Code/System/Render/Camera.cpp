#include "Camera.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/Comp_Position.h"
#include "../Collision/Collision.h"

extern Coordinator gCoordinator;

Camera::Camera()
{
	mCamera.x = 0.0f;
	mCamera.y = 0.0f;
	mCamera.w = 800.0f;
	mCamera.h = 600.0f;

	mCameraEdge.x = 50.0f;
	mCameraEdge.y = 50.0f;
	mCameraEdge.w = 700.0f;
	mCameraEdge.h = 500.0f;

}

Camera::~Camera()
{
}

void Camera::CheckCollision(Entity player)
{
	auto& position = gCoordinator.GetComponent<Position>(player);

	//std::cout << position.pos << '\n';

	if (check_RectVsPoint(mCamera, position.pos) && !check_RectVsPoint(mCameraEdge, position.pos))
	{
		if (int(position.pos[0]) < mCameraEdge.x)
		{
			mCamera.x -= (mCameraEdge.x - int(position.pos[0]));
			mCameraEdge.x -= (mCameraEdge.x - int(position.pos[0]));
		}
		else if (int(position.pos[0]) > (mCameraEdge.x + mCameraEdge.w))
		{
			mCamera.x += (int(position.pos[0]) - (mCameraEdge.x + mCameraEdge.w));
			mCameraEdge.x += (int(position.pos[0]) - (mCameraEdge.x + mCameraEdge.w));
		}

		if (int(position.pos[1]) < mCameraEdge.y)
		{
			mCamera.y -= (mCameraEdge.y - int(position.pos[1]));
			mCameraEdge.y -= (mCameraEdge.y - int(position.pos[1]));
		}
		else if (int(position.pos[1]) > (mCameraEdge.y + mCameraEdge.h))
		{
			mCamera.y += (int(position.pos[1]) - (mCameraEdge.y + mCameraEdge.h));
			mCameraEdge.y += (int(position.pos[1]) - (mCameraEdge.y + mCameraEdge.h));
		}
	}
}


//Camera::Camera(int width, int hight)
//{
//	mCamera.x = 0;
//	mCamera.y = 0;
//	mCamera.w = width;
//	mCamera.h = hight;
//}
//
//Camera::Camera(int width, int hight, int xpos, int ypos)
//{
//	mCamera.x = xpos;
//	mCamera.y = ypos;
//	mCamera.w = width;
//	mCamera.h = hight;
//}