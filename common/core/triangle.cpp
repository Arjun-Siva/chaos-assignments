#include "triangle.h"
#include "vec3.h"
#include "ray.h"
#include "color.h"

const double EPSILON = 1e-6;

Triangle::Triangle(const vec3 &a, const vec3 &b, const vec3 &c) : v0(a), v1(b), v2(c) {
    computeNormal();
}

Triangle::Triangle(const vec3 &a, const vec3 &b, const vec3 &c, const Color& color) : v0(a), v1(b), v2(c), color(color) {
    computeNormal();
}


void Triangle::computeNormal() {
    vec3 edge1 = this->v1 - this->v0;
    vec3 edge2 = this->v2 - this->v0;

    this->normal = (edge1.cross(edge2)).normalized();
}

double Triangle::intersect(const Ray &r) const {
    if (normal.dot(r.d) >= -EPSILON) return -1; //parallel, backface culling

    //t = -(normal . (O - v0)) / (normal . D)
    double t = -1 * normal.dot(r.o - v0) / normal.dot(r.d);
    if (t < 0) return -1;

    // intersection point
    vec3 p = r.o + r.d * t;

    vec3 e01 = v1 - v0;
    vec3 e12 = v2 - v1;
    vec3 e20 = v0 - v2;

    if (normal.dot(e01.cross(p-v0)) <= -EPSILON) return -1;
    if (normal.dot(e12.cross(p-v1)) <= -EPSILON) return -1;
    if (normal.dot(e20.cross(p-v2)) <= -EPSILON) return -1;

    return t;
}

void Triangle::setColor(const Color& newColor)
{
    this->color = newColor;
}
