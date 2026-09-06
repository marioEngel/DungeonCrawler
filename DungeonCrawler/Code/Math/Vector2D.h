#pragma once
#include <iostream>
#include <array>
#include <assert.h>

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
	
	// operator overloading
	friend Vector2D operator+(const Vector2D& firstVector, const Vector2D& secondVector)
	{
		return Vector2D(firstVector.x + secondVector.x, firstVector.y + secondVector.y);
	}
	friend Vector2D operator-(const Vector2D& firstVector, const Vector2D& secondVector)
	{
		return Vector2D(firstVector.x - secondVector.x, firstVector.y - secondVector.y);
	}
	friend std::ostream& operator<<(std::ostream& out, const Vector2D& vec2D)
	{
		out << "Vec2D(" << vec2D.x << ", " << vec2D.y << ") ";
		return out;
	}
	friend Vector2D operator*(T scale, const Vector2D& vec2D)
	{
		return Vector2D(scale * vec2D.x, scale * vec2D.y);
	}
	friend Vector2D operator*(const Vector2D& vec2D, T scale)
	{
		return Vector2D(scale * vec2D.x, scale * vec2D.y);
	}
	friend bool operator==(const Vector2D& firstVector, const Vector2D& secondVector) {
		return (firstVector.x == secondVector.x && firstVector.y == secondVector.y);
	}


	T operator[](int index) const
	{
		assert(index == 0 || index == 1);
		return (index == 0) ? x : y;
	}
	T& operator[](int index)
	{
		assert(index == 0 || index == 1);
		return index == 0 ? x : y;
	}

	T x;
	T y;
};

template <typename T>
Vector2D<T>::Vector2D()
{
	x = T(0);
	y = T(0);
}

template <typename T>
Vector2D<T>::Vector2D(T xInput, T yInput)
{
	x = xInput;
	y = yInput;
}

template <typename T>
Vector2D<T>::~Vector2D()
{
}

template <typename T>
void Vector2D<T>::normalize()
{
	T scale = std::sqrt(this->x * this->x + this->y * this->y);

	if (scale != 0.0)
	{
		this->x /= scale;
		this->y /= scale;
	}
}

template <typename T>
T Vector2D<T>::calc_amountSquared()
{
	return x * x + y * y;
}

template <typename T>
T Vector2D<T>::calc_amount()
{
	return std::sqrt(x * x + y * y);
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
	if (x == 0)
	{
		std::cout << "!!! Division by 0 in scaleToX !!!" << std::endl;
	}
	T superScale = xScale / x;
	x *= superScale;
	y *= superScale;
}

template <typename T>
void Vector2D<T>::scaleToY(T yScale)
{
	if (y == 0)
	{
		std::cout << "!!! Division by 0 in scaleToY !!!" << std::endl;
	}
	T superScale = yScale / y;
	x *= superScale;
	y *= superScale;
}

template <typename T> 
Vector2D<T> Vector2D<T>::flip()
{
	return Vector2D<T>(-x, -y);
}

template <typename T>
Vector2D<T> Vector2D<T>::flipX()
{
	return Vector2D<T>(-x, y);
}

template <typename T>
Vector2D<T> Vector2D<T>::flipY()
{
	return Vector2D<T>(x, -y);
}
