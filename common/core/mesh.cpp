#include "mesh.h"
#include "vec3.h"
#include "triangle.h"
#include <cstdlib>
#include <vector>
#include <cassert>

const double EPSILON = 1e-6;

Mesh::Mesh() : uniformColor(Color(1.0f, 1.0f, 1.0f)), randomizeColors(false)
{}

void Mesh::setUniformColor(const Color &color)
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
    assert((i0 != i1 && i1 != i2 && i2 != i0) && "All three vertex indices must be different");

    triangleVertIndices.push_back(i0);
    triangleVertIndices.push_back(i1);
    triangleVertIndices.push_back(i2);
}

Triangle Mesh::getTriangleByIndex(const int index) const
{
    assert(index >= 0 && (unsigned long long)index < triangleVertIndices.size() / 3);

    const vec3& v0 = vertices[triangleVertIndices[index*3]];
    const vec3& v1 = vertices[triangleVertIndices[index*3 + 1]];
    const vec3& v2 = vertices[triangleVertIndices[index*3 + 2]];

    return Triangle(v0, v1, v2, uniformColor);
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

void Mesh::setMaterial(const Material& material)
{
    this->material = material;
}

void Mesh::computeTriangleNormals()
{
    triangleNormals.clear();
    triangleNormals.reserve(triangleVertIndices.size()/3);

    for (size_t i = 0; i < triangleVertIndices.size(); i += 3)
    {
        vec3 v0 = vertices[triangleVertIndices[i]];
        vec3 v1 = vertices[triangleVertIndices[i + 1]];
        vec3 v2 = vertices[triangleVertIndices[i + 2]];

        vec3 edge1 = v1 - v0;
        vec3 edge2 = v2 - v0;

        vec3 normal = (edge1.cross(edge2)).normalized();
        triangleNormals.push_back(normal);
    }
}

void Mesh::computeVertexNormals()
{
    // init vector's normals to 0,0,0
    vertexNormals = std::vector<vec3>(vertices.size(), vec3(0, 0, 0));

    // for each vertex v of the triangle
    //  add the triangle t's normal to v
    for (size_t i = 0; i < triangleVertIndices.size(); i += 3)
    {
        const int i0 = triangleVertIndices[i];
        const int i1 = triangleVertIndices[i + 1];
        const int i2 = triangleVertIndices[i + 2];

        const vec3& triangleNormal = triangleNormals[i / 3];

        vertexNormals[i0] = vertexNormals[i0] + triangleNormal;
        vertexNormals[i1] = vertexNormals[i1] + triangleNormal;
        vertexNormals[i2] = vertexNormals[i2] + triangleNormal;
    }

    // normalize all vector normals
    for (vec3& norm : vertexNormals)
    {
        norm = norm.normalized();
    }
}

double Mesh::intersectRay(const Ray& r, int& hitTriangleIndex, vec3& hitPoint, vec3& hitNormal, bool cullBackFaces) const
{
    double minT = 1/EPSILON;
    hitTriangleIndex = -1;

    for (size_t i = 0; i < triangleVertIndices.size(); i += 3)
    {
        const vec3& v0 = vertices[triangleVertIndices[i]];
        const vec3& v1 = vertices[triangleVertIndices[i + 1]];
        const vec3& v2 = vertices[triangleVertIndices[i + 2]];
        const vec3& normal = triangleNormals[i / 3];

        if (cullBackFaces && normal.dot(r.d) >= -EPSILON) continue; // backface culling

        double denom = normal.dot(r.d);
        if (std::abs(denom) < EPSILON) continue; // parallel

        double t = -normal.dot(r.o - v0) / denom;
        if (t < 0 || t > minT) continue; // opposite direction

        vec3 p = r.o + r.d * t;

        vec3 e01 = v1 - v0;
        vec3 e12 = v2 - v1;
        vec3 e20 = v0 - v2;

        if (normal.dot(e01.cross(p - v0)) < EPSILON) continue;
        if (normal.dot(e12.cross(p - v1)) < EPSILON) continue;
        if (normal.dot(e20.cross(p - v2)) < EPSILON) continue;

        minT = t;
        hitPoint = p;
        hitNormal = normal;
        hitTriangleIndex = static_cast<int>(i / 3);
    }

    return (hitTriangleIndex != -1) ? minT : -1.0;
}
