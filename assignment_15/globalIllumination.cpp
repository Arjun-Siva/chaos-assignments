#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <mutex>
#include <queue>
#include <cstdlib>

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
float numberOfBuckets;

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
    numberOfBuckets = renderQueue.size();
}


void renderRegion(Scene& scene, PixelBuffer& buffer, int startX, int startY, int region_width, int region_height)
{
    Camera& camera = scene.camera;
    int width = scene.width;
    int height = scene.height;

    for (int y = startY; y < startY + region_height; ++y) {
        for (int x = startX; x < startX + region_width; ++x) {
            Color pixelColor = Color(0, 0, 0);
            for (int n = 0; n < 128; ++n) { //AA
                float x_offset = static_cast<float>(rand()) / RAND_MAX;
                float y_offset = static_cast<float>(rand()) / RAND_MAX;
                float u = (x + x_offset) / width;
                float v = (y + y_offset) / height;
                Ray ray = camera.generateRay(u, v);

                pixelColor = pixelColor + recursiveShader(ray, scene, 5);
            }
            buffer.setColor(x, y, pixelColor * (1.0f / 128.f));
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
        std::cout<< (1.0f - static_cast<float>(renderQueue.size()) / numberOfBuckets) * 100 <<"% completed"<<std::endl;
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

void buildBVHTree(Scene& scene)
{
    std::vector<Triangle> ts = scene.getAllTrianglesInScene();
    scene.min_triangles_per_bvhnode = 4;
    scene.max_bvhtree_depth = 24;
    scene.useBVH = true;
    std::unique_ptr<BVHNode> root = scene.buildBVHTree(ts, 0);
    scene.bvhRoot = std::move(root);
}


void shadeTexture(const std::string &outputFile, Scene& scene)
{
    int width = scene.width;
    int height = scene.height;
    PixelBuffer buffer(width, height);

    buildBVHTree(scene);

    int numThreads = std::thread::hardware_concurrency();

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
    std::string sceneFileName = "scene2.crtscene";

    Scene scene(sceneFileName);
    scene.gi_ray_count = 1;
    scene.bucketSize = 24;

    scene.height = 720;
    scene.width = 720;


    assert(scene.bucketSize > 0);

    for(Mesh& mesh : scene.geometryObjects)
    {
        mesh.setUniformColor(Color(1, 1, 1));
    }

   shadeTexture(sceneFileName.substr(0, sceneFileName.find_last_of('.'))+"_gi.ppm", scene);

    std::cout<<"Render completed"<<std::endl;
    return 0;
}
