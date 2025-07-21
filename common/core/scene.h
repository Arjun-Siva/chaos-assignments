#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "mesh.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include "ray.h"
#include "intersectionData.h"
#include "texture.h"

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>

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
    std::unordered_map<std::string, std::shared_ptr<Texture>> textureMap;
    int bucketSize;


    Scene();
    Scene(const std::string& sceneFileName);
    void addMesh(Mesh& mesh);
    std::vector<Mesh> getMeshes();
    void parseSceneFile(const std::string& sceneFileName);
    void addLight(Light& light);
    void addMaterial(Material& material);
    void addTexture(std::string& name, std::shared_ptr<Texture> texture);
    IntersectionData traceRay(const Ray& ray);

};

#endif // SCENE_H
