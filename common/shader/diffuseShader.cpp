#include "color.h"
#include "scene.h"
#include "intersectionData.h"

const double EPSILON = 1e-6;
const double SHADOW_BIAS = 1e-3;

Color diffuseShader(const IntersectionData& intersectData, Scene& scene)
{
    const Material hitMaterial = *intersectData.material;
    const float albedoR = hitMaterial.albedo.r;
    const float albedoG = hitMaterial.albedo.g;
    const float albedoB = hitMaterial.albedo.b;
    // Shadow ray
    vec3 shadowOrigin = intersectData.hitPoint + intersectData.hitPointNormal * SHADOW_BIAS;
    Color pixelColor = Color(0, 0, 0);

    for (const Light& light : scene.lights)
    {
        vec3 shadowDir = (light.getPosition() - shadowOrigin).normalized();

        Ray shadowRay = Ray(shadowOrigin, shadowDir, RayType::shadow, 1);

        vec3 shadowHitPoint;
        vec3 shadowDHitNormal;
        int shadownHitTriangleIndex;
        double distanceToLight = (light.getPosition() - shadowOrigin).length();
        bool shadowReachLight = true;

        for (const Mesh& mesh : scene.geometryObjects)
        {
            double st = mesh.intersectRay(shadowRay, shadownHitTriangleIndex, shadowHitPoint, shadowDHitNormal, false);

            // intersection before reaching light
            if (st < distanceToLight && st > EPSILON)
            {
                shadowReachLight = false;
                break;
            }
        }

        // add contribution of each light
        if (shadowReachLight)
        {
            vec3 lightDir = light.getPosition() - intersectData.hitPoint;
            float sphereRadius = lightDir.length();

            float cosLaw = 0;
            lightDir = lightDir.normalized();
            if (hitMaterial.smoothShading)
                cosLaw = std::max(0.0f, lightDir.dot(intersectData.interpolatedVertNormal));
            else
                cosLaw = std::max(0.0f, lightDir.dot(intersectData.hitPointNormal));

            float sphereArea = 4.0f * M_PI * sphereRadius * sphereRadius;

            float rContrib = scene.geometryObjects[intersectData.objectIdx].uniformColor.r * albedoR;
            float gContrib = scene.geometryObjects[intersectData.objectIdx].uniformColor.g * albedoG;
            float bContrib = scene.geometryObjects[intersectData.objectIdx].uniformColor.b * albedoB;

            pixelColor = pixelColor + (Color(rContrib, gContrib, bContrib) * (cosLaw/sphereArea)) * light.getIntensity();
        }

    } // lights loop end
    return pixelColor;
}
