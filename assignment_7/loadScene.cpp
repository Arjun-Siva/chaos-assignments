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

//    std::cout<<allTriangles[0].v0<<std::endl;
//    std::cout<<allTriangles[0].v1<<std::endl;
//    std::cout<<allTriangles[0].v2<<std::endl;
//    std::cout<<allTriangles[0].normal<<std::endl;
//    std::cout<<"tg "<<allTriangles[0].color.r<<","<<allTriangles[0].color.g<<","<<allTriangles[0].color.b<<std::endl;
////    std::cout<<allTriangles[0].color.r<<std::endl;

//    std::cout<<"eye:"<<scene.camera.getPosition()<<"\n";
//    mat3 orientation = scene.camera.getOrientation();
//    std::cout<<"orientation:"<<orientation.rows[0]<<";"<<orientation.rows[1]<<";"<<orientation.rows[2]<<"\n";

//    std::cout<<"w "<<scene.width<<std::endl;
//    std::cout<<"h "<<scene.height<<std::endl;
//    std::cout<<"bg "<<scene.bgColor.r<<","<<scene.bgColor.g<<","<<scene.bgColor.b<<std::endl;

    renderScene(sceneFileName.substr(0, sceneFileName.find_last_of('.'))+"_scene.ppm", scene.width, scene.height, scene.camera, allTriangles, scene.bgColor);

    std::cout<<"Render completed"<<std::endl;

    return 0;
}
