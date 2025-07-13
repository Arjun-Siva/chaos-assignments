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

    Ray(const vec3& o, const vec3& d, RayType type, int pathDepth) : o(o), d(d), type(type), pathDepth(pathDepth) {};

    Ray reflectedRay(vec3& normal, vec3& point) const
    {
        vec3 newD = this->d - (normal * (this->d.dot(normal)) * 2);
        return Ray(point, newD.normalized(), this->type, this->pathDepth + 1);
    }
};

#endif // RAY_H
