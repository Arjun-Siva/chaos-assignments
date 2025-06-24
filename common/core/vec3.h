#pragma once

#include <cmath>

class vec3 {
public:
    double x, y, z;

    vec3();
    vec3(double x, double y, double z);

    vec3 operator+(const vec3& v) const;
    vec3 operator-(const vec3& v) const;
    vec3 operator*(double t) const;
    vec3 operator/(double t) const;

    double dot(const vec3& v) const;
    vec3 cross(const vec3& v) const;
    vec3 normalized() const;
    double length() const;
};
