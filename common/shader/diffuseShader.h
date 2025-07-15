#ifndef DIFFUSESHADER_H
#define DIFFUSESHADER_H

#include "color.h"
#include "scene.h"
#include "intersectionData.h"

Color diffuseShader(const IntersectionData& intersectData, Scene& scene);

#endif // DIFFUSESHADER_H
