#include <iostream>
#include <string>
#include <fstream>
#include "mesh.h"
#include "scene.h"
#include "camera.h"
#include "triangle.h"


double EPSILON = 1e-6;

void shadeBarycentric(const std::string &outputFile, Scene& scene)
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
            int hitTriangleIndex;
            Triangle hitTriangle(vec3(0,0,0), vec3(0,0,0), vec3(0,0,0));
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
                    hitTriangle = mesh.getTriangleByIndex(meshHitTriIndex);
                    hitPoint = meshHitPoint;

                    hitNormal = meshHitNormal;
                    hitColor = meshColor;
                    hitMaterial = mesh.material;
                    missedAllMeshes = false;
                    hitTriangleIndex = meshHitTriIndex;
                }
            } // mesh-loop end

            if (!missedAllMeshes)
            {
                vec3 baryCoords = hitTriangle.getBaryCentricCoords(hitPoint);
                pixelColor = Color(baryCoords.x, baryCoords.y, baryCoords.z);

            } // missedAllMeshes if end

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

    for(Mesh& mesh : scene.geometryObjects)
    {
        Material mat = Material();
        mesh.setMaterial(mat);
    }

    shadeBarycentric(sceneFileName.substr(0, sceneFileName.find_last_of('.'))+"_barycentric.ppm", scene);

    std::cout<<"Render completed"<<std::endl;

    return 0;
}
