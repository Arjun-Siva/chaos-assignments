#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "mesh.h"
#include "scene.h"
#include "camera.h"
#include "triangle.h"
#include "clamp.h"
#include "intersectionData.h"
#include "recursiveShader.h"
#include "pixelBuffer.h"


void shadeTexture(const std::string &outputFile, Scene& scene)
{
    int width = scene.width;
    int height = scene.height;
    Camera camera = scene.camera;
    PixelBuffer buffer(width, height);

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();
    // for each pixel
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            float u = (x + 0.5f) / width;
            float v = (y + 0.5f) / height;

            Ray ray = camera.generateRay(u, v);

            Color pixelColor = recursiveShader(ray, scene, 5);
            buffer.setColor(x, y, pixelColor);
        }
    }

    // End timer
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate duration
    std::chrono::duration<double> duration = end - start;
    std::cout << "Time taken: " << duration.count() << " seconds\n";


    buffer.writeToPPM(outputFile);
}


int main()
{

    std::string sceneFileName = "scene0.crtscene";

    Scene scene(sceneFileName);

    for(Mesh& mesh : scene.geometryObjects)
    {
        mesh.setUniformColor(Color(1, 1, 1));
    }

    shadeTexture(sceneFileName.substr(0, sceneFileName.find_last_of('.'))+"_profile.ppm", scene);

    std::cout<<"Render completed"<<std::endl;


    return 0;
}
