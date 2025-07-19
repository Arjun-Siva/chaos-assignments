#ifndef MATERIAL_H
#define MATERIAL_H

#include <memory>

#include "vec3.h"
#include "color.h"
#include "texture.h"

enum class MaterialType {
    Diffuse,
    Reflective,
    Refractive,
    Constant
};

class Material {
public:
    std::shared_ptr<Texture> albedoTex;
    MaterialType type;
    bool smoothShading;
    float ior;

    Material();
    Material(std::shared_ptr<Texture> texture, MaterialType type, bool doSmoothShading);
    Material(std::shared_ptr<Texture> texture, MaterialType type, bool doSmoothShading, float ior);
};

#endif // MATERIAL_H
