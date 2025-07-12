#ifndef MESH_H
#define MESH_H

#include "vec3.h"
#include "triangle.h"
#include "color.h"
#include "material.h"
#include <vector>

class Mesh
{
public:
    Mesh();
    // TODO: reserve space before insertion
    void insertVertex(float v0, float v1, float v2);
    void insertTriangleIndex(int i0, int i1, int i2);
    Triangle getTriangleByIndex(const int index) const;
    std::vector<Triangle> generateTriangleList() const;
    void setUniformColor(const Color &color);
    void setRandomColors();
    void setMaterial(const Material& material);
    void computeTriangleNormals();
    void computeVertexNormals();
    double intersectRay(const Ray& r, int& hitTriangleIndex, vec3& hitPoint, vec3& hitNormal, bool cullBackFaces) const;

    Color uniformColor;
    bool randomizeColors;
    std::vector<vec3> vertices;
    std::vector<int> triangleVertIndices;
    std::vector<vec3> triangleNormals;
    std::vector<vec3> vertexNormals;
    Material material;

};

#endif // MESH_H
