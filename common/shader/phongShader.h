#ifndef PHONGSHADER_H
#define PHONGSHADER_H
#include "color.h"
#include "scene.h"
#include "intersectionData.h"

Color phongShader(IntersectionData& intersectData, Scene& scene);

#endif // PHONGSHADER_H
