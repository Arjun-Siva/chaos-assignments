#include "color.h"
#include "scene.h"
#include "intersectionData.h"
#include "recursiveShader.h"

const double REFLECTION_BIAS = 1e-2;

Color reflectiveShader(const Ray& ray, const IntersectionData& intersectData, Scene& scene, int max_depth)
{
    const Material hitMaterial = *intersectData.material;
    const float albedoR = hitMaterial.albedo.r;
    const float albedoG = hitMaterial.albedo.g;
    const float albedoB = hitMaterial.albedo.b;

    const vec3& normal = (hitMaterial.smoothShading) ? intersectData.interpolatedVertNormal : intersectData.hitPointNormal;
    const Ray reflectedR = ray.reflectedRay(normal, intersectData.hitPoint + (normal * REFLECTION_BIAS));

    const Color reflectedColor = recursiveShader(reflectedR, scene, max_depth);

    return Color(reflectedColor.r * albedoR,  reflectedColor.g * albedoG, reflectedColor.b * albedoB);
}
