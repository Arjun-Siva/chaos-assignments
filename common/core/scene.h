#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "mesh.h"
#include "color.h"
#include <vector>

class Scene
{
private:
    std::vector<Mesh> geometryObjects;

public:
    Camera camera;
    int height;
    int width;
    Color bgColor;
    Scene();
    Scene(const std::string& sceneFileName);
    void addMesh(Mesh& mesh);
    std::vector<Mesh> getMeshes();
    void parseSceneFile(const std::string& sceneFileName);
};

#endif // SCENE_H
