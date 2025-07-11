#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
public:
    vec3 o; // origin
    vec3 d; // direction

    Ray(const vec3& o, const vec3& d) : o(o), d(d) {};
};

#endif // RAY_H
