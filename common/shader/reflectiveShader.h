#ifndef REFLECTIVESHADER_H
#define REFLECTIVESHADER_H

#include "color.h"
#include "scene.h"
#include "intersectionData.h"

Color reflectiveShader(const Ray& ray, IntersectionData& intersectData, Scene& scene, int max_depth);

#endif // REFLECTIVESHADER_H
