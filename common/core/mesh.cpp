#include "mesh.h"
#include "vec3.h"
#include "triangle.h"
#include <cstdlib>
#include <vector>
#include <cassert>

Mesh::Mesh() : uniformColor(Color(255, 255, 255)), randomizeColors(false)
{}

void Mesh::setUniformColor(Color &color)
{
    this->uniformColor = color;
}

void Mesh::setRandomColors()
{
    this->randomizeColors = true;
}

void Mesh::insertVertex(float v0, float v1, float v2)
{
    vertices.push_back(vec3(v0, v1, v2));
}

void Mesh::insertTriangleIndex(int i0, int i1, int i2)
{
    if (!(i0 != i1 && i1 != i2 && i2 != i0)) std::cout<<i0<<","<<i1<<","<<i2<<std::endl;
    assert((i0 != i1 && i1 != i2 && i2 != i0) && "All three vertex indices must be different");


    triangleVertIndices.push_back(i0);
    triangleVertIndices.push_back(i1);
    triangleVertIndices.push_back(i2);
}

std::vector<Triangle> Mesh::generateTriangleList() const
{
    assert((triangleVertIndices.size() % 3 == 0) && "Missing indices for triangles");
    std::vector<Triangle> triangles;

    for(size_t i=0; i+2 < triangleVertIndices.size(); i += 3)
    {
        int idx0 = triangleVertIndices[i];
        int idx1 = triangleVertIndices[i + 1];
        int idx2 = triangleVertIndices[i + 2];

        const vec3& v0 = vertices[idx0];
        const vec3& v1 = vertices[idx1];
        const vec3& v2 = vertices[idx2];

        Color color = this->uniformColor;

        if (this->randomizeColors)
        {
            int r = rand() % 256;
            int g = rand() % 256;
            int b = rand() % 256;

            color = Color(r, g, b);
        }

        triangles.push_back(Triangle(v0, v1, v2, color));
    }

    return triangles;

}
