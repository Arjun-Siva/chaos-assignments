#ifndef DIFFUSESHADER_H
#define DIFFUSESHADER_H

#include <cmath>
#include <cstdlib>

#include "color.h"
#include "scene.h"
#include "intersectionData.h"

Color diffuseShader(const Ray& ray, IntersectionData& intersectData, Scene& scene, int max_depth);

#endif // DIFFUSESHADER_H
