#ifndef BVHNODE_H
#define BVHNODE_H

#include "aabb.h"
#include "triangle.h"
#include "ray.h"

#include <vector>
#include <memory>

class BVHNode
{
private:

public:
    AABB boundingBox;
//    int leftChildIdx;
//    int rightChildIdx;
    std::unique_ptr<BVHNode> left = nullptr;
    std::unique_ptr<BVHNode> right = nullptr;
    std::vector<std::pair<int, int>> triangleIndices; // object idx and triangle idx

    BVHNode();
    void createBB(std::vector<Triangle>& trianglesInNode);
};

#endif // BVHNODE_H
