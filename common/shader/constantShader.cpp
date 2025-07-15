#include "color.h"
#include "scene.h"
#include "intersectionData.h"

Color constantShader(const IntersectionData& intersectData, Scene& scene)
{
    const Material hitMaterial = *intersectData.material;

    const Color meshColor = scene.geometryObjects[intersectData.objectIdx].uniformColor;
    const float albedoR = hitMaterial.albedo.r;
    const float albedoG = hitMaterial.albedo.g;
    const float albedoB = hitMaterial.albedo.b;

    Color pixelColor = Color(meshColor.r * albedoR , meshColor.g * albedoG, meshColor.b * albedoB);
    return pixelColor;
}
