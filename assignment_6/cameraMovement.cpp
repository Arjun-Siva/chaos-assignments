#include "vec3.h"
#include "ray.h"
#include "color.h"
#include "triangle.h"
#include "camera.h"
#include "renderPPM.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <memory>


int main() {
    const int width = 852;
    const int height = 480;

    std::vector<Triangle> triangles;

    // floor
    Triangle f0(vec3(-20,-1, 10), vec3(20, -1, 10), vec3(-10, -1.2, -100));
    f0.color = Color(166.0/255.0, 163.0/255.0, 162.0/255.0);
    triangles.push_back(f0);

    Triangle t1(vec3(-4, -1 ,-4), vec3(-2, -1, -5), vec3(-2, 2, -5));
    t1.color = Color(79.0f/255.0f, 79.0f/255.0f, 78.0f/255.0f);
    triangles.push_back(t1);

    Triangle t2(vec3(-2, 2, -5), vec3(-2, -1, -5), vec3(2, -1, -7));
    t2.color = Color(0, 1, 0);
    triangles.push_back(t2);

    Triangle t3(vec3(2, -1, -7), vec3(-4, -1 ,-4), vec3(-2, 2, -5));
    t3.color = Color(0, 0, 1);
    triangles.push_back(t3);

    Camera camera(width/height);
    camera.dolly(-1);
    camera.pan(25);

    for (int i = 0; i < 12; i++)
    {
        camera.dolly(0.5);
        camera.truck(-0.125);
        camera.pan(-5*i);

        std::string file = std::to_string(i)+"_frame.ppm";
        renderScene(file, width, height, camera, triangles, Color(0, 0, 0));
        camera.pan(5*i);
    }
    for (int i = 12; i < 24; i++)
    {
        camera.dolly(0.5);
        camera.truck(0.25);
        camera.pan(-7*i);

        std::string file = std::to_string(i)+"_frame.ppm";
        renderScene(file, width, height, camera, triangles, Color(0, 0, 0));
        camera.pan(7*i);
    }
    std::cout<<"Completed";
}
