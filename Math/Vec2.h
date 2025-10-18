#pragma once
#include <cmath>
#include <glm/vec2.hpp>
#include <stdexcept>


template <typename T>
class Vec2
{

public:
    Vec2() = default;
    T x{};
    T y{};
    Vec2(T x_val, T y_val): x(x_val), y(y_val){}
    Vec2(T scalar): x(scalar), y(scalar){}

    operator glm::vec2() const
    {
        return glm::vec2(x, y);
    }

    operator Vec2() const
    {
        return Vec2(x, y);
    }
    Vec2 operator+(Vec2 rhs)
    {
        return Vec2{x + rhs.x, y + rhs.y};
    }

    Vec2 operator-(Vec2 rhs)
    {
        return Vec2{x - rhs.x, y - rhs.y};
    }

    Vec2 operator*(float scalar)
    {
        return Vec2{x * scalar, y * scalar};
    }

    Vec2 operator/(float scalar)
    {
        return Vec2(x / scalar, y / scalar);
    }

    T operator[](int index)
    {
        // Returns z for positive index values
        return index == 1 ? x : index == 2 ? y : throw std::out_of_range("Index out of range");
    }

    float getMagnitude()
    {
        return std::sqrt(x * x + y * y);
    }

    // Modifies the current object-
    const Vec2& normalize()
    {
        float magnitude = getMagnitude();
        x /= magnitude;
        y /= magnitude;
        return *this;
    }

    // Returns normalized variant of the current object
    Vec2 getNormalized()
    {
        float magnitude = getMagnitude();
        return Vec2(x/magnitude, y/magnitude);
    }


    static float dot(Vec2 lhs, Vec2 rhs)
    {
        return ((lhs.x * rhs.x) + (lhs.y * rhs.y));
    }

};

// Defining using directives for ease of use
using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;