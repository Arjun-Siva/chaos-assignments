#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"

class Camera
{
private:
    vec3 eye;

    vec3 forward;
    vec3 right;
    vec3 up;

    float aspectRatio;
    float focalLength;


public:
    Camera(float aspectRatio);

    Camera(const vec3& eye, const vec3& target, const vec3& upVec, float aspectRatio, float focalLength);

    Ray generateRay(float u, float v) const;

    // movements

    // displacement
    void dolly(float distance);
    void boom(float distance);
    void truck(float distance);

    // rotation
    void pan(float degrees);
    void tilt(float degrees);
    void roll(float degrees);

};

#endif // CAMERA_H
