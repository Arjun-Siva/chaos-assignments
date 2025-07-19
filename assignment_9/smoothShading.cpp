#include <iostream>
#include <string>
#include <fstream>
#include "mesh.h"
#include "scene.h"
#include "camera.h"
#include "triangle.h"
#include "intersectionData.h"
#include "recursiveShader.h"


double EPSILON = 1e-6;

void shadeSmooth(const std::string &outputFile, Scene& scene)
{
    int width = scene.width;
    int height = scene.height;
    Camera camera = scene.camera;

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
//            Color pixelColor = bg;

//            IntersectionData iData = scene.traceRay(ray);

//            if (iData.triangleIdx != -1)
//            {
//                // Shadow ray
//                vec3 shadowOrigin = iData.hitPoint + iData.hitPointNormal*1e-3f;
//                pixelColor = Color(0, 0, 0);

//                for (const Light& light : scene.lights)
//                {
//                    vec3 shadowDir = (light.getPosition() - shadowOrigin).normalized();

//                    Ray shadowRay = Ray(shadowOrigin, shadowDir);

//                    vec3 shadowHitPoint;
//                    vec3 shadowDHitNormal;
//                    int shadownHitTriangleIndex;
//                    double distanceToLight = (light.getPosition() - shadowOrigin).length();
//                    bool shadowReachLight = true;

//                    for (const Mesh& mesh : scene.geometryObjects)
//                    {
//                        double st = mesh.intersectRay(shadowRay, shadownHitTriangleIndex, shadowHitPoint, shadowDHitNormal, false);
//                        // intersection before reaching light
//                        if (st < distanceToLight && st > EPSILON)
//                        {
//                            shadowReachLight = false;
//                            break;
//                        }
//                    }

//                    // add contribution of each light
//                    if (shadowReachLight)
//                    {
//                        vec3 lightDir = light.getPosition() - iData.hitPoint;
//                        float sphereRadius = lightDir.length();

//                        float cosLaw = 0;
//                        lightDir = lightDir.normalized();
//                        if (iData.material->smoothShading)
//                            cosLaw = std::max(0.0f, lightDir.dot(iData.interpolatedVertNormal));
//                        else
//                            cosLaw = std::max(0.0f, lightDir.dot(iData.hitPointNormal));

//                        float albedoR = iData.material->albedo.r;
//                        float albedoG = iData.material->albedo.g;
//                        float albedoB = iData.material->albedo.b;

//                        float sphereArea = 4.0f * M_PI * sphereRadius * sphereRadius;

//                        float rContrib = scene.geometryObjects[iData.objectIdx].uniformColor.r * albedoR;
//                        float gContrib = scene.geometryObjects[iData.objectIdx].uniformColor.g * albedoG;
//                        float bContrib = scene.geometryObjects[iData.objectIdx].uniformColor.b * albedoB;

//                        pixelColor = pixelColor + (Color(rContrib, gContrib, bContrib) * (cosLaw/sphereArea)) * light.getIntensity();
//                    }

//                } // lights loop end
//            } // missedAllLights if end

            Color pixelColor = recursiveShader(ray, scene, 5);

            out<<static_cast<int>(pixelColor.r * 255)<<" "<<static_cast<int>(pixelColor.g * 255)<<" "<<static_cast<int>(pixelColor.b * 255)<<"\n";
        } // x loop end
    } // y loop end

    out.close();
}


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <scene_file.crtScene>\n";
        return 1;
    }

    std::string sceneFileName = argv[1];

    Scene scene(sceneFileName);

//    scene.height = 480;
//    scene.width = 852;

    for(Mesh& mesh : scene.geometryObjects)
    {
        mesh.setUniformColor(Color(1, 1, 1));
    }

    shadeSmooth(sceneFileName.substr(0, sceneFileName.find_last_of('.'))+"_smooth.ppm", scene);

    std::cout<<"Render completed"<<std::endl;

    return 0;
}
