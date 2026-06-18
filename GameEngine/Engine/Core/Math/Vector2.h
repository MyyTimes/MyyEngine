#pragma once
#include <cmath>

const static inline float PI = 3.1415926535f;

template <typename T = float> // default: float
struct Vector2
{    
    T x = 0;
    T y = 0;

    // Constructors
    Vector2() = default;
    Vector2(T inX, T inY) : x(inX), y(inY) {}

    float Length() const 
    {
		return std::sqrt(static_cast<float>(x * x + y * y));
    }

    T LengthSquared() const 
    {
        return x * x + y * y;
    }

	Vector2<float> Normalize() const
    {
        float len = Length();
        if (len > 0.0f) 
			return Vector2<float>(static_cast<float>(x) / len, static_cast<float>(y) / len);
		return Vector2<float>(0.0f, 0.0f);
	}

    // Dot product
	T Dot(const Vector2<T>& v) const
	{
        return x * v.x + y * v.y;
    }

    float Distance(const Vector2<T>& v) const 
    {
        return Vector2(x - v.x, y - v.y).Length();
    }

	Vector2<float> RotateRad(float radian) const
	{
		float cosTheta = std::cos(radian);
		float sinTheta = std::sin(radian);
		return Vector2<float>(
			static_cast<float>(x) * cosTheta - static_cast<float>(y) * sinTheta,
			static_cast<float>(x) * sinTheta + static_cast<float>(y) * cosTheta
		);
	}

	Vector2<float> RotateDeg(float degree) const
	{
		return RotateRad(degree * PI / 180.0f);
	}

    // OPERATOR OVERLOADING 
	// Calculating
	Vector2<T> operator+(const Vector2<T>& v) const { return Vector2<T>(x + v.x, y + v.y); }
	Vector2<T> operator-(const Vector2<T>& v) const { return Vector2<T>(x - v.x, y - v.y); }
	Vector2<T> operator*(T scalar) const { return Vector2<T>(x * scalar, y * scalar); }
	Vector2<T> operator/(T scalar) const { return Vector2<T>(x / scalar, y / scalar); }

    // +=, -=, *=
	Vector2<T>& operator+=(const Vector2<T>& v) { x += v.x; y += v.y; return *this; }
	Vector2<T>& operator-=(const Vector2<T>& v) { x -= v.x; y -= v.y; return *this; }
	Vector2<T>& operator*=(T scalar) { x *= scalar; y *= scalar; return *this; }

    // Comparison
    bool operator==(const Vector2<T>& v) const { return (x == v.x && y == v.y); }

    // Assignment
    // For now, it is not necessary
    Vector2<T>& operator=(const Vector2<T>& v)
    {
        if (this == &v) return *this;
        x = v.x;
        y = v.y;
        return *this;
    }
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;
