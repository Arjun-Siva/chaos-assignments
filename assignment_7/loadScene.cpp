#include <iostream>
#include <string>
#include <vector>
#include "mesh.h"
#include "triangle.h"
#include "scene.h"
#include "renderPPM.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <scene_file.crtScene>\n";
        return 1;
    }

    std::string sceneFileName = argv[1];

    Scene scene(sceneFileName);

    std::vector<Mesh> meshList = scene.getMeshes();

    std::vector<Triangle> allTriangles;

    for(Mesh& mesh : meshList)
    {
        mesh.setRandomColors();
        const auto& triangles = mesh.generateTriangleList();
        allTriangles.insert(allTriangles.end(), triangles.begin(), triangles.end());
    }

    renderScene(sceneFileName.substr(0, sceneFileName.find_last_of('.'))+"_scene.ppm", scene.width, scene.height, scene.camera, allTriangles, scene.bgColor);

    std::cout<<"Render completed"<<std::endl;

    return 0;
}
