#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3.h"
#include "ray.h"
#include "color.h"

class Triangle
{
public:
    vec3 v0, v1, v2;
    vec3 normal;
    Color color;

    Triangle(const vec3 &a,const vec3 &b, const vec3 &c);
    Triangle(const vec3 &a,const vec3 &b, const vec3 &c, const Color &color);
    double intersect(const Ray& r) const;
    void setColor(const Color& newColor);

private:
    void computeNormal();

};

#endif // TRIANGLE_H
