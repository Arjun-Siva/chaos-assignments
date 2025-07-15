#ifndef RECURSIVESHADER_H
#define RECURSIVESHADER_H

#include "ray.h"
#include "color.h"
#include "scene.h"

Color recursiveShader(const Ray &ray, Scene& scene, int max_depth);

#endif // RECURSIVESHADER_H
