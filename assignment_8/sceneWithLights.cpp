#include <iostream>
#include <string>
#include "shadePixels.h"
#include "mesh.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <scene_file.crtScene>\n";
        return 1;
    }

    std::string sceneFileName = argv[1];

    Scene scene(sceneFileName);

    for(Mesh& mesh : scene.geometryObjects)
    {
        mesh.setUniformColor(Color(1.0, 0.0, 1.0));
        Material mat = Material();
        mat.albedo = Color(0.5, 0.5, 0.5);
        mesh.setMaterial(mat);
    }

    scene.height = 720;
    scene.width = 1280;

    shadePixels(sceneFileName.substr(0, sceneFileName.find_last_of('.'))+"_scene.ppm", scene);

    std::cout<<"Render completed"<<std::endl;

    return 0;
}
