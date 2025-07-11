#include "material.h"

Material::Material()
    : albedo(1.0f, 1.0f, 1.0f), type(MaterialType::Diffuse), smoothShading(false) {}

Material::Material(const Color& albedo, MaterialType type, bool doSmoothShading)
    : albedo(albedo), type(type),  smoothShading(doSmoothShading) {}
