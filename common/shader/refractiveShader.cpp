#include <cmath>
#include "color.h"
#include "scene.h"
#include "intersectionData.h"
#include "recursiveShader.h"

const double REFRACTION_BIAS = 1e-2;
const double REFLECTION_BIAS = 1e-2;
const double EPSILON = 1e-6;

Color refractiveShader(const Ray& ray, const IntersectionData& intersectData, Scene& scene, int max_depth)
{
    if (ray.pathDepth > max_depth)
    {
        return scene.bgColor;
    }

    vec3 normal = (intersectData.material->smoothShading) ? intersectData.interpolatedVertNormal : intersectData.hitPointNormal;

    // n1 is assumed to be air/vacuum and n2 is the object
    float n1 = 1.f;
    float n2 = intersectData.material->ior;

    float dotIN = ray.d.dot(normal);

    // incident rays leaves the object (normal and ray are in the same direction)
    if (dotIN > EPSILON)
    {
        normal = -1*normal;
        std::swap(n1, n2);
    }

    // alpha - angle of incidence, beta - angle of refraction

    float cosAlpha = -1 * ray.d.dot(normal);
    float sinAlpha = sqrt(1 - pow(cosAlpha, 2));

    // angle of incidence < critical angle
    if (sinAlpha < n2/n1)
    {
        float sinBeta = sinAlpha * n1 / n2;
        float cosBeta = sqrt(1 - pow(sinBeta, 2));
        vec3 C = (ray.d + (cosAlpha * normal)).normalized();
        vec3 B = C * sinBeta;
        vec3 A = cosBeta * -1 * normal;
        vec3 R = A + B;

        // albedo for refraction?

        Ray refractionRay(intersectData.hitPoint + (-1* normal * REFRACTION_BIAS), R, RayType::refractive, ray.pathDepth + 1);
        Color refractionColor = recursiveShader(refractionRay, scene, max_depth);

        Ray reflectionRay = ray.reflectedRay(normal, intersectData.hitPoint + (normal * REFLECTION_BIAS));
        Color reflectionColor = recursiveShader(reflectionRay, scene, max_depth);

        float fresnel = 0.5 * pow(1.0 + dotIN, 5);

        return (fresnel * reflectionColor) + ((1.0 - fresnel) * refractionColor);
    }

    Ray reflectionRay = ray.reflectedRay(normal, intersectData.hitPoint + (normal * REFLECTION_BIAS)); // depth will be incremented

    return recursiveShader(reflectionRay, scene, max_depth);

}
