#ifndef MATERIAL_H
#define MATERIAL_H


#include "vec3.h"
#include "color.h"

enum class MaterialType {
    Diffuse,
    Reflective,
    Refractive,
    Phong
};

class Material {
public:
    Color albedo;
    MaterialType type;
    bool smoothShading;

    Material();
    Material(const Color& albedo, MaterialType type, bool doSmoothShading);
};

#endif // MATERIAL_H
