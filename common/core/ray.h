#ifndef RAY_H
#define RAY_H

#include "vec3.h"

enum class RayType
{
    invalid,
    camera,
    shadow,
    reflection,
    refractive
};

class Ray
{
public:
    vec3 o; // origin
    vec3 d; // direction
    RayType type;
    int pathDepth;

    Ray(const vec3& o, const vec3& d) : o(o), d(d) {};
};

#endif // RAY_H
