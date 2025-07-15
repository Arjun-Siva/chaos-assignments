#ifndef REFRACTIVESHADER_H
#define REFRACTIVESHADER_H

#include "color.h"
#include "scene.h"
#include "intersectionData.h"

Color refractiveShader(const Ray& ray, const IntersectionData& intersectData, Scene& scene, int max_depth);

#endif // REFRACTIVESHADER_H
