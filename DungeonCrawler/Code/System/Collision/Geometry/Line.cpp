#include "Line.h"
#include "../../../Math/GeneralMathStuff.h"

Line::Line() :
	mStartPoint{ Vector2D<float> {0.0f, 0.0f} },
	mStoppPoint{ Vector2D<float> {1.0f, 1.0f} },
	mLinestyle{ eLineStyle::NORMAL }
{
	mSlope_m = (mStoppPoint[1] - mStartPoint[1]) / (mStoppPoint[0] - mStartPoint[0]);
	mOffset_t = mStartPoint[1] - mSlope_m * mStartPoint[0];
}

Line::Line(Vector2D<float> startPoint, Vector2D<float> stoppPoint) :
	mStartPoint{ startPoint },
	mStoppPoint{ stoppPoint }
{
	if (floatsEqual(startPoint[0], stoppPoint[0]))
	{
		mLinestyle = eLineStyle::VERTICAL;
		mSlope_m = 0;
		mOffset_t = 0;

	}
	else if (floatsEqual(startPoint[1], stoppPoint[1]))
	{
		mLinestyle = eLineStyle::HORIZONTAL;
		mSlope_m = 0;
		mOffset_t = startPoint[1];
	}
	else
	{
		mLinestyle = eLineStyle::NORMAL;
		mSlope_m = (mStoppPoint[1] - mStartPoint[1]) / (mStoppPoint[0] - mStartPoint[0]);
		mOffset_t = mStartPoint[1] - mSlope_m * mStartPoint[0];
	}
}

Line::~Line()
{
}

eLineStyle Line::return_Linestyle() const
{
	return mLinestyle;
}

Vector2D<float> Line::return_StartPoint() const
{
	return mStartPoint;
}

Vector2D<float> Line::return_StoppPoint() const
{
	return mStoppPoint;
}

float Line::return_Slope_m() const
{
	return mSlope_m;
}

float Line::return_Offset_t() const
{
	return mOffset_t;
}

void Line::printData() const
{
	std::cout << "Style: " << mLinestyle << '\n';
	std::cout << "Start: " << mStartPoint << '\n';
	std::cout << "Stopp: " << mStoppPoint << '\n';
	std::cout << "m: " << mSlope_m << '\n';
	std::cout << "t: " << mOffset_t << '\n';

}