#ifndef MESH_H
#define MESH_H

#include "vec3.h"
#include "triangle.h"
#include "color.h"
#include <vector>

class Mesh
{
public:
    Mesh();
    void insertVertex(float v0, float v1, float v2);
    void insertTriangleIndex(int i0, int i1, int i2);
    std::vector<Triangle> generateTriangleList() const;
    void setUniformColor(Color &color);
    void setRandomColors();
private:
    Color uniformColor;
    bool randomizeColors;
    std::vector<vec3> vertices;
    std::vector<int> triangleVertIndices;

};

#endif // MESH_H
