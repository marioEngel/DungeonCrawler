#include "Camera.h"
#include "../../ECS/Coordinator.h"
#include "../../Component/PositionComp.h"
#include "../Collision/Collision.h"

extern Coordinator gCoordinator;

Camera::Camera()
{
	mCamera.x = 0;
	mCamera.y = 0;
	mCamera.w = 800;
	mCamera.h = 600;

	mCameraEdge.x = 50;
	mCameraEdge.y = 50;
	mCameraEdge.w = 700;
	mCameraEdge.h = 500;

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