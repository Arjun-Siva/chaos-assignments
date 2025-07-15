#ifndef MATERIAL_H
#define MATERIAL_H


#include "vec3.h"
#include "color.h"

enum class MaterialType {
    Diffuse,
    Reflective,
    Refractive,
    Constant
};

class Material {
public:
    Color albedo;
    MaterialType type;
    bool smoothShading;
    float ior;

    Material();
    Material(const Color& albedo, MaterialType type, bool doSmoothShading);
    Material(const Color& albedo, MaterialType type, bool doSmoothShading, float ior);
};

#endif // MATERIAL_H
