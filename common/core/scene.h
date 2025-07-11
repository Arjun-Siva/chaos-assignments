#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "mesh.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include <vector>

class Scene
{
private:

public:
    Camera camera;
    int height;
    int width;
    Color bgColor;
    std::vector<Mesh> geometryObjects;
    std::vector<Light> lights;
    std::vector<Material> meshMaterials;

    Scene();
    Scene(const std::string& sceneFileName);
    void addMesh(Mesh& mesh);
    std::vector<Mesh> getMeshes();
    void parseSceneFile(const std::string& sceneFileName);
    void addLight(Light& light);
    void addMaterial(Material& material);
};

#endif // SCENE_H
