#include "material.h"

Material::Material()
    : albedoTex(nullptr), type(MaterialType::Diffuse), smoothShading(false), ior(1), shininess(0), ambientLightIntensity(0), ka(0), kd(0), ks(0) {}

Material::Material(std::shared_ptr<Texture> texture, MaterialType type, bool doSmoothShading)
    : albedoTex(texture), type(type),  smoothShading(doSmoothShading), ior(1), shininess(0), ambientLightIntensity(0), ka(0), kd(0), ks(0) {}

Material::Material(std::shared_ptr<Texture> texture, MaterialType type, bool doSmoothShading, float ior)
    : albedoTex(texture), type(type),  smoothShading(doSmoothShading), ior(ior), shininess(0), ambientLightIntensity(0), ka(0), kd(0), ks(0) {}
