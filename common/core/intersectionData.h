#ifndef INTERSECTIONDATA_H
#define INTERSECTIONDATA_H

#include "vec3.h"
#include "material.h"
#include "baryCoord.h"

struct IntersectionData
{
    vec3 hitPoint;
    vec3 hitPointNormal; //triangle face normal
    vec3 interpolatedVertNormal; // interpolated normal from barycentric coords
    BaryCoord baryCentricCoords;
    const Material* material;
    int objectIdx = -1;
    int triangleIdx = -1;
};

#endif // INTERSECTIONDATA_H
