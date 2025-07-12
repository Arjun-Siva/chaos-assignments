#include "scene.h"
#include "color.h"
#include "mesh.h"
#include "vec3.h"
#include "camera.h"
#include "triangle.h"
#include "light.h"
#include "material.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

using namespace rapidjson;

Scene::Scene() : camera(1920.0f/1080.0f)
{
    this->bgColor = Color(0, 0, 0);
    this->height = 1080;
    this->width = 1920;
}

Scene::Scene(const std::string& sceneFileName) : camera(1920.0f/1080.0f)
{
    parseSceneFile(sceneFileName);
}

void Scene::addMesh(Mesh& mesh)
{
    geometryObjects.push_back(mesh);
}

std::vector<Mesh> Scene::getMeshes()
{
    return geometryObjects;
}

void Scene::addMaterial(Material &material)
{
    meshMaterials.push_back(material);
}

void Scene::parseSceneFile(const std::string &sceneFileName)
{
    // default values for camera
    this->bgColor = Color(0, 0, 0);
    this->height = 1080;
    this->width = 1920;

    vec3 eye(0, 0, 0);
    vec3 right(1, 0, 0);
    vec3 up(0, 1, 0);
    vec3 forward(0, 0, -1);

    this->camera = Camera(eye, right, up, forward, (float)width/(float)height, 1);

    // Read file
    std::ifstream ifs(sceneFileName);
    assert(ifs.is_open());

    IStreamWrapper isw(ifs);
    Document doc;
    doc.ParseStream(isw);

    // Settings
    const Value& settingsVal = doc.FindMember("settings")->value;

    if (!settingsVal.IsNull() && settingsVal.IsObject())
    {
        const Value& bgColorVal = settingsVal.FindMember("background_color")->value;
        if(!bgColorVal.IsNull() && bgColorVal.IsArray())
        {
            assert(bgColorVal.Size() == 3);
            // color values are in the range 0.0 to 1.0
            this->bgColor = Color(static_cast<float>(bgColorVal[0].GetDouble()),
                    static_cast<float>(bgColorVal[1].GetDouble()),
                    static_cast<float>(bgColorVal[2].GetDouble())
                    );
        }


        const Value& imageDim = settingsVal.FindMember("image_settings")->value;
        if(!imageDim.IsNull() && imageDim.IsObject())
        {
            const Value& hv = imageDim.FindMember("height")->value;
            if (!hv.IsNull()) this->height = static_cast<int>(hv.GetDouble());
            const Value& wv = imageDim.FindMember("width")->value;
            if (!wv.IsNull()) this->width = static_cast<int>(wv.GetDouble());
        }
    }

    // camera
    const Value& cameraVal = doc.FindMember("camera")->value;

    if (!cameraVal.IsNull() && cameraVal.IsObject())
    {
        const Value& positionVal = cameraVal.FindMember("position")->value;
        if(!positionVal.IsNull() && positionVal.IsArray())
        {
            assert(positionVal.Size() == 3);
            eye = vec3(static_cast<float>(positionVal[0].GetDouble()),
                    static_cast<float>(positionVal[1].GetDouble()),
                    static_cast<float>(positionVal[2].GetDouble())
                    );

            this->camera = Camera(eye, right, up, forward, (float)this->width/(float)this->height, 1);
        }

        const Value& matrixVal = cameraVal.FindMember("matrix")->value;
        if(!matrixVal.IsNull() && matrixVal.IsArray())
        {
            vec3 r0 = vec3(static_cast<float>(matrixVal[0].GetDouble()),
                    static_cast<float>(matrixVal[1].GetDouble()),
                    static_cast<float>(matrixVal[2].GetDouble())
                    );

            vec3 r1 = vec3(static_cast<float>(matrixVal[3].GetDouble()),
                    static_cast<float>(matrixVal[4].GetDouble()),
                    static_cast<float>(matrixVal[5].GetDouble())
                    );

            vec3 r2 = vec3(static_cast<float>(matrixVal[6].GetDouble()),
                    static_cast<float>(matrixVal[7].GetDouble()),
                    static_cast<float>(matrixVal[8].GetDouble())
                    );

            mat3 rotationMatrix = mat3(r0, r1, r2);
            this->camera.setOrientation(rotationMatrix);
        }

    }

    // Materials
    // Materials must be loaded before creating meshes
    if (doc.HasMember("materials") && doc["materials"].IsArray())
    {
        const auto& materials = doc.FindMember("materials")->value;
        for (const auto& material: materials.GetArray())
        {
            Material currentMaterial;

            if (material.HasMember("type"))
            {
                std::string matType = material["type"].GetString();

                if (matType == "diffuse")
                {
                    currentMaterial.type = MaterialType::Diffuse;
                }
                else if (matType == "reflective")
                {
                    currentMaterial.type = MaterialType::Reflective;
                }
                else if (matType == "refractive")
                {
                    currentMaterial.type = MaterialType::Refractive;
                }
                else if (matType == "phong")
                {
                    currentMaterial.type = MaterialType::Phong;
                }

            }

            if (material.HasMember("albedo") && material["albedo"].IsArray())
            {
                const auto& albedoArr = material["albedo"].GetArray();
                assert(albedoArr.Size() == 3);

                float a0 = static_cast<float>(albedoArr[0].GetDouble());
                float a1 = static_cast<float>(albedoArr[1].GetDouble());
                float a2 = static_cast<float>(albedoArr[2].GetDouble());

                currentMaterial.albedo = Color(a0, a1, a2);
            }

            if (material.HasMember("smooth_shading") && material["smooth_shading"].IsBool())
            {
                currentMaterial.smoothShading = material["smooth_shading"].GetBool();
            }

            this->addMaterial(currentMaterial);

        }
    }



    // Objects
    if (doc.HasMember("objects") && doc["objects"].IsArray())
    {
        const auto& objs = doc.FindMember("objects")->value;
        for (const auto& obj : objs.GetArray())
        {
            Mesh mesh;
            // vertices
            if (obj.HasMember("vertices") && obj["vertices"].IsArray())
            {
                const auto& vertexArray = obj["vertices"].GetArray();
                assert(vertexArray.Size() % 3 == 0);
                mesh.vertices.reserve(vertexArray.Size() / 3);

                for (unsigned int i = 0; i + 2 < vertexArray.Size(); i += 3)
                {
                    mesh.insertVertex(static_cast<float>(vertexArray[i].GetDouble()),
                                      static_cast<float>(vertexArray[i + 1].GetDouble()),
                                      static_cast<float>(vertexArray[i + 2].GetDouble())
                                    );
                }

            }

            if (obj.HasMember("triangles") && obj["triangles"].IsArray())
            {
                const auto& triangleArray = obj["triangles"].GetArray();
                assert(triangleArray.Size() % 3 == 0);

                mesh.triangleVertIndices.reserve(triangleArray.Size());

                for (unsigned int i = 0; i + 2 < triangleArray.Size(); i += 3)
                {
                    mesh.insertTriangleIndex(static_cast<int>(triangleArray[i].GetDouble()),
                                      static_cast<int>(triangleArray[i + 1].GetDouble()),
                                      static_cast<int>(triangleArray[i + 2].GetDouble())
                                    );
                }
            }

            if (obj.HasMember("material_index") && obj["material_index"].IsInt())
            {
                mesh.setMaterial(this->meshMaterials[obj["material_index"].GetInt()]);
            }

            mesh.computeTriangleNormals();
            mesh.computeVertexNormals();

            this->addMesh(mesh);
        }

    }

    // Lights
    if (doc.HasMember("lights") && doc["lights"].IsArray())
    {
        const auto& lights = doc.FindMember("lights")->value;
        std::vector<Light> sceneLights;
        this->lights.reserve(lights.GetArray().Size());

        for (const auto& light : lights.GetArray())
        {
            if(!light.IsNull() && light.IsObject())
            {
                if(light.HasMember("intensity") && light.HasMember("position"))
                {
                    float intensity = light["intensity"].GetFloat();
                    assert(light["position"].IsArray());
                    const auto& positionArr = light["position"].GetArray();
                    assert(positionArr.Size() == 3);
                    vec3 position = vec3(static_cast<float>(positionArr[0].GetDouble()),
                            static_cast<float>(positionArr[1].GetDouble()),
                            static_cast<float>(positionArr[2].GetDouble())
                            );

                    this->lights.push_back(Light(position, intensity));
                }
            }
        }
    }
}
