#ifndef RECURSIVESHADER_H
#define RECURSIVESHADER_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include <cassert>
#include "camera.h"
#include "ray.h"
#include "color.h"
#include "triangle.h"
#include "mesh.h"
#include "scene.h"

const double EPSILON = 1e-6;

Color recursiveShader(const Ray &ray, Scene& scene, int max_depth)
{
    if (ray.pathDepth > max_depth)
    {
        return Color(0.f, 0.f, 0.f);
    }

    Color pixelColor = scene.bgColor;

    IntersectionData iData = scene.traceRay(ray);

    // no hit
    if(iData.triangleIdx == -1)
    {
        return pixelColor;
    }

    assert(iData.material != nullptr);
    const Material& hitMaterial = *iData.material;
    const float albedoR = hitMaterial.albedo.r;
    const float albedoG = hitMaterial.albedo.g;
    const float albedoB = hitMaterial.albedo.b;

    if (hitMaterial.type == MaterialType::Diffuse)
    {
        // Shadow ray
        vec3 shadowOrigin = iData.hitPoint + iData.hitPointNormal*1e-3f;
        pixelColor = Color(0, 0, 0);

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
                vec3 lightDir = light.getPosition() - iData.hitPoint;
                float sphereRadius = lightDir.length();

                float cosLaw = 0;
                lightDir = lightDir.normalized();
                if (hitMaterial.smoothShading)
                    cosLaw = std::max(0.0f, lightDir.dot(iData.interpolatedVertNormal));
                else
                    cosLaw = std::max(0.0f, lightDir.dot(iData.hitPointNormal));

                float sphereArea = 4.0f * M_PI * sphereRadius * sphereRadius;

                float rContrib = scene.geometryObjects[iData.objectIdx].uniformColor.r * albedoR;
                float gContrib = scene.geometryObjects[iData.objectIdx].uniformColor.g * albedoG;
                float bContrib = scene.geometryObjects[iData.objectIdx].uniformColor.b * albedoB;

                pixelColor = pixelColor + (Color(rContrib, gContrib, bContrib) * (cosLaw/sphereArea)) * light.getIntensity();
            }

        } // lights loop end
        return pixelColor;
    } // end if diffuse

    else if (hitMaterial.type == MaterialType::Reflective)
    {
        vec3& normal = (hitMaterial.smoothShading) ? iData.interpolatedVertNormal : iData.hitPointNormal;
        Ray reflectedR = ray.reflectedRay(normal, iData.hitPoint);

        const Color reflectedColor = recursiveShader(reflectedR, scene, max_depth);

        return Color(reflectedColor.r * albedoR,  reflectedColor.g * albedoG, reflectedColor.b * albedoB);
    }

    return Color(0,0,0);
}

#endif // RECURSIVESHADER_H
