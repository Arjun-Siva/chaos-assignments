#ifndef CONSTANTSHADER_H
#define CONSTANTSHADER_H

#include "intersectionData.h"
#include "scene.h"
#include "color.h"

Color constantShader(const IntersectionData& intersectData, Scene& scene);

#endif // CONSTANTSHADER_H
