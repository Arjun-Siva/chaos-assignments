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
    Constant,
    Phong
};

class Material {
public:
    std::shared_ptr<Texture> albedoTex;
    MaterialType type;
    bool smoothShading;
    float ior; // refractive index
    int shininess; // exponent in phong
    float ambientLightIntensity;

    //phong parameters
    float ka; //ambient coefficient
    float kd; //diffuse
    float ks; //specular

    Material();
    Material(std::shared_ptr<Texture> texture, MaterialType type, bool doSmoothShading);
    Material(std::shared_ptr<Texture> texture, MaterialType type, bool doSmoothShading, float ior);
};

#endif // MATERIAL_H
