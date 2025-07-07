#include "scene.h"
#include "color.h"
#include "mesh.h"
#include "vec3.h"
#include "camera.h"
#include "triangle.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <cassert>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

using namespace rapidjson;

Scene::Scene() : camera(1920.0/1080.0)
{
    this->bgColor = Color(0, 0, 0);
    this->height = 1080;
    this->width = 1920;
}

Scene::Scene(const std::string& sceneFileName) : camera(1920.0/1080.0)
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
            this->bgColor = Color(static_cast<int>(bgColorVal[0].GetDouble() * 255),
                    static_cast<int>(bgColorVal[1].GetDouble() * 255),
                    static_cast<int>(bgColorVal[2].GetDouble() * 255)
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

        const Value& matrixVal = cameraVal.FindMember("image_settings")->value;
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
            this->camera.transformBasis(rotationMatrix);
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

                for (unsigned int i = 0; i + 2 < triangleArray.Size(); i += 3)
                {
                    mesh.insertTriangleIndex(static_cast<int>(triangleArray[i].GetDouble()),
                                      static_cast<int>(triangleArray[i + 1].GetDouble()),
                                      static_cast<int>(triangleArray[i + 2].GetDouble())
                                    );
                }
            }

            this->addMesh(mesh);
        }
    }

}
