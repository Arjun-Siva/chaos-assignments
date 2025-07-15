#include "material.h"

Material::Material()
    : albedo(1.0f, 1.0f, 1.0f), type(MaterialType::Diffuse), smoothShading(false), ior(1) {}

Material::Material(const Color& albedo, MaterialType type, bool doSmoothShading)
    : albedo(albedo), type(type),  smoothShading(doSmoothShading), ior(1) {}

Material::Material(const Color& albedo, MaterialType type, bool doSmoothShading, float ior)
    : albedo(albedo), type(type),  smoothShading(doSmoothShading), ior(ior) {}
