#pragma once
#include <cmath>

const static inline float PI = 3.1415926535f;

struct Vector2
{    
    float x = 0.0f;
    float y = 0.0f;

    // Constructors
    Vector2() = default;
    Vector2(float inX, float inY) : x(inX), y(inY) {}

    float Length() const 
    {
        return std::sqrt(x * x + y * y);
    }

    float LengthSquared() const 
    {
        return x * x + y * y;
    }

    Vector2 Normalize() const 
    {
        float len = Length();
        if (len > 0.0f) 
            return Vector2(x / len, y / len);
        return Vector2(0.0f, 0.0f);
    }

    // Dot product
    float Dot(const Vector2& v) const 
    {
        return x * v.x + y * v.y;
    }

    float Distance(const Vector2& v) const 
    {
        return Vector2(x - v.x, y - v.y).Length();
    }

    Vector2 RotateRad(float radian) const 
    {
        float cosTheta = std::cos(radian);
        float sinTheta = std::sin(radian);
        return Vector2(
            x * cosTheta - y * sinTheta,
            x * sinTheta + y * cosTheta
        );
    }

    Vector2 RotateDeg(float degree) const
    {
        return RotateRad(degree * PI / 180.0f);
    }

    // OPERATOR OVERLOADING 

    // Addition 
    Vector2 operator+(const Vector2& v) const { return Vector2(x + v.x, y + v.y); }
    // Subtraction
    Vector2 operator-(const Vector2& v) const { return Vector2(x - v.x, y - v.y); }
    // Scalar Multiplication
    Vector2 operator*(float scalar) const { return Vector2(x * scalar, y * scalar); }
    // Scalar Division
    Vector2 operator/(float scalar) const { return Vector2(x / scalar, y / scalar); }

    // +=, -=, *=
    Vector2& operator+=(const Vector2& v) { x += v.x; y += v.y; return *this; }
    Vector2& operator-=(const Vector2& v) { x -= v.x; y -= v.y; return *this; }
    Vector2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }

    // Comparison
    bool operator==(const Vector2& v) const { return (x == v.x && y == v.y); }

    // Assignment
    // For now, it is not necessary
    Vector2& operator=(const Vector2& v)
    {
        if (this == &v) return *this;
        x = v.x;
        y = v.y;
        return *this;
    }
};