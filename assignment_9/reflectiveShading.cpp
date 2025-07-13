#include <iostream>
#include <string>
#include <fstream>
#include "mesh.h"
#include "scene.h"
#include "camera.h"
#include "triangle.h"
#include "intersectionData.h"
#include "recursiveShader.h"

void shadeReflection(const std::string &outputFile, Scene& scene)
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
            Color pixelColor = recursiveShader(ray, scene, 3);

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
        mesh.setUniformColor(Color(1, 1, 1));
    }

    shadeReflection(sceneFileName.substr(0, sceneFileName.find_last_of('.'))+"_reflection.ppm", scene);

    std::cout<<"Render completed"<<std::endl;

    return 0;
}
