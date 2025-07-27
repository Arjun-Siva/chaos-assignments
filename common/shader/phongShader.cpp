#include "color.h"
#include "scene.h"
#include "intersectionData.h"

const double SHADOW_BIAS = 1e-3;

Color phongShader(IntersectionData& intersectData, Scene& scene)
{
    const Material hitMaterial = *intersectData.material;

    Color albedo = scene.geometryObjects[intersectData.objectIdx].getAlbedo(intersectData.baryCentricCoords, intersectData.triangleIdx);
    const float albedoR = albedo.r;
    const float albedoG = albedo.g;
    const float albedoB = albedo.b;
    // Shadow ray
    vec3 shadowOrigin = intersectData.hitPoint + intersectData.hitPointNormal * SHADOW_BIAS;
    Color pixelColor = Color(0, 0, 0);
    Color diffuseColor = Color(0, 0, 0);
    Color specularColor = Color(0, 0, 0);

    for (const Light& light : scene.lights)
    {
        vec3 shadowDir = (light.getPosition() - shadowOrigin).normalized();

        Ray shadowRay = Ray(shadowOrigin, shadowDir, RayType::shadow, 1);

        vec3 shadowHitPoint;
        vec3 shadowDHitNormal;
        int shadownHitTriangleIndex;
        double distanceToLight = (light.getPosition() - shadowOrigin).length();
        bool shadowReachLight = true;

        if (scene.useBVH)
        {
            IntersectionData t = scene.traceRayBVH(shadowRay);

            if (t.objectIdx != -1)
            {
                shadowHitPoint = t.hitPoint;
                double st = (shadowHitPoint - shadowOrigin).length();
                if (st < distanceToLight && st > EPSILON)
                    shadowReachLight = false;
            }
        }

        else
        {
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
        }

        // add contribution of each light
        if (shadowReachLight)
        {
            // diffuse
            vec3 lightDir = light.getPosition() - intersectData.hitPoint;
            float sphereRadius = lightDir.length();

            vec3 normal = hitMaterial.smoothShading ? intersectData.interpolatedVertNormal : intersectData.hitPointNormal;

            vec3 L = lightDir.normalized();
            float cosLaw = std::max(0.0f, L.dot(normal));

            float attenuation = 1.0f / 4.0f * M_PI * sphereRadius * sphereRadius;

            diffuseColor = diffuseColor + (albedo * (cosLaw * attenuation)) * light.getIntensity();

            //specular in blinn-phong
            vec3 V = (scene.camera.getPosition() - intersectData.hitPoint).normalized();
            vec3 H = (L + V).normalized();
            float NdotH = std::max(normal.dot(H), 0.0f);

            specularColor = specularColor + Color(1, 1, 1) * (std::pow(NdotH, hitMaterial.shininess) * attenuation) * light.getIntensity() * 0.01f;

        }

    } // lights loop end

    // ambient
    Color ambientColor = albedo * hitMaterial.ambientLightIntensity;

    pixelColor = hitMaterial.ka * ambientColor + hitMaterial.kd * diffuseColor + hitMaterial.ks * specularColor;

    return pixelColor;
}
