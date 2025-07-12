#ifndef SHADEPIXELS_H
#define SHADEPIXELS_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <cmath>
#include "camera.h"
#include "ray.h"
#include "color.h"
#include "triangle.h"
#include "mesh.h"
#include "scene.h"

double EPSILON = 1e-6;

void shadePixels(const std::string &outputFile, Scene& scene)
{
    int width = scene.width;
    int height = scene.height;
    Camera camera = scene.camera;
    Color bg = scene.bgColor;

    std::ofstream out(outputFile);
    out << "P3\n" << width << ' ' << height << "\n255\n";

    // for each pixel
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float u = (x + 0.5f) / width;
            float v = (y + 0.5f) / height;

            Ray ray = camera.generateRay(u, v);
            Color pixelColor = bg;

            double shortestIntersection = 1/EPSILON;

            // find shortest intersecting triangle
            // find the point of intersection
            // get the normal of triangle

            vec3 hitPoint;
            vec3 hitNormal;
            Color hitColor;
            Material hitMaterial;
            bool missedAllMeshes = true;

            for (const Mesh& mesh : scene.geometryObjects)
            {
                Color meshColor = mesh.uniformColor;
                int meshHitTriIndex;
                vec3 meshHitPoint;
                vec3 meshHitNormal;

                double t = mesh.intersectRay(ray, meshHitTriIndex, meshHitPoint, meshHitNormal, true);

                // shorter hit than previous and not miss (not -1)
                if (t < shortestIntersection && t > EPSILON)
                {
                    shortestIntersection = t;
                    hitPoint = meshHitPoint;
                    hitNormal = meshHitNormal;
                    hitColor = meshColor;
                    hitMaterial = mesh.material;
                    missedAllMeshes = false;
                }
            } // mesh-loop end

            if (!missedAllMeshes)
            {
                // Shadow ray
                vec3 shadowOrigin = hitPoint + hitNormal*1e-4f;
                pixelColor = Color(0, 0, 0);

                for (const Light& light : scene.lights)
                {
                    vec3 shadowDir = (light.getPosition() - shadowOrigin).normalized();

                    Ray shadowRay = Ray(shadowOrigin, shadowDir);

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
                        vec3 lightDir = light.getPosition() - hitPoint;
                        float sphereRadius = lightDir.length();

                        lightDir = lightDir.normalized();
                        float cosLaw = std::max(0.0f, lightDir.dot(hitNormal));

                        float albedoR = hitMaterial.albedo.r;
                        float albedoG = hitMaterial.albedo.g;
                        float albedoB = hitMaterial.albedo.b;

                        float sphereArea = 4.0f * M_PI * sphereRadius * sphereRadius;

                        float rContrib = hitColor.r * albedoR;
                        float gContrib = hitColor.g * albedoG;
                        float bContrib = hitColor.b * albedoB;

                        pixelColor = pixelColor + (Color(rContrib, gContrib, bContrib) * (cosLaw/sphereArea)) * light.getIntensity();
                    }

                } // lights loop end
            } // missedAllLights if end

            out<<static_cast<int>(pixelColor.r * 255)<<" "<<static_cast<int>(pixelColor.g * 255)<<" "<<static_cast<int>(pixelColor.b * 255)<<"\n";
        } // x loop end
    } // y loop end

    out.close();
}

#endif // SHADEPIXELS_H
