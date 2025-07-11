#include "color.h"
#include "clamp.h"
#include <cassert>
#include <iostream>

Color::Color(float r, float g, float b)
{
    double EPSILON = 1e-6;
    // colors are always stored in the range 0 to 1
    assert(r > -EPSILON && r < 1 + EPSILON);
    assert(g > -EPSILON && g < 1 + EPSILON);
    assert(b > -EPSILON && b < 1 + EPSILON);

    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color() : r(1.0), g(1.0), b(1.0)
{}

Color Color::operator*(float scalar) const
{
    return Color(clamp(r * scalar, 0.0f, 1.0f), clamp(g * scalar, 0.0f, 1.0f), clamp(b * scalar, 0.0f, 1.0f));
}

Color operator*(float scalar, const Color& c)
{
    return c * scalar;
}

Color Color::operator+(const Color& other)
{
    return Color(clamp(r + other.r, 0.0f, 1.0f), clamp(g + other.g, 0.0f, 1.0f), clamp(b + other.b, 0.0f, 1.0f));
}
