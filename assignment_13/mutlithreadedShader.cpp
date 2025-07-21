#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>

#include "mesh.h"
#include "scene.h"
#include "camera.h"
#include "triangle.h"
#include "clamp.h"
#include "intersectionData.h"
#include "recursiveShader.h"
#include "pixelBuffer.h"

void renderRegion(Scene& scene, PixelBuffer& buffer, int startX, int startY, int region_width, int region_height)
{
    Camera& camera = scene.camera;
    int width = scene.width;
    int height = scene.height;

    for (int y = startY; y < startY + region_height; ++y) {
        for (int x = startX; x < startX + region_width; ++x) {
            float u = (x + 0.5f) / width;
            float v = (y + 0.5f) / height;

            Ray ray = camera.generateRay(u, v);

            Color pixelColor = recursiveShader(ray, scene, 5);
            buffer.setColor(x, y, pixelColor);
        }
    }
}

void parallelRender(Scene& scene, PixelBuffer& buffer, int imageWidth, int imageHeight, int numThreadsX, int numThreadsY) {
    std::vector<std::thread> threads;
    int blockWidth  = imageWidth / numThreadsX;
    int blockHeight = imageHeight / numThreadsY;

    for (int by = 0; by < numThreadsY; ++by) {
        for (int bx = 0; bx < numThreadsX; ++bx) {
            int startX = bx * blockWidth;
            int startY = by * blockHeight;
            int endX = (bx == numThreadsX - 1) ? imageWidth  : startX + blockWidth;
            int endY = (by == numThreadsY - 1) ? imageHeight : startY + blockHeight;

            threads.emplace_back(renderRegion, std::ref(scene), std::ref(buffer),
                                 startX, startY, endX - startX, endY - startY);
        }
    }

    for (auto& t : threads) {
        t.join();
    }
}

void shadeTexture(const std::string &outputFile, Scene& scene)
{
    int width = scene.width;
    int height = scene.height;

    PixelBuffer buffer(width, height);

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    parallelRender(scene, buffer, width, height, 4, 3);

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
