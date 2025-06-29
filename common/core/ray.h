#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray
{
public:
    Ray(vec3 o, vec3 d);
    vec3 o; // origin
    vec3 d; // direction
};

#endif // RAY_H
