#pragma once
#include <iostream>
#include <array>
# define M_PI 3.14159265358979323846f  /* pi */

// all function have to be in the header file for reasons
// otherwise linking error

template <typename T>
class Vector2D
{
public:
	Vector2D();
	Vector2D(T xInput, T yInput);
	~Vector2D();

	T calc_amountSquared();
	T calc_amount();

	void normalize();
	void scaleToX(T xScale);
	void scaleToY(T yScale);
	Vector2D<T> flip();
	Vector2D<T> flipX();
	Vector2D<T> flipY();

	T returnXval();
	T returnYval();
	
	// operator overloading
	friend Vector2D operator+(const Vector2D& firstVector, const Vector2D& secondVector)
	{
		return Vector2D(firstVector.mVecVal[0] + secondVector.mVecVal[0], firstVector.mVecVal[1] + secondVector.mVecVal[1]);
	}
	friend Vector2D operator-(const Vector2D& firstVector, const Vector2D& secondVector)
	{
		return Vector2D(firstVector.mVecVal[0] - secondVector.mVecVal[0], firstVector.mVecVal[1] - secondVector.mVecVal[1]);
	}
	friend std::ostream& operator<<(std::ostream& out, const Vector2D& vec2D)
	{
		out << "Vec2D(" << vec2D.mVecVal[0] << ", " << vec2D.mVecVal[1] << ") ";
		return out;
	}
	friend Vector2D operator*(T scale, const Vector2D& vec2D)
	{
		return Vector2D(scale * vec2D.mVecVal[0], scale * vec2D.mVecVal[1]);
	}
	friend Vector2D operator*(const Vector2D& vec2D, T scale)
	{
		return Vector2D(scale * vec2D.mVecVal[0], scale * vec2D.mVecVal[1]);
	}
	T operator[](int index) const
	{
		return mVecVal[index];
	}
	T& operator[](int index)
	{
		return mVecVal[index];
	}

private:
	std::array<T, 2> mVecVal{};
};

template <typename T>
Vector2D<T>::Vector2D()
{
	mVecVal = std::array<T, 2> {T(0.0), T(0.0)};
}

template <typename T>
Vector2D<T>::Vector2D(T xInput, T yInput)
{
	mVecVal = std::array<T, 2> {xInput, yInput};
}

template <typename T>
Vector2D<T>::~Vector2D()
{
}

template <typename T>
void Vector2D<T>::normalize()
{
	T scale = std::sqrt(this->mVecVal[0] * this->mVecVal[0] + this->mVecVal[1] * this->mVecVal[1]);

	if (scale != 0.0)
	{
		this->mVecVal[0] /= scale;
		this->mVecVal[1] /= scale;
	}
}

template <typename T>
T Vector2D<T>::calc_amountSquared()
{
	return mVecVal[0] * mVecVal[0] + mVecVal[1] * mVecVal[1];
}

template <typename T>
T Vector2D<T>::calc_amount()
{
	return std::sqrt(mVecVal[0] * mVecVal[0] + mVecVal[1] * mVecVal[1]);
}

// returns angle in rad
template <typename T>
float calc_angle(Vector2D<T> vectorStart, Vector2D<T> vectorEnd, bool flip)
{
	Vector2D<T> tmpUnitVec = vectorEnd - vectorStart;
	tmpUnitVec.normalize();
	float rtnAngle;

	if (flip)
	{
		tmpUnitVec[1] = -tmpUnitVec[1];
	}

	if (tmpUnitVec[0] > 0.0)
	{
		// unten rechts
		if (tmpUnitVec[1] < 0.0)
		{
			rtnAngle = 2.0f * M_PI + std::atan(tmpUnitVec[1] / tmpUnitVec[0]);
		}
		// oben rechts
		else
		{
			rtnAngle = std::atan(tmpUnitVec[1] / tmpUnitVec[0]);
		}
	}
	else
	{
		// unten links
		if (tmpUnitVec[1] < 0.0)
		{
			rtnAngle = M_PI + std::atan(tmpUnitVec[1] / tmpUnitVec[0]);
		}
		// oben links
		else
		{
			rtnAngle = M_PI + std::atan(tmpUnitVec[1] / tmpUnitVec[0]);
		}
	}

	while (rtnAngle > 2.0f * M_PI)
	{
		rtnAngle -= 2.0f * M_PI;
	}

	return rtnAngle;
}

// return vector with x-/y-values normalized to +1/-1
template <typename T> 
Vector2D<T> directionNorm(Vector2D<T> vect)
{
	T tmpX = T(0);
	T tmpY = T(0);

	if (vect[0] > T(0))
	{
		tmpX = T(1);
	}
	else if (vect[0] < T(0))
	{
		tmpX = T(-1);
	}
	else
	{
		tmpX = T(0);
	}

	if (vect[1] > T(0))
	{
		tmpY = T(1);
	}
	else if (vect[1] < T(0))
	{
		tmpY = T(-1);
	}
	else
	{
		tmpY = T(0);
	}

	return Vector2D<T> {tmpX, tmpY};
}

template <typename T>
void Vector2D<T>::scaleToX(T xScale)
{
	if (mVecVal[0] == 0)
	{
		std::cout << "!!! Division by 0 in scaleToX !!!" << std::endl;
	}
	T superScale = xScale / mVecVal[0];
	mVecVal[0] *= superScale;
	mVecVal[1] *= superScale;
}

template <typename T>
void Vector2D<T>::scaleToY(T yScale)
{
	if (mVecVal[1] == 0)
	{
		std::cout << "!!! Division by 0 in scaleToY !!!" << std::endl;
	}
	T superScale = yScale / mVecVal[1];
	mVecVal[0] *= superScale;
	mVecVal[1] *= superScale;
}

template <typename T> 
Vector2D<T> Vector2D<T>::flip()
{
	return Vector2D<T>(-mVecVal[0], -mVecVal[1]);
}

template <typename T>
Vector2D<T> Vector2D<T>::flipX()
{
	return Vector2D<T>(-mVecVal[0], mVecVal[1]);
}

template <typename T>
Vector2D<T> Vector2D<T>::flipY()
{
	return Vector2D<T>(mVecVal[0], -mVecVal[1]);
}

template <typename T>
T Vector2D<T>::returnXval()
{
	return mVecVal[0];
}

template <typename T>
T Vector2D<T>::returnYval()
{
	return mVecVal[1];
}