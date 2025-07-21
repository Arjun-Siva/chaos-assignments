#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>

#include "mesh.h"
#include "scene.h"
#include "camera.h"
#include "triangle.h"
#include "clamp.h"
#include "intersectionData.h"
#include "recursiveShader.h"
#include "pixelBuffer.h"


struct Bucket
{
    int x, y, width, height;
};

// Shared queue and mutex
std::queue<Bucket> renderQueue;
std::mutex queueMutex;

void createBuckets(int imageWidth, int imageHeight, int bucketSize)
{

    for (int y = 0; y < imageHeight; y += bucketSize)
    {
        for (int x = 0; x < imageWidth; x += bucketSize)
        {
            int w = std::min(bucketSize, imageWidth - x);
            int h = std::min(bucketSize, imageHeight - y);
           renderQueue.push({x, y, w, h});
        }
    }
}


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

// Thread function
void workerThread(Scene& scene, PixelBuffer& buffer)
{
    while (true)
    {
        Bucket region;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (renderQueue.empty())
                break;

            region = renderQueue.front();
            renderQueue.pop();

        }

        renderRegion(scene, buffer, region.x, region.y, region.width, region.height);

    }
}


void bucketRender(Scene& scene, PixelBuffer& buffer, int numThreads)
{
   createBuckets(scene.width, scene.height, scene.bucketSize);

   std::vector<std::thread> threads;
   for (int i = 0; i < numThreads; ++i)
   {
       threads.emplace_back(workerThread, std::ref(scene), std::ref(buffer));
   }

   for (auto& t : threads)
   {
       t.join();
   }
}

void shadeTexture(const std::string &outputFile, Scene& scene)
{
    int width = scene.width;
    int height = scene.height;
    PixelBuffer buffer(width, height);
    int numThreads = std::thread::hardware_concurrency();;

    // Start timer
    auto start = std::chrono::high_resolution_clock::now();

    bucketRender(scene, buffer, numThreads);

//    // End timer
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

    assert(scene.bucketSize > 0);

    for(Mesh& mesh : scene.geometryObjects)
    {
        mesh.setUniformColor(Color(1, 1, 1));
    }

   shadeTexture(sceneFileName.substr(0, sceneFileName.find_last_of('.'))+"_profile.ppm", scene);

    std::cout<<"Render completed"<<std::endl;
    return 0;
}
